#include "coolang/codegen/codegen.h"

#include <iostream>
#include "coolang/codegen/ast_to_code_map.h"
#include "coolang/codegen/c_std.h"
#include "coolang/codegen/io_codegen.h"
#include "coolang/codegen/object_codegen.h"
#include "coolang/codegen/platform.h"
#include "coolang/codegen/string_codegen.h"
#include "coolang/codegen/vtable.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/Optional.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Transforms/Scalar/GVN.h"

namespace coolang {

class CodegenVisitor : public ConstAstVisitor {
 public:
  explicit CodegenVisitor(const ProgramAst& program_ast)
      : module_(new llvm::Module(program_ast.GetFilePath().filename().string(),
                                 context_)),
        data_layout_(module_.get()),
        builder_(context_),
        ast_to_(&context_, module_.get(), &builder_, &data_layout_,
                &program_ast),
        c_std_(module_.get(), &ast_to_),
        object_codegen_(&context_, &builder_, &ast_to_, &c_std_) {}

  void Visit(const CaseExpr& case_expr) override;
  void Visit(const StrExpr& str) override;
  void Visit(const WhileExpr& while_expr) override;
  void Visit(const LetExpr& let_expr) override;
  void Visit(const IntExpr& int_expr) override;
  void Visit(const IsVoidExpr& is_void) override;
  void Visit(const MethodCallExpr& call_expr) override;
  void Visit(const NotExpr& not_expr) override;
  void Visit(const IfExpr& if_expr) override;
  void Visit(const NegExpr& neg_expr) override;
  void Visit(const BlockExpr& block) override;
  void Visit(const ObjectExpr& obj) override;
  void Visit(const BinOpExpr& binop) override {}
  void Visit(const MultiplyExpr& mult) override;
  void Visit(const LessThanEqualCompareExpr& le_expr) override;
  void Visit(const SubtractExpr& minus) override;
  void Visit(const AddExpr& add_expr) override;
  void Visit(const EqCompareExpr& eq_expr) override;
  void Visit(const DivideExpr& div_expr) override;
  void Visit(const LessThanCompareExpr& lt_expr) override;
  void Visit(const NewExpr& new_expr) override;
  void Visit(const AssignExpr& assign) override;
  void Visit(const BoolExpr& bool_expr) override;
  void Visit(const ClassAst& class_ast) override;
  void Visit(const CaseBranch& case_branch) override {}
  void Visit(const MethodFeature& method_feature) override {}
  void Visit(const AttributeFeature& attr_feature) override {}

  void Visit(const ProgramAst& prog) override;

 private:
  void ClearScope() { in_scope_vars_.clear(); }

  void RemoveFromScope(const std::string& id) {
    in_scope_vars_[id].pop();
    if (in_scope_vars_[id].empty()) {
      in_scope_vars_.erase(id);
    }
  }

  void AddToScope(const std::string& id, llvm::AllocaInst* val) {
    in_scope_vars_[id].push(val);
  }

  std::unordered_map<std::string, std::stack<llvm::AllocaInst*>> in_scope_vars_;

  llvm::Value* ConvertType(llvm::Value* convert_me, const std::string& cur_type,
                           const std::string& dest_type);
  llvm::Value* GenStrEqCmp(llvm::Value* lhs_value, llvm::Value* rhs_value);
  llvm::Value* GenIsVoid(llvm::Value* val);

  void GenExitIfVoid(llvm::Value* val, int line_num,
                     const std::string& exit_message);
  void GenExit(int line_num, const std::string& exit_message) const;

  llvm::Value* GenAllocAndConstruct(const std::string& type_name);

  llvm::Value* CreateBoxedBasic(const std::string& type_name,
                                llvm::Value* basic_val);
  llvm::Value* UnboxValue(const std::string& type_name, llvm::Value* boxed_val);

  void GenConstructor(const ClassAst& class_ast);
  void GenMethodBodies(const ClassAst& class_ast);

  void GenMainFunc();

  llvm::Value* GetAssignmentLhsPtr(const AssignExpr& assign);

  const ClassAst* CurClass() const { return ast_to_.CurClass(); }
  const ProgramAst* GetProgramAst() const { return ast_to_.GetProgramAst(); }

  std::unordered_map<const Expr*, llvm::Value*> codegened_values_;

  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  llvm::DataLayout data_layout_;
  llvm::IRBuilder<> builder_;
  AstToCodeMap ast_to_;
  CStd c_std_;
  ObjectCodegen object_codegen_;
};

int StructAttrIndex(const ClassAst* class_ast, int attribute_index) {
  int super_attr_count = 0;
  for (const ClassAst* super : class_ast->GetAllSuperClasses()) {
    super_attr_count += super->GetAttributeFeatures().size();
  }
  return super_attr_count + attribute_index +
         AstToCodeMap::obj_attributes_offset;
}

void CodegenVisitor::Visit(const CaseExpr& case_expr) {
  std::vector<const ClassAst*> classes;
  classes.push_back(GetProgramAst()->GetObjectClass());
  classes.push_back(GetProgramAst()->GetIoClass());
  classes.push_back(GetProgramAst()->GetIntClass());
  classes.push_back(GetProgramAst()->GetStringClass());
  classes.push_back(GetProgramAst()->GetBoolClass());
  for (const ClassAst* cool_class :
       GetProgramAst()->SortedClassesWithSupersBeforeSubs()) {
    classes.push_back(cool_class);
  }

  // We want the most specific type to match first
  std::reverse(classes.begin(), classes.end());

  std::unordered_map<std::string, const CaseBranch*> branches_by_type =
      case_expr.BranchesByType();

  case_expr.GetCaseExpr()->Accept(*this);
  llvm::Value* case_val = codegened_values_.at(case_expr.GetCaseExpr());

  if (!AstToCodeMap::IsBasicType(case_expr.GetCaseExpr()->GetExprType())) {
    GenExitIfVoid(case_val,
                  case_expr.GetCaseExpr()->GetLineRange().end_line_num,
                  "Match on void in case statement.");
  }

  llvm::Value* case_val_as_obj =
      ConvertType(case_val, case_expr.GetCaseExpr()->GetExprType(), "Object");
  llvm::Value* case_val_type = builder_.CreateCall(
      ast_to_.LlvmFunc("Object", "type_name"), {case_val_as_obj});

  llvm::BasicBlock* done_bb = llvm::BasicBlock::Create(context_, "done-case");

  std::vector<std::pair<llvm::Value*, llvm::BasicBlock*>> branch_vals_and_bbs;

  for (auto cool_class : classes) {
    const auto branch_find = branches_by_type.find(cool_class->GetName());
    if (branch_find != branches_by_type.end()) {
      const CaseBranch* branch = branch_find->second;

      llvm::Value* branch_type =
          builder_.CreateGlobalStringPtr(branch->GetType());

      llvm::BasicBlock* branch_taken = llvm::BasicBlock::Create(
          context_, branch->GetType(), ast_to_.CurLlvmFunc());

      llvm::BasicBlock* not_taken = llvm::BasicBlock::Create(
          context_, branch->GetType() + "-not-taken", ast_to_.CurLlvmFunc());

      llvm::Value* matches_branch = GenStrEqCmp(case_val_type, branch_type);
      std::vector<const ClassAst*> branch_descendants =
          ast_to_.GetClassByName(branch->GetType())->AllDescendantClasses();

      llvm::Value* should_take_branch = matches_branch;

      for (const ClassAst* branch_descendant : branch_descendants) {
        llvm::Value* descendant_type =
            builder_.CreateGlobalStringPtr(branch_descendant->GetName());
        llvm::Value* matches_descendant =
            GenStrEqCmp(case_val_type, descendant_type);
        should_take_branch =
            builder_.CreateOr(should_take_branch, matches_descendant);
      }

      builder_.CreateCondBr(should_take_branch, branch_taken, not_taken);

      builder_.SetInsertPoint(branch_taken);

      llvm::Value* case_val_as_branch_type = ConvertType(
          case_val, case_expr.GetCaseExpr()->GetExprType(), branch->GetType());
      llvm::AllocaInst* alloca_inst = builder_.CreateAlloca(
          ast_to_.LlvmBasicOrClassPtrTy(branch->GetType()));
      builder_.CreateStore(case_val_as_branch_type, alloca_inst);

      AddToScope(branch->GetId(), alloca_inst);
      branch->GetExpr()->Accept(*this);
      RemoveFromScope(branch->GetId());

      llvm::Value* branch_val = codegened_values_.at(branch->GetExpr());
      branch_val = ConvertType(branch_val, branch->GetExpr()->GetExprType(),
                               case_expr.GetExprType());
      branch_vals_and_bbs.emplace_back(branch_val, builder_.GetInsertBlock());

      builder_.CreateBr(done_bb);

      builder_.SetInsertPoint(not_taken);
    }
  }

  object_codegen_.GenExitWithMessage(
      "No match in case statement for Class %s\n", {case_val_type});

  branch_vals_and_bbs.emplace_back(
      ast_to_.LlvmBasicOrClassPtrDefaultVal(case_expr.GetExprType()),
      builder_.GetInsertBlock());

  builder_.CreateBr(done_bb);

  ast_to_.CurLlvmFunc()->getBasicBlockList().push_back(done_bb);
  builder_.SetInsertPoint(done_bb);

  llvm::PHINode* pn =
      builder_.CreatePHI(ast_to_.LlvmBasicOrClassPtrTy(case_expr.GetExprType()),
                         case_expr.GetBranches().size());
  for (const auto& phi_val_and_bb : branch_vals_and_bbs) {
    pn->addIncoming(phi_val_and_bb.first, phi_val_and_bb.second);
  }

  codegened_values_[&case_expr] = pn;
}

void CodegenVisitor::Visit(const StrExpr& str) {
  codegened_values_[&str] = builder_.CreateGlobalStringPtr(str.GetVal());
}

void CodegenVisitor::Visit(const WhileExpr& while_expr) {
  llvm::BasicBlock* loop_cond_bb =
      llvm::BasicBlock::Create(context_, "loop-cond", ast_to_.CurLlvmFunc());

  llvm::BasicBlock* loop_body_bb =
      llvm::BasicBlock::Create(context_, "loop-body", ast_to_.CurLlvmFunc());

  llvm::BasicBlock* loop_done_bb =
      llvm::BasicBlock::Create(context_, "loop-done", ast_to_.CurLlvmFunc());

  builder_.CreateBr(loop_cond_bb);

  builder_.SetInsertPoint(loop_cond_bb);
  while_expr.GetConditionExpr()->Accept(*this);
  llvm::Value* cond_val = codegened_values_.at(while_expr.GetConditionExpr());
  builder_.CreateCondBr(cond_val, loop_body_bb, loop_done_bb);

  builder_.SetInsertPoint(loop_body_bb);
  while_expr.GetLoopExpr()->Accept(*this);
  builder_.CreateBr(loop_cond_bb);

  builder_.SetInsertPoint(loop_done_bb);

  codegened_values_[&while_expr] =
      ast_to_.LlvmBasicOrClassPtrDefaultVal("Object");
}

llvm::Value* CodegenVisitor::ConvertType(llvm::Value* convert_me,
                                         const std::string& cur_type,
                                         const std::string& dest_type) {
  if (AstToCodeMap::IsBasicType(cur_type) && dest_type == "Object") {
    return CreateBoxedBasic(cur_type, convert_me);
  }
  if (AstToCodeMap::IsBasicType(cur_type) && dest_type != "Object" &&
      dest_type != cur_type) {
    // invalid conversion, this should only occur in a case branch that's not
    // taken so we can return anything, choose to just return default val
    return ast_to_.LlvmBasicOrClassPtrDefaultVal(dest_type);
  }
  if (AstToCodeMap::IsBasicType(dest_type) && cur_type == "Object") {
    return UnboxValue(dest_type, convert_me);
  }
  if (cur_type != dest_type) {
    return builder_.CreateBitCast(convert_me,
                                  ast_to_.LlvmBasicOrClassPtrTy(dest_type));
  }
  return convert_me;
}

void CodegenVisitor::Visit(const LetExpr& let_expr) {
  std::vector<std::pair<std::string, llvm::AllocaInst*>> bindings;
  const LetExpr* cur_let = &let_expr;
  const Expr* in_expr = nullptr;

  while (in_expr == nullptr) {
    llvm::AllocaInst* alloca_inst = builder_.CreateAlloca(
        ast_to_.LlvmBasicOrClassPtrTy(cur_let->GetType()));

    if (cur_let->GetInitializationExpr()) {
      cur_let->GetInitializationExpr()->Accept(*this);

      llvm::Value* init_val =
          codegened_values_.at(cur_let->GetInitializationExpr().get());
      llvm::Type* let_type = ast_to_.LlvmBasicOrClassPtrTy(cur_let->GetType());

      if (AstToCodeMap::IsBasicType(
              cur_let->GetInitializationExpr()->GetExprType()) &&
          cur_let->GetType() == "Object") {
        init_val = CreateBoxedBasic(
            cur_let->GetInitializationExpr()->GetExprType(), init_val);
      } else if (let_type != init_val->getType()) {
        init_val = builder_.CreateBitCast(init_val, let_type);
      }

      builder_.CreateStore(init_val, alloca_inst);
    } else {
      builder_.CreateStore(
          ast_to_.LlvmBasicOrClassPtrDefaultVal(cur_let->GetType()),
          alloca_inst);
    }

    AddToScope(cur_let->GetId(), alloca_inst);
    bindings.emplace_back(cur_let->GetId(), alloca_inst);

    in_expr = cur_let->GetInExpr().get();
    cur_let = cur_let->GetChainedLet().get();
  }

  in_expr->Accept(*this);

  for (const auto& binding : bindings) {
    RemoveFromScope(binding.first);
  }

  codegened_values_[&let_expr] = codegened_values_.at(in_expr);
}

void CodegenVisitor::Visit(const IntExpr& int_expr) {
  codegened_values_[&int_expr] = llvm::ConstantInt::get(
      context_, llvm::APInt(32, int_expr.GetValAsInt(), true));
}

void CodegenVisitor::Visit(const IsVoidExpr& is_void) {
  is_void.GetChildExpr()->Accept(*this);
  codegened_values_[&is_void] =
      GenIsVoid(codegened_values_.at(is_void.GetChildExpr()));
}

void CodegenVisitor::Visit(const MethodCallExpr& call_expr) {
  // codegen args first
  for (const auto& arg : call_expr.GetArgs()) {
    arg->Accept(*this);
  }
  // codegen LHS after all args
  call_expr.GetLhsExpr()->Accept(*this);

  const ClassAst* class_calling_method =
      ast_to_.GetClassByName(call_expr.GetLhsExpr()->GetExprType());

  const bool is_static_dispatch = call_expr.GetStaticDispatchType().has_value();
  if (is_static_dispatch) {
    class_calling_method =
        ast_to_.GetClassByName(call_expr.GetStaticDispatchType().value());
  }

  const auto method_feature =
      class_calling_method->GetMethodFeatureByName(call_expr.GetMethodName());

  const ClassAst* class_that_defines_method =
      class_calling_method->GetClassOrSuperClassThatDefinesMethod(
          method_feature);

  llvm::Value* lhs_val = codegened_values_.at(call_expr.GetLhsExpr());

  if (!AstToCodeMap::IsBasicType(call_expr.GetLhsExpr()->GetExprType())) {
    GenExitIfVoid(lhs_val, call_expr.GetLhsExpr()->GetLineRange().end_line_num,
                  "Dispatch to void.");
  }

  std::vector<llvm::Value*> arg_vals;

  if (ast_to_.GetClassByName(call_expr.GetLhsExpr()->GetExprType()) ==
      class_that_defines_method) {
    arg_vals.push_back(lhs_val);
  } else {
    auto* dest_type =
        ast_to_.LlvmClass(class_that_defines_method)->getPointerTo();
    auto* casted_value = builder_.CreateBitCast(lhs_val, dest_type);
    arg_vals.push_back(casted_value);
  }

  for (size_t i = 0; i < method_feature->GetArgs().size(); i++) {
    if (method_feature->GetArgs()[i].GetType() ==
        call_expr.GetArgs()[i]->GetExprType()) {
      arg_vals.push_back(codegened_values_.at(call_expr.GetArgs()[i].get()));
    } else {
      const auto dest_type =
          ast_to_.LlvmClass(method_feature->GetArgs()[i].GetType())
              ->getPointerTo();
      auto* casted_value = builder_.CreateBitCast(
          codegened_values_.at(call_expr.GetArgs()[i].get()), dest_type);
      arg_vals.push_back(casted_value);
    }
  }

  if (is_static_dispatch) {
    const auto called_method = ast_to_.LlvmFunc(
        call_expr.GetStaticDispatchType().value(), call_expr.GetMethodName());
    codegened_values_[&call_expr] =
        builder_.CreateCall(called_method, arg_vals);
  } else if (!AstToCodeMap::TypeUsesVtable(
                 call_expr.GetLhsExpr()->GetExprType())) {
    const auto called_method = ast_to_.LlvmFunc(
        call_expr.GetLhsExpr()->GetExprType(), call_expr.GetMethodName());
    codegened_values_[&call_expr] =
        builder_.CreateCall(called_method, arg_vals);
  } else {
    llvm::Value* vtable_ptr_ptr = builder_.CreateStructGEP(
        ast_to_.LlvmClass(call_expr.GetLhsExpr()->GetExprType()), lhs_val, 0);
    llvm::Value* vtable_ptr = builder_.CreateLoad(vtable_ptr_ptr);
    const int method_offset_in_vtable =
        ast_to_.GetVtable(class_calling_method)
            .GetIndexOfMethodFeature(method_feature);
    llvm::Value* vtable_func_ptr =
        builder_.CreateStructGEP(nullptr, vtable_ptr, method_offset_in_vtable);
    llvm::Value* vtable_func = builder_.CreateLoad(vtable_func_ptr);
    codegened_values_[&call_expr] = builder_.CreateCall(vtable_func, arg_vals);
  }

  llvm::Type* return_type =
      ast_to_.LlvmBasicOrClassPtrTy(call_expr.GetExprType());
  if (codegened_values_[&call_expr]->getType() != return_type) {
    codegened_values_[&call_expr] =
        builder_.CreateBitCast(codegened_values_[&call_expr], return_type);
  }
}

void CodegenVisitor::Visit(const NotExpr& not_expr) {
  not_expr.GetChildExpr()->Accept(*this);
  codegened_values_[&not_expr] =
      builder_.CreateNot(codegened_values_.at(not_expr.GetChildExpr()));
}

void CodegenVisitor::Visit(const IfExpr& if_expr) {
  if_expr.GetIfConditionExpr()->Accept(*this);
  llvm::Value* cond_val = codegened_values_.at(if_expr.GetIfConditionExpr());

  llvm::BasicBlock* then_bb =
      llvm::BasicBlock::Create(context_, "then", ast_to_.CurLlvmFunc());
  llvm::BasicBlock* else_bb = llvm::BasicBlock::Create(context_, "else");
  llvm::BasicBlock* done_bb = llvm::BasicBlock::Create(context_, "done-if");

  builder_.CreateCondBr(cond_val, then_bb, else_bb);

  // then block
  builder_.SetInsertPoint(then_bb);

  if_expr.GetThenExpr()->Accept(*this);
  llvm::Value* then_val = codegened_values_.at(if_expr.GetThenExpr());
  then_val = ConvertType(then_val, if_expr.GetThenExpr()->GetExprType(),
                         if_expr.GetExprType());

  // Codegen of 'Then' can change the current block, update then_bb for the PHI.
  then_bb = builder_.GetInsertBlock();

  builder_.CreateBr(done_bb);

  // else block
  ast_to_.CurLlvmFunc()->getBasicBlockList().push_back(else_bb);
  builder_.SetInsertPoint(else_bb);

  if_expr.GetElseExpr()->Accept(*this);
  llvm::Value* else_val = codegened_values_.at(if_expr.GetElseExpr());
  else_val = ConvertType(else_val, if_expr.GetElseExpr()->GetExprType(),
                         if_expr.GetExprType());
  // Codegen of 'Else' can change the current block, update else_bb for the PHI.
  else_bb = builder_.GetInsertBlock();

  builder_.CreateBr(done_bb);

  // merge block
  ast_to_.CurLlvmFunc()->getBasicBlockList().push_back(done_bb);
  builder_.SetInsertPoint(done_bb);

  llvm::PHINode* pn = builder_.CreatePHI(
      ast_to_.LlvmBasicOrClassPtrTy(if_expr.GetExprType()), 2);
  pn->addIncoming(then_val, then_bb);
  pn->addIncoming(else_val, else_bb);

  codegened_values_[&if_expr] = pn;
}

void CodegenVisitor::Visit(const NegExpr& neg_expr) {
  neg_expr.GetChildExpr()->Accept(*this);
  codegened_values_[&neg_expr] =
      builder_.CreateMul(ast_to_.LlvmConstInt32(-1),
                         codegened_values_.at(neg_expr.GetChildExpr()));
}

void CodegenVisitor::Visit(const BlockExpr& block) {
  for (const auto& sub_expr : block.GetExprs()) {
    sub_expr->Accept(*this);
  }
  codegened_values_[&block] =
      codegened_values_.at(block.GetExprs().back().get());
}

void CodegenVisitor::Visit(const ObjectExpr& obj) {
  // First check for let binding, it has priority over class attributes and
  // method params
  auto in_scope_var = in_scope_vars_.find(obj.GetId());
  if (in_scope_var != in_scope_vars_.end()) {
    codegened_values_[&obj] = builder_.CreateLoad(in_scope_var->second.top());
    return;
  }

  if (obj.GetId() == "self") {
    codegened_values_[&obj] = ast_to_.CurLlvmFunc()->args().begin();
    return;
  }

  for (const ClassAst* class_ast : CurClass()->SupersThenThis()) {
    for (size_t i = 0; i < class_ast->GetAttributeFeatures().size(); i++) {
      const auto* attr = class_ast->GetAttributeFeatures()[i];

      llvm::Value* cur_class_val = ast_to_.CurLlvmFunc()->args().begin();
      cur_class_val = ConvertType(cur_class_val, CurClass()->GetName(),
                                  class_ast->GetName());

      if (attr->GetId() == obj.GetId()) {
        llvm::Value* element_ptr = builder_.CreateStructGEP(
            ast_to_.LlvmClass(class_ast), cur_class_val,
            StructAttrIndex(class_ast, i));
        codegened_values_[&obj] = builder_.CreateLoad(element_ptr);
        return;
      }
    }
  }
}

void CodegenVisitor::Visit(const MultiplyExpr& mult) {
  mult.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(mult.GetLhsExpr().get());

  mult.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(mult.GetRhsExpr().get());

  codegened_values_[&mult] = builder_.CreateMul(lhs_value, rhs_value);
}

void CodegenVisitor::Visit(const LessThanEqualCompareExpr& le_expr) {
  le_expr.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_val = codegened_values_.at(le_expr.GetLhsExpr().get());

  le_expr.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_val = codegened_values_.at(le_expr.GetRhsExpr().get());

  codegened_values_[&le_expr] = builder_.CreateICmpSLE(lhs_val, rhs_val);
}

void CodegenVisitor::Visit(const SubtractExpr& minus) {
  minus.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(minus.GetLhsExpr().get());

  minus.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(minus.GetRhsExpr().get());

  codegened_values_[&minus] = builder_.CreateSub(lhs_value, rhs_value);
}

void CodegenVisitor::Visit(const AddExpr& add_expr) {
  add_expr.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(add_expr.GetLhsExpr().get());

  add_expr.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(add_expr.GetRhsExpr().get());

  codegened_values_[&add_expr] = builder_.CreateAdd(lhs_value, rhs_value);
}

llvm::Value* CodegenVisitor::GenStrEqCmp(llvm::Value* lhs_value,
                                         llvm::Value* rhs_value) {
  llvm::Value* strcmp_result =
      builder_.CreateCall(c_std_.GetStrCmpFunc(), {lhs_value, rhs_value});
  return builder_.CreateICmpEQ(strcmp_result, ast_to_.LlvmConstInt32(0));
}

llvm::Value* CodegenVisitor::GenIsVoid(llvm::Value* val) {
  return builder_.CreateICmpEQ(
      builder_.CreatePtrToInt(val, builder_.getInt32Ty()),
      llvm::ConstantInt::get(context_, llvm::APInt(32, 0, true)));
}

void CodegenVisitor::GenExitIfVoid(llvm::Value* val, int line_num,
                                   const std::string& exit_message) {
  llvm::Value* is_void_lhs = GenIsVoid(val);

  llvm::BasicBlock* is_void_bb =
      llvm::BasicBlock::Create(context_, "is-void", ast_to_.CurLlvmFunc());
  llvm::BasicBlock* not_void_bb =
      llvm::BasicBlock::Create(context_, "not-void", ast_to_.CurLlvmFunc());

  builder_.CreateCondBr(is_void_lhs, is_void_bb, not_void_bb);
  builder_.SetInsertPoint(is_void_bb);

  GenExit(line_num, exit_message);

  // will exit before taking this branch, but LLVM requires it
  builder_.CreateBr(not_void_bb);

  builder_.SetInsertPoint(not_void_bb);
}

void CodegenVisitor::GenExit(int line_num,
                             const std::string& exit_message) const {
  const std::string full_exit_message = GetProgramAst()->GetFileName() + ":" +
                                        std::to_string(line_num) + ": " +
                                        exit_message + "\n";
  object_codegen_.GenExitWithMessage(full_exit_message, {});
}

void CodegenVisitor::Visit(const EqCompareExpr& eq_expr) {
  eq_expr.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(eq_expr.GetLhsExpr().get());

  eq_expr.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(eq_expr.GetRhsExpr().get());

  if (eq_expr.GetLhsExpr()->GetExprType() == "Int" ||
      eq_expr.GetLhsExpr()->GetExprType() == "Bool") {
    codegened_values_[&eq_expr] = builder_.CreateICmpEQ(lhs_value, rhs_value);
    return;
  }

  if (eq_expr.GetLhsExpr()->GetExprType() == "String") {
    codegened_values_[&eq_expr] = GenStrEqCmp(lhs_value, rhs_value);
    return;
  }

  codegened_values_[&eq_expr] = builder_.CreateICmpEQ(
      builder_.CreatePtrToInt(lhs_value, builder_.getInt32Ty()),
      builder_.CreatePtrToInt(rhs_value, builder_.getInt32Ty()));
}

void CodegenVisitor::Visit(const DivideExpr& div_expr) {
  div_expr.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(div_expr.GetLhsExpr().get());

  div_expr.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(div_expr.GetRhsExpr().get());

  codegened_values_[&div_expr] = builder_.CreateSDiv(lhs_value, rhs_value);
}

void CodegenVisitor::GenConstructor(const ClassAst& class_ast) {
  llvm::Function* constructor = ast_to_.GetConstructor(&class_ast);

  llvm::BasicBlock* constructor_entry =
      llvm::BasicBlock::Create(context_, "entrypoint", constructor);
  builder_.SetInsertPoint(constructor_entry);

  // store the vtable first since initializers might make dynamic calls
  llvm::Value* vtable_ptr_ptr = builder_.CreateStructGEP(
      ast_to_.LlvmClass(&class_ast), constructor->args().begin(),
      AstToCodeMap::obj_vtable_index);
  builder_.CreateStore(ast_to_.GetVtable(&class_ast).GetGlobalInstance(),
                       vtable_ptr_ptr);

  // store the class type_name second
  llvm::Value* typename_ptr_ptr = builder_.CreateStructGEP(
      ast_to_.LlvmClass(&class_ast), constructor->args().begin(),
      AstToCodeMap::obj_typename_index);
  builder_.CreateStore(builder_.CreateGlobalStringPtr(class_ast.GetName()),
                       typename_ptr_ptr);

  // store the type_size third
  const auto type_size =
      data_layout_.getTypeAllocSize(ast_to_.LlvmClass(&class_ast));
  llvm::Value* typesize_ptr = builder_.CreateStructGEP(
      ast_to_.LlvmClass(&class_ast), constructor->args().begin(),
      AstToCodeMap::obj_typesize_index);
  builder_.CreateStore(
      llvm::ConstantInt::get(context_, llvm::APInt(32, type_size, true)),
      typesize_ptr);

  // store a pointer to this constructor 4th
  llvm::Value* constructor_ptr_ptr = builder_.CreateStructGEP(
      ast_to_.LlvmClass(&class_ast), constructor->args().begin(),
      AstToCodeMap::obj_constructor_index);
  builder_.CreateStore(constructor, constructor_ptr_ptr);

  // first store default values
  for (const ClassAst* cur_class : class_ast.SupersThenThis()) {
    for (size_t i = 0; i < cur_class->GetAttributeFeatures().size(); i++) {
      const AttributeFeature* attr = cur_class->GetAttributeFeatures()[i];

      llvm::Value* element_ptr = builder_.CreateStructGEP(
          ast_to_.LlvmClass(&class_ast), constructor->args().begin(),
          StructAttrIndex(cur_class, i));
      builder_.CreateStore(
          ast_to_.LlvmBasicOrClassPtrDefaultVal(attr->GetType()), element_ptr);
    }
  }

  auto dummy_constructor_method =
      MethodFeature(LineRange(0, 0), "constructor", {}, "", {});
  ast_to_.SetLlvmFunction(&dummy_constructor_method, constructor);
  ast_to_.SetCurrentMethod(&dummy_constructor_method);

  // then store value from init expr
  for (const ClassAst* cur_class : class_ast.SupersThenThis()) {
    for (size_t i = 0; i < cur_class->GetAttributeFeatures().size(); i++) {
      const AttributeFeature* attr = cur_class->GetAttributeFeatures()[i];

      if (attr->GetRootExpr()) {
        attr->GetRootExpr()->Accept(*this);
        llvm::Value* init_val = codegened_values_.at(attr->GetRootExpr().get());
        init_val = ConvertType(init_val, attr->GetRootExpr()->GetExprType(),
                               attr->GetType());

        llvm::Value* element_ptr = builder_.CreateStructGEP(
            ast_to_.LlvmClass(&class_ast), constructor->args().begin(),
            StructAttrIndex(cur_class, i));

        builder_.CreateStore(init_val, element_ptr);
      }
    }
  }

  ast_to_.SetCurrentMethod(nullptr);
  ast_to_.EraseMethod(&dummy_constructor_method);

  builder_.CreateRetVoid();
}

void CodegenVisitor::GenMethodBodies(const ClassAst& class_ast) {
  for (const auto* method : class_ast.GetMethodFeatures()) {
    llvm::Function* func = ast_to_.LlvmFunc(method);

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(context_, "entrypoint", func);
    builder_.SetInsertPoint(entry);

    auto arg_iter = func->arg_begin();
    arg_iter++;  // skip implicit self param
    for (const auto& arg : method->GetArgs()) {
      llvm::AllocaInst* alloca_inst =
          builder_.CreateAlloca(ast_to_.LlvmBasicOrClassPtrTy(arg.GetType()));
      builder_.CreateStore(arg_iter, alloca_inst);
      AddToScope(arg.GetId(), alloca_inst);
      arg_iter++;
    }

    ast_to_.SetCurrentMethod(method);
    method->GetRootExpr()->Accept(*this);
    ast_to_.SetCurrentMethod(nullptr);

    for (const auto& arg : method->GetArgs()) {
      RemoveFromScope(arg.GetId());
    }

    llvm::Value* retval = ConvertType(
        codegened_values_.at(method->GetRootExpr().get()),
        method->GetRootExpr()->GetExprType(), method->GetReturnType());
    builder_.CreateRet(retval);
  }
}

void CodegenVisitor::Visit(const LessThanCompareExpr& lt_expr) {
  lt_expr.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_val = codegened_values_.at(lt_expr.GetLhsExpr().get());

  lt_expr.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_val = codegened_values_.at(lt_expr.GetRhsExpr().get());

  codegened_values_[&lt_expr] = builder_.CreateICmpSLT(lhs_val, rhs_val);
}

void CodegenVisitor::Visit(const NewExpr& new_expr) {
  if (ast_to_.LlvmBasicType(new_expr.GetType()) != nullptr) {
    codegened_values_[&new_expr] =
        ast_to_.LlvmBasicOrClassPtrDefaultVal(new_expr.GetType());
    return;
  }

  if (new_expr.GetType() == "SELF_TYPE") {
    llvm::Value* cur_class_val = ast_to_.CurLlvmFunc()->args().begin();
    llvm::Value* cur_class_val_as_obj =
        ConvertType(cur_class_val, CurClass()->GetName(), "Object");
    llvm::Value* copied = builder_.CreateCall(
        ast_to_.LlvmFunc("Object", "copy"), {cur_class_val_as_obj});

    llvm::Value* constructor_func_ptr = builder_.CreateStructGEP(
        nullptr, copied, AstToCodeMap::obj_constructor_index);
    llvm::Value* constructor_func = builder_.CreateLoad(constructor_func_ptr);
    builder_.CreateCall(constructor_func, {copied});

    codegened_values_[&new_expr] = copied;
    return;
  }

  codegened_values_[&new_expr] = GenAllocAndConstruct(new_expr.GetType());
}

llvm::Value* CodegenVisitor::GenAllocAndConstruct(
    const std::string& type_name) {
  llvm::Type* type = ast_to_.LlvmClass(type_name);

  const auto new_size = data_layout_.getTypeAllocSize(type);
  llvm::Value* malloc_len_val =
      llvm::ConstantInt::get(context_, llvm::APInt(32, new_size, true));

  llvm::Value* malloc_val =
      builder_.CreateCall(c_std_.GetMallocFunc(), {malloc_len_val});

  llvm::Value* new_val = builder_.CreateBitCast(
      malloc_val, ast_to_.LlvmClass(type_name)->getPointerTo());

  std::vector<llvm::Value*> args;
  args.push_back(new_val);
  builder_.CreateCall(ast_to_.GetConstructor(type_name), args);

  return new_val;
}

llvm::Value* CodegenVisitor::CreateBoxedBasic(const std::string& type_name,
                                              llvm::Value* basic_val) {
  llvm::Value* boxed_val = GenAllocAndConstruct("Object");

  llvm::Value* typename_ptr_ptr = builder_.CreateStructGEP(
      ast_to_.LlvmClass("Object"), boxed_val, AstToCodeMap::obj_typename_index);

  builder_.CreateStore(builder_.CreateGlobalStringPtr(type_name),
                       typename_ptr_ptr);

  llvm::Value* boxed_data_ptr =
      builder_.CreateStructGEP(ast_to_.LlvmClass("Object"), boxed_val,
                               AstToCodeMap::obj_boxed_data_index);

  // TODO this could be tricky for GCing strings since both the orignal and the
  // boxed point to the string
  llvm::Value* boxed_data_as_i8ptr = basic_val;

  // if type_name is String we already have an i8ptr, but need to convert for
  // Bool or Int
  if (type_name == "Int" || type_name == "Bool") {
    boxed_data_as_i8ptr =
        builder_.CreateIntToPtr(basic_val, builder_.getInt8PtrTy());
  }

  builder_.CreateStore(boxed_data_as_i8ptr, boxed_data_ptr);

  return boxed_val;
}

llvm::Value* CodegenVisitor::UnboxValue(const std::string& type_name,
                                        llvm::Value* boxed_val) {
  llvm::Value* boxed_data_ptr =
      builder_.CreateStructGEP(ast_to_.LlvmClass("Object"), boxed_val,
                               AstToCodeMap::obj_boxed_data_index);
  llvm::Value* boxed_data_as_i8ptr = builder_.CreateLoad(boxed_data_ptr);

  if (type_name == "Int") {
    return builder_.CreatePtrToInt(boxed_data_as_i8ptr, builder_.getInt32Ty());
  }
  if (type_name == "Bool") {
    return builder_.CreatePtrToInt(boxed_data_as_i8ptr, builder_.getInt1Ty());
  }
  // no conversion needed for String
  return boxed_data_as_i8ptr;
}

void CodegenVisitor::Visit(const AssignExpr& assign) {
  assign.GetRhsExpr()->Accept(*this);
  codegened_values_[&assign] = codegened_values_.at(assign.GetRhsExpr());

  llvm::Value* assign_lhs_ptr = GetAssignmentLhsPtr(assign);
  llvm::Value* assign_rhs_val = codegened_values_.at(assign.GetRhsExpr());

  if (AstToCodeMap::IsBasicType(assign.GetRhsExpr()->GetExprType())) {
    auto* lhs_ty = assign_lhs_ptr->getType()->getPointerElementType();

    const bool lhs_is_basic_ty = lhs_ty == builder_.getInt1Ty() ||
                                 lhs_ty == builder_.getInt32Ty() ||
                                 lhs_ty == builder_.getInt8PtrTy();

    if (!lhs_is_basic_ty) {
      // The only non-basic thing you can assign a basic to is Object
      assign_rhs_val = ConvertType(
          assign_rhs_val, assign.GetRhsExpr()->GetExprType(), "Object");
    }
  } else if (assign_lhs_ptr->getType()->getPointerElementType() !=
             assign_rhs_val->getType()) {
    assign_rhs_val = builder_.CreateBitCast(
        assign_rhs_val, assign_lhs_ptr->getType()->getPointerElementType());
  }

  builder_.CreateStore(assign_rhs_val, assign_lhs_ptr);
}

void CodegenVisitor::Visit(const BoolExpr& bool_expr) {
  if (bool_expr.GetVal()) {
    codegened_values_[&bool_expr] =
        llvm::ConstantInt::get(context_, llvm::APInt(1, 1, false));
  } else if (!bool_expr.GetVal()) {
    codegened_values_[&bool_expr] =
        llvm::ConstantInt::get(context_, llvm::APInt(1, 0, false));
  }
}

llvm::Value* CodegenVisitor::GetAssignmentLhsPtr(const AssignExpr& assign) {
  const auto in_scope_var = in_scope_vars_.find(assign.GetId());
  if (in_scope_var != in_scope_vars_.end()) {
    return in_scope_var->second.top();
  }

  for (const ClassAst* class_ast : CurClass()->SupersThenThis()) {
    for (size_t i = 0; i < class_ast->GetAttributeFeatures().size(); i++) {
      const auto* attr = class_ast->GetAttributeFeatures()[i];

      llvm::Value* cur_class_val = ast_to_.CurLlvmFunc()->args().begin();
      cur_class_val = ConvertType(cur_class_val, CurClass()->GetName(),
                                  class_ast->GetName());

      if (attr->GetId() == assign.GetId()) {
        return builder_.CreateStructGEP(ast_to_.LlvmClass(class_ast),
                                        cur_class_val,
                                        StructAttrIndex(class_ast, i));
      }
    }
  }

  return nullptr;
}

void CodegenVisitor::Visit(const ClassAst& class_ast) {
  ast_to_.SetCurClass(&class_ast);

  GenConstructor(class_ast);
  GenMethodBodies(class_ast);

  ast_to_.SetCurClass(nullptr);
  ClearScope();
}

void CodegenVisitor::GenMainFunc() {
  llvm::FunctionType* func_type =
      llvm::FunctionType::get(builder_.getVoidTy(), false);
  llvm::Function* func = llvm::Function::Create(
      func_type, llvm::Function::ExternalLinkage, "main", module_.get());
  llvm::BasicBlock* entry =
      llvm::BasicBlock::Create(context_, "entrypoint", func);
  builder_.SetInsertPoint(entry);

  llvm::AllocaInst* main_class =
      builder_.CreateAlloca(ast_to_.LlvmClass("Main"));
  std::vector<llvm::Value*> args;
  args.push_back(main_class);

  builder_.CreateCall(ast_to_.GetConstructor("Main"), args);
  builder_.CreateCall(ast_to_.LlvmFunc("Main", "main"), args);
  builder_.CreateRetVoid();
}

void CodegenVisitor::Visit(const ProgramAst& prog) {
  for (const auto& class_ast : prog.GetClasses()) {
    ast_to_.Insert(&class_ast);
  }
  ast_to_.Insert(prog.GetIoClass());
  ast_to_.Insert(prog.GetObjectClass());

  ast_to_.AddMethods(prog.GetObjectClass());
  ast_to_.AddMethods(prog.GetIoClass());

  ast_to_.AddAttributes(prog.GetObjectClass());
  ast_to_.AddAttributes(prog.GetIoClass());

  ast_to_.AddConstructor(prog.GetObjectClass());
  ast_to_.AddConstructor(prog.GetIoClass());

  // just sets up ast to function mapping and creates func definitions
  ast_to_.AddMethods(prog.GetStringClass());
  ast_to_.AddMethods(prog.GetIntClass());
  ast_to_.AddMethods(prog.GetBoolClass());

  object_codegen_.GenAllFuncBodies();

  IoCodegen io_codegen(&context_, &builder_, &ast_to_, &c_std_);
  io_codegen.GenAllFuncBodies();

  StringCodegen string_codegen(&context_, &builder_, &ast_to_, &c_std_);
  string_codegen.GenAllFuncBodies();

  for (const auto& class_ast : prog.GetClasses()) {
    ast_to_.AddAttributes(&class_ast);
    ast_to_.AddConstructor(&class_ast);
  }

  for (const ClassAst* class_ast : prog.SortedClassesWithSupersBeforeSubs()) {
    ast_to_.AddMethods(class_ast);
  }

  GenConstructor(*prog.GetObjectClass());
  GenConstructor(*prog.GetIoClass());

  GenMainFunc();

  for (const auto& class_ast : prog.GetClasses()) {
    class_ast.Accept(*this);
  }

  module_->print(llvm::errs(), nullptr);

  // Initialize the target registry etc.
  LLVMInitializeX86TargetInfo();
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetMC();
  LLVMInitializeX86AsmParser();
  LLVMInitializeX86AsmPrinter();

  const auto target_triple = llvm::sys::getDefaultTargetTriple();
  module_->setTargetTriple(target_triple);

  std::string error;
  const auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!target) {
    llvm::errs() << error;
    return;
  }

  const auto cpu = "generic";
  const auto features = "";

  const llvm::TargetOptions opt;
  const auto rm = llvm::Optional<llvm::Reloc::Model>();
  auto the_target_machine =
      target->createTargetMachine(target_triple, cpu, features, opt, rm);

  module_->setDataLayout(the_target_machine->createDataLayout());

  std::error_code ec;

  std::filesystem::path object_file_path = GetProgramAst()->GetFilePath();
  object_file_path.replace_extension(platform::GetObjectFileExtension());

  llvm::raw_fd_ostream dest(object_file_path.string(), ec,
                            llvm::sys::fs::F_None);

  if (ec) {
    llvm::errs() << "Could not open file: " << ec.message();
    return;
  }

  llvm::legacy::PassManager pass;
  const auto file_type = llvm::TargetMachine::CGFT_ObjectFile;

  if (the_target_machine->addPassesToEmitFile(pass, dest, file_type)) {
    llvm::errs() << "TheTargetMachine can't emit a file of this type";
    return;
  }

  pass.run(*module_);
  dest.flush();

  std::cout << "ast_->GetFileName() " << GetProgramAst()->GetFileName() << "\n";

  std::cout << "Wrote " << object_file_path.string() << "\n";
}

void Codegen::GenerateCode() const {
  CodegenVisitor codegen_visitor(*ast_);
  ast_->Accept(codegen_visitor);
}

void Codegen::Link(const std::optional<std::string>& exe_filename) const {
  std::string linker_cmd =
      platform::GetLinkerCommand(ast_->GetFilePath(), exe_filename);
  std::cout << linker_cmd << std::endl;
  system(linker_cmd.c_str());
}

}  // namespace coolang
