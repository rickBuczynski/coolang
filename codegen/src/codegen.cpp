#include "coolang/codegen/codegen.h"

#include <iostream>
#include "coolang/codegen/ast_to_code_map.h"
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
        ast_to_code_map_(&context_, module_.get(), &builder_, &program_ast) {}

  void Visit(const CaseExpr& node) override {}
  void Visit(const StrExpr& node) override;
  void Visit(const WhileExpr& node) override {}
  void Visit(const LetExpr& node) override;
  void Visit(const IntExpr& node) override;
  void Visit(const IsVoidExpr& node) override;
  void Visit(const MethodCallExpr& node) override;
  void Visit(const NotExpr& node) override {}
  void Visit(const IfExpr& node) override;
  void Visit(const NegExpr& node) override {}
  void Visit(const BlockExpr& node) override;
  void Visit(const ObjectExpr& node) override;
  void Visit(const BinOpExpr& node) override {}
  void Visit(const MultiplyExpr& node) override {}
  void Visit(const LessThanEqualCompareExpr& node) override {}
  void Visit(const SubtractExpr& node) override;
  void Visit(const AddExpr& node) override;
  void Visit(const EqCompareExpr& node) override;
  void Visit(const DivideExpr& node) override {}
  void Visit(const LessThanCompareExpr& node) override {}
  void Visit(const NewExpr& node) override;
  void Visit(const AssignExpr& node) override;
  void Visit(const BoolExpr& node) override {}
  void Visit(const ClassAst& node) override;
  void Visit(const CaseBranch& node) override {}
  void Visit(const MethodFeature& node) override {}
  void Visit(const AttributeFeature& node) override {}

  void Visit(const ProgramAst& node) override;

 private:
  llvm::Constant* CreateCStdFuncDecl(const std::string& func_name,
                                     const std::string& return_type,
                                     const std::vector<std::string>& arg_types,
                                     bool is_var_arg = false) {
    std::vector<llvm::Type*> llvm_arg_types;
    for (const auto& arg : arg_types) {
      llvm_arg_types.push_back(GetLlvmBasicType(arg));
    }

    llvm::FunctionType* func_type = llvm::FunctionType::get(
        GetLlvmBasicType(return_type), llvm_arg_types, is_var_arg);

    return module_->getOrInsertFunction(func_name, func_type);
  }

  llvm::Function* CreateAbortFunc() {
    std::vector<llvm::Type*> abort_args;
    abort_args.push_back(GetLlvmClassType("Object")->getPointerTo());

    llvm::FunctionType* abort_type = llvm::FunctionType::get(
        GetLlvmClassType("Object")->getPointerTo(), abort_args, false);

    llvm::Function* abort_func =
        llvm::Function::Create(abort_type, llvm::Function::ExternalLinkage,
                               "Object-abort", module_.get());

    llvm::BasicBlock* abort_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", abort_func);
    builder_.SetInsertPoint(abort_entry);

    // TODO abort the program instead of returning
    builder_.CreateRetVoid();

    return abort_func;
  }

  llvm::Function* CreateIoOutStringFunc() {
    std::vector<llvm::Type*> io_out_string_args;
    io_out_string_args.push_back(GetLlvmClassType("IO")->getPointerTo());
    io_out_string_args.push_back(builder_.getInt8Ty()->getPointerTo());

    llvm::FunctionType* io_out_string_type = llvm::FunctionType::get(
        GetLlvmClassType("IO")->getPointerTo(), io_out_string_args, false);

    llvm::Function* io_out_string_func = llvm::Function::Create(
        io_out_string_type, llvm::Function::ExternalLinkage, "IO-out_string",
        module_.get());

    llvm::BasicBlock* io_out_string_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", io_out_string_func);
    builder_.SetInsertPoint(io_out_string_entry);

    llvm::Value* format_str = builder_.CreateGlobalStringPtr("%s");
    auto arg_iterator = io_out_string_func->arg_begin();
    arg_iterator++;
    llvm::Value* args[] = {format_str, arg_iterator};
    builder_.CreateCall(printf_func_, args);
    builder_.CreateRet(io_out_string_func->arg_begin());

    return io_out_string_func;
  }

  llvm::Function* CreateIoOutIntFunc() {
    std::vector<llvm::Type*> io_out_int_args;
    io_out_int_args.push_back(GetLlvmClassType("IO")->getPointerTo());
    io_out_int_args.push_back(builder_.getInt32Ty());

    llvm::FunctionType* io_out_int_type = llvm::FunctionType::get(
        GetLlvmClassType("IO")->getPointerTo(), io_out_int_args, false);

    llvm::Function* io_out_int_func =
        llvm::Function::Create(io_out_int_type, llvm::Function::ExternalLinkage,
                               "IO-out_int", module_.get());

    llvm::BasicBlock* io_out_int_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", io_out_int_func);
    builder_.SetInsertPoint(io_out_int_entry);
    llvm::Value* format_str = builder_.CreateGlobalStringPtr("%d");
    auto arg_iterator = io_out_int_func->arg_begin();
    arg_iterator++;
    llvm::Value* args[] = {format_str, arg_iterator};
    builder_.CreateCall(printf_func_, args);
    builder_.CreateRet(io_out_int_func->arg_begin());

    return io_out_int_func;
  }

  llvm::Function* CreateStringConcatFunc() {
    std::vector<llvm::Type*> string_concat_args;
    string_concat_args.push_back(GetLlvmBasicType("String"));
    string_concat_args.push_back(GetLlvmBasicType("String"));

    llvm::FunctionType* string_concat_type = llvm::FunctionType::get(
        GetLlvmBasicType("String"), string_concat_args, false);

    llvm::Function* string_concat_func = llvm::Function::Create(
        string_concat_type, llvm::Function::ExternalLinkage, "String-concat",
        module_.get());

    llvm::BasicBlock* string_concat_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", string_concat_func);
    builder_.SetInsertPoint(string_concat_entry);

    auto arg_iter = string_concat_func->arg_begin();
    llvm::Value* lhs_arg = arg_iter;
    arg_iter++;
    llvm::Value* rhs_arg = arg_iter;

    llvm::Value* lhs_len = builder_.CreateCall(strlen_func_, {lhs_arg});
    llvm::Value* rhs_len = builder_.CreateCall(strlen_func_, {rhs_arg});
    llvm::Value* const_one =
        llvm::ConstantInt::get(context_, llvm::APInt(32, 1, true));
    llvm::Value* concated_len = builder_.CreateAdd(lhs_len, rhs_len);
    concated_len = builder_.CreateAdd(concated_len, const_one);

    // TODO this malloc leaks memory
    llvm::Value* concated_val =
        builder_.CreateCall(malloc_func_, {concated_len});
    builder_.CreateCall(strcpy_func_, {concated_val, lhs_arg});
    builder_.CreateCall(strcat_func_, {concated_val, rhs_arg});

    builder_.CreateRet(concated_val);

    return string_concat_func;
  }

  llvm::Function* CreateStringSubstrFunc() {
    std::vector<llvm::Type*> string_substr_args;
    string_substr_args.push_back(GetLlvmBasicType("String"));
    string_substr_args.push_back(GetLlvmBasicType("Int"));
    string_substr_args.push_back(GetLlvmBasicType("Int"));

    llvm::FunctionType* string_substr_type = llvm::FunctionType::get(
        GetLlvmBasicType("String"), string_substr_args, false);

    llvm::Function* string_substr_func = llvm::Function::Create(
        string_substr_type, llvm::Function::ExternalLinkage, "String-substr",
        module_.get());

    llvm::BasicBlock* string_substr_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", string_substr_func);
    builder_.SetInsertPoint(string_substr_entry);

    auto arg_iter = string_substr_func->arg_begin();
    llvm::Value* str_lhs = arg_iter;
    arg_iter++;
    llvm::Value* int_start_index = arg_iter;
    arg_iter++;
    llvm::Value* substr_len = arg_iter;

    llvm::Value* const_one =
        llvm::ConstantInt::get(context_, llvm::APInt(32, 1, true));
    llvm::Value* malloc_len = builder_.CreateAdd(substr_len, const_one);

    // TODO this malloc leaks memory
    llvm::Value* substr_val = builder_.CreateCall(malloc_func_, {malloc_len});
    llvm::Value* substr_start_ptr =
        builder_.CreateGEP(str_lhs, int_start_index);
    builder_.CreateCall(strncpy_func_,
                        {substr_val, substr_start_ptr, substr_len});

    llvm::Value* substr_last_ptr = builder_.CreateGEP(substr_val, substr_len);
    builder_.CreateStore(
        llvm::ConstantInt::get(context_, llvm::APInt(8, 0, true)),
        substr_last_ptr);

    builder_.CreateRet(substr_val);

    return string_substr_func;
  }

  llvm::Function* CreateStringLengthFunc() {
    std::vector<llvm::Type*> string_length_args;
    string_length_args.push_back(GetLlvmBasicType("String"));

    llvm::FunctionType* string_length_type = llvm::FunctionType::get(
        GetLlvmBasicType("Int"), string_length_args, false);

    llvm::Function* string_length_func = llvm::Function::Create(
        string_length_type, llvm::Function::ExternalLinkage, "String-length",
        module_.get());

    llvm::BasicBlock* string_length_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", string_length_func);
    builder_.SetInsertPoint(string_length_entry);

    llvm::Value* len_val =
        builder_.CreateCall(strlen_func_, {string_length_func->arg_begin()});
    builder_.CreateRet(len_val);

    return string_length_func;
  }

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

  const ClassAst* GetClassByName(std::string name) const {
    return ast_to_code_map_.GetClassByName(name);
  }

  llvm::Function* CreateConstructor(const ClassAst& node);
  llvm::Function* GetConstructor(const std::string& class_name) {
    return constructors_[GetClassByName(class_name)];
  }

  llvm::Function* GetLlvmFunction(const std::string& class_name,
                                  const std::string& method_name) {
    return functions_[GetClassByName(class_name)
                          ->GetMethodFeatureByName(method_name)];
  }
  void SetLlvmFunction(const std::string& class_name,
                       const std::string& method_name, llvm::Function* func) {
    functions_[GetClassByName(class_name)
                   ->GetMethodFeatureByName(method_name)] = func;
  }

  llvm::Type* GetLlvmBasicType(const std::string& class_name) const {
    return ast_to_code_map_.GetLlvmBasicType(class_name);
  }

  llvm::Type* GetLlvmClassType(const std::string& class_name) {
    return ast_to_code_map_.GetLlvmClassType(class_name);
  }

  llvm::Type* GetLlvmClassType(const ClassAst* class_ast) {
    return ast_to_code_map_.GetLlvmClassType(class_ast);
  }

  const Vtable& GetVtable(const ClassAst* class_ast) {
    return ast_to_code_map_.GetVtable(class_ast);
  }

  const ClassAst* CurClass() const {
    return ast_to_code_map_.GetCurrentClass();
  }

  const ProgramAst* GetProgramAst() const {
    return ast_to_code_map_.GetProgramAst();
  }

  llvm::Type* GetLlvmBasicOrPointerToClassType(const std::string& type_name) {
    return ast_to_code_map_.GetLlvmBasicOrPointerToClassType(type_name);
  }

  llvm::Value* GetLlvmBasicOrPointerDefaultVal(const std::string& type_name) {
    return ast_to_code_map_.GetLlvmBasicOrPointerDefaultVal(type_name);
  }

  int GetVtableIndexOfMethodFeature(const ClassAst* class_ast,
                                    const MethodFeature* method_feature) const;
  void SetupVtable(const ClassAst* class_ast);

  llvm::Value* GetAssignmentLhsPtr(const AssignExpr& node);

  std::unordered_map<const Expr*, llvm::Value*> codegened_values_;
  std::unordered_map<std::string, std::stack<llvm::AllocaInst*>> in_scope_vars_;

  std::unordered_map<const MethodFeature*, llvm::Function*> functions_;
  std::unordered_map<const ClassAst*, llvm::Function*> constructors_;

  const MethodFeature* current_method_ = nullptr;

  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  llvm::DataLayout data_layout_;
  llvm::IRBuilder<> builder_;
  AstToCodeMap ast_to_code_map_;

  llvm::Constant* printf_func_ =
      CreateCStdFuncDecl("printf", "Int", {"String"}, true);
  llvm::Constant* strlen_func_ =
      CreateCStdFuncDecl("strlen", "Int", {"String"});
  llvm::Constant* strcpy_func_ =
      CreateCStdFuncDecl("strcpy", "String", {"String", "String"});
  llvm::Constant* strncpy_func_ =
      CreateCStdFuncDecl("strncpy", "String", {"String", "String", "Int"});
  llvm::Constant* strcat_func_ =
      CreateCStdFuncDecl("strcat", "String", {"String", "String"});

  // use String (becomes char*) as return type for malloc
  // since llvm has no void* type
  llvm::Constant* malloc_func_ =
      CreateCStdFuncDecl("malloc", "String", {"Int"});
};

void CodegenVisitor::Visit(const StrExpr& node) {
  codegened_values_[&node] = builder_.CreateGlobalStringPtr(node.GetVal());
}

void CodegenVisitor::Visit(const LetExpr& node) {
  std::vector<std::pair<std::string, llvm::AllocaInst*>> bindings;
  const LetExpr* cur_let = &node;
  const Expr* in_expr = nullptr;

  while (in_expr == nullptr) {
    llvm::AllocaInst* alloca_inst = builder_.CreateAlloca(
        GetLlvmBasicOrPointerToClassType(cur_let->GetType()));
    bindings.emplace_back(cur_let->GetId(), alloca_inst);

    if (cur_let->GetInitializationExpr()) {
      cur_let->GetInitializationExpr()->Accept(*this);

      llvm::Value* init_val =
          codegened_values_[cur_let->GetInitializationExpr().get()];
      llvm::Type* let_type = GetLlvmBasicOrPointerToClassType(node.GetType());

      if (let_type != init_val->getType()) {
        init_val = builder_.CreateBitCast(init_val, let_type);
      }

      builder_.CreateStore(init_val, alloca_inst);
    } else {
      builder_.CreateStore(GetLlvmBasicOrPointerDefaultVal(cur_let->GetType()),
                           alloca_inst);
    }

    in_expr = cur_let->GetInExpr().get();
    cur_let = cur_let->GetChainedLet().get();
  }

  for (const auto& binding : bindings) {
    AddToScope(binding.first, binding.second);
  }

  in_expr->Accept(*this);

  for (const auto& binding : bindings) {
    RemoveFromScope(binding.first);
  }

  codegened_values_[&node] = codegened_values_.at(in_expr);
}

void CodegenVisitor::Visit(const IntExpr& node) {
  codegened_values_[&node] =
      llvm::ConstantInt::get(context_, llvm::APInt(32, node.GetVal(), true));
}

void CodegenVisitor::Visit(const IsVoidExpr& node) {
  node.GetChildExpr()->Accept(*this);

  llvm::Value* obj_ptr_as_int = builder_.CreatePtrToInt(
      codegened_values_.at(node.GetChildExpr()), builder_.getInt32Ty());

  codegened_values_[&node] = builder_.CreateICmpEQ(
      obj_ptr_as_int,
      llvm::ConstantInt::get(context_, llvm::APInt(32, 0, true)), "isvoidcmp");
}

void CodegenVisitor::Visit(const MethodCallExpr& node) {
  std::vector<llvm::Value*> arg_vals;

  const ClassAst* class_calling_method =
      GetClassByName(node.GetLhsExpr()->GetExprType());

  const auto method_feature =
      class_calling_method->GetMethodFeatureByName(node.GetMethodName());

  const ClassAst* class_that_defines_method =
      class_calling_method->GetClassOrSuperClassThatDefinesMethod(
          method_feature);

  node.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_val = codegened_values_.at(node.GetLhsExpr());

  if (class_calling_method == class_that_defines_method) {
    arg_vals.push_back(lhs_val);
  } else {
    auto* dest_type =
        GetLlvmClassType(class_that_defines_method)->getPointerTo();
    auto* casted_value = builder_.CreateBitCast(lhs_val, dest_type);
    arg_vals.push_back(casted_value);
  }

  for (size_t i = 0; i < method_feature->GetArgs().size(); i++) {
    node.GetArgs()[i]->Accept(*this);
    if (method_feature->GetArgs()[i].GetType() ==
        node.GetArgs()[i]->GetExprType()) {
      arg_vals.push_back(codegened_values_.at(node.GetArgs()[i].get()));
    } else {
      const auto dest_type =
          GetLlvmClassType(method_feature->GetArgs()[i].GetType())
              ->getPointerTo();
      auto* casted_value = builder_.CreateBitCast(
          codegened_values_.at(node.GetArgs()[i].get()), dest_type);
      arg_vals.push_back(casted_value);
    }
  }

  if (node.GetLhsExpr()->GetExprType() == "String") {
    // don't use a vtable for string methods since it can't be inherited from
    const auto called_method =
        GetLlvmFunction(node.GetLhsExpr()->GetExprType(), node.GetMethodName());
    codegened_values_[&node] = builder_.CreateCall(called_method, arg_vals);
  } else {
    llvm::Value* vtable_ptr_ptr = builder_.CreateStructGEP(
        GetLlvmClassType(node.GetLhsExpr()->GetExprType()), lhs_val, 0);
    llvm::Value* vtable_ptr = builder_.CreateLoad(vtable_ptr_ptr);
    const int method_offset_in_vtable =
        GetVtableIndexOfMethodFeature(class_calling_method, method_feature);
    llvm::Value* vtable_func_ptr =
        builder_.CreateStructGEP(nullptr, vtable_ptr, method_offset_in_vtable);
    llvm::Value* vtable_func = builder_.CreateLoad(vtable_func_ptr);
    codegened_values_[&node] = builder_.CreateCall(vtable_func, arg_vals);
  }
}

void CodegenVisitor::Visit(const IfExpr& node) {
  node.GetIfConditionExpr()->Accept(*this);
  llvm::Value* cond_val = codegened_values_.at(node.GetIfConditionExpr());

  llvm::BasicBlock* then_bb = llvm::BasicBlock::Create(
      context_, "then", functions_.at(current_method_));
  llvm::BasicBlock* else_bb = llvm::BasicBlock::Create(context_, "else");
  llvm::BasicBlock* done_bb = llvm::BasicBlock::Create(context_, "done-if");

  llvm::Type* return_type =
      GetLlvmBasicOrPointerToClassType(node.GetExprType());

  builder_.CreateCondBr(cond_val, then_bb, else_bb);

  // then block
  builder_.SetInsertPoint(then_bb);

  node.GetThenExpr()->Accept(*this);
  llvm::Value* then_val = codegened_values_.at(node.GetThenExpr());
  if (then_val->getType() != return_type) {
    then_val = builder_.CreateBitCast(then_val, return_type);
  }
  // Codegen of 'Then' can change the current block, update then_bb for the PHI.
  then_bb = builder_.GetInsertBlock();

  builder_.CreateBr(done_bb);

  // else block
  functions_.at(current_method_)->getBasicBlockList().push_back(else_bb);
  builder_.SetInsertPoint(else_bb);

  node.GetElseExpr()->Accept(*this);
  llvm::Value* else_val = codegened_values_.at(node.GetElseExpr());
  if (else_val->getType() != return_type) {
    else_val = builder_.CreateBitCast(else_val, return_type);
  }
  // Codegen of 'Else' can change the current block, update else_bb for the PHI.
  else_bb = builder_.GetInsertBlock();

  builder_.CreateBr(done_bb);

  // merge block
  functions_.at(current_method_)->getBasicBlockList().push_back(done_bb);
  builder_.SetInsertPoint(done_bb);

  llvm::PHINode* pn = builder_.CreatePHI(return_type, 2);
  pn->addIncoming(then_val, then_bb);
  pn->addIncoming(else_val, else_bb);

  codegened_values_[&node] = pn;
}

void CodegenVisitor::Visit(const BlockExpr& node) {
  for (const auto& sub_expr : node.GetExprs()) {
    sub_expr->Accept(*this);
  }
  codegened_values_[&node] = codegened_values_.at(node.GetExprs().back().get());
}

void CodegenVisitor::Visit(const ObjectExpr& node) {
  // First check for let binding, it has priority over class attributes and
  // method params
  auto in_scope_var = in_scope_vars_.find(node.GetId());
  if (in_scope_var != in_scope_vars_.end()) {
    codegened_values_[&node] = builder_.CreateLoad(in_scope_var->second.top());
    return;
  }

  if (node.GetId() == "self") {
    codegened_values_[&node] = functions_.at(current_method_)->args().begin();
    return;
  }

  // TODO maybe need super class attributes?
  // are attributes private or protected?
  for (size_t i = 0; i < CurClass()->GetAttributeFeatures().size(); i++) {
    const auto* attr = CurClass()->GetAttributeFeatures()[i];

    const int attribute_index = i + 1;  // offset 1 since vtable is at 0
    if (attr->GetId() == node.GetId()) {
      llvm::Value* element_ptr = builder_.CreateStructGEP(
          GetLlvmClassType(CurClass()),
          functions_.at(current_method_)->args().begin(), attribute_index);
      codegened_values_[&node] = builder_.CreateLoad(element_ptr);
      return;
    }
  }
}

void CodegenVisitor::Visit(const SubtractExpr& node) {
  node.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(node.GetLhsExpr().get());

  node.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(node.GetRhsExpr().get());

  codegened_values_[&node] = builder_.CreateSub(lhs_value, rhs_value);
}

void CodegenVisitor::Visit(const AddExpr& node) {
  node.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(node.GetLhsExpr().get());

  node.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(node.GetRhsExpr().get());

  codegened_values_[&node] = builder_.CreateAdd(lhs_value, rhs_value);
}

void CodegenVisitor::Visit(const EqCompareExpr& node) {
  node.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(node.GetLhsExpr().get());

  node.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(node.GetRhsExpr().get());

  // TODO handle types other than int
  if (node.GetLhsExpr()->GetExprType() == "Int") {
    codegened_values_[&node] = builder_.CreateICmpEQ(lhs_value, rhs_value);
  }
}

llvm::Function* CodegenVisitor::CreateConstructor(const ClassAst& node) {
  using namespace std::string_literals;

  std::vector<llvm::Type*> constructor_arg_types;

  constructor_arg_types.push_back(GetLlvmClassType(&node)->getPointerTo());

  llvm::FunctionType* constructor_func_type = llvm::FunctionType::get(
      builder_.getVoidTy(), constructor_arg_types, false);
  llvm::Function* constructor = llvm::Function::Create(
      constructor_func_type, llvm::Function::ExternalLinkage,
      "construct"s + "-" + node.GetName(), module_.get());
  llvm::BasicBlock* constructor_entry =
      llvm::BasicBlock::Create(context_, "entrypoint", constructor);
  builder_.SetInsertPoint(constructor_entry);

  // store the vtable first since initializers might make dynamic calls
  const int vtable_index = 0;
  llvm::Value* vtable_ptr_ptr = builder_.CreateStructGEP(
      GetLlvmClassType(&node), constructor->args().begin(), vtable_index);
  builder_.CreateStore(GetVtable(&node).GetGlobalInstance(), vtable_ptr_ptr);

  // TODO constructor should init super class attrs too

  // first store default values
  for (size_t i = 0; i < node.GetAttributeFeatures().size(); i++) {
    const auto* attr = node.GetAttributeFeatures()[i];

    const int attribute_index = i + 1;  // offset 1 since vtable is at 0
    llvm::Value* element_ptr = builder_.CreateStructGEP(
        GetLlvmClassType(&node), constructor->args().begin(), attribute_index);

    builder_.CreateStore(GetLlvmBasicOrPointerDefaultVal(attr->GetType()),
                         element_ptr);
  }

  auto dummy_constructor_method =
      MethodFeature(LineRange(0, 0), "constructor", {}, "", {});
  functions_[&dummy_constructor_method] = constructor;
  current_method_ = &dummy_constructor_method;

  // then store value from init expr
  for (size_t i = 0; i < node.GetAttributeFeatures().size(); i++) {
    const auto* attr = node.GetAttributeFeatures()[i];

    if (attr->GetRootExpr()) {
      attr->GetRootExpr()->Accept(*this);
      const int attribute_index = i + 1;  // offset 1 since vtable is at 0
      llvm::Value* element_ptr = builder_.CreateStructGEP(
          GetLlvmClassType(&node), constructor->args().begin(),
          attribute_index);
      builder_.CreateStore(codegened_values_.at(attr->GetRootExpr().get()),
                           element_ptr);
    }
  }

  current_method_ = nullptr;
  functions_.erase(&dummy_constructor_method);

  builder_.CreateRetVoid();
  return constructor;
}

void CodegenVisitor::Visit(const NewExpr& node) {
  if (GetLlvmBasicType(node.GetType()) != nullptr) {
    // TODO don't ignore "new" for basic types
    return;
  }

  llvm::Type* type = GetLlvmClassType(node.GetType());

  auto new_size = data_layout_.getTypeAllocSize(type);
  llvm::Value* malloc_len_val =
      llvm::ConstantInt::get(context_, llvm::APInt(32, new_size, true));

  llvm::Value* malloc_val = builder_.CreateCall(malloc_func_, {malloc_len_val});

  llvm::Value* new_val = builder_.CreateBitCast(
      malloc_val, GetLlvmClassType(node.GetType())->getPointerTo());

  std::vector<llvm::Value*> args;
  args.push_back(new_val);
  builder_.CreateCall(GetConstructor(node.GetType()), args);

  codegened_values_[&node] = new_val;
}

void CodegenVisitor::Visit(const AssignExpr& node) {
  node.GetRhsExpr()->Accept(*this);
  codegened_values_[&node] = codegened_values_.at(node.GetRhsExpr());

  llvm::Value* assign_lhs_ptr = GetAssignmentLhsPtr(node);
  llvm::Value* assign_rhs_val = codegened_values_.at(node.GetRhsExpr());

  builder_.CreateStore(assign_rhs_val, assign_lhs_ptr);
}

llvm::Value* CodegenVisitor::GetAssignmentLhsPtr(const AssignExpr& node) {
  const auto in_scope_var = in_scope_vars_.find(node.GetId());
  if (in_scope_var != in_scope_vars_.end()) {
    return in_scope_var->second.top();
  }

  // TODO maybe need super class attributes?
  // are attributes private or protected?
  for (size_t i = 0; i < CurClass()->GetAttributeFeatures().size(); i++) {
    const auto* attr = CurClass()->GetAttributeFeatures()[i];

    if (attr->GetId() == node.GetId()) {
      const int attribute_index = i + 1;  // offset 1 since vtable is at 0
      return builder_.CreateStructGEP(
          GetLlvmClassType(CurClass()),
          functions_.at(current_method_)->args().begin(), attribute_index);
    }
  }

  return nullptr;
}

void CodegenVisitor::Visit(const ClassAst& node) {
  SetupVtable(&node);

  ast_to_code_map_.SetCurrentClass(&node);

  for (const auto* method : node.GetMethodFeatures()) {
    llvm::Function* func = functions_.at(method);

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(context_, "entrypoint", func);
    builder_.SetInsertPoint(entry);

    auto arg_iter = func->arg_begin();
    arg_iter++;  // skip implicit self param
    for (const auto& arg : method->GetArgs()) {
      llvm::AllocaInst* alloca_inst = builder_.CreateAlloca(
          GetLlvmBasicOrPointerToClassType(arg.GetType()));
      builder_.CreateStore(arg_iter, alloca_inst);
      AddToScope(arg.GetId(), alloca_inst);
      arg_iter++;
    }

    current_method_ = method;
    method->GetRootExpr()->Accept(*this);
    current_method_ = nullptr;

    for (const auto& arg : method->GetArgs()) {
      RemoveFromScope(arg.GetId());
    }

    llvm::Type* return_type =
        GetLlvmBasicOrPointerToClassType(method->GetReturnType());

    if (codegened_values_.at(method->GetRootExpr().get())->getType() ==
        return_type) {
      builder_.CreateRet(codegened_values_.at(method->GetRootExpr().get()));
    } else {
      auto* casted_value = builder_.CreateBitCast(
          codegened_values_.at(method->GetRootExpr().get()), return_type);
      builder_.CreateRet(casted_value);
    }
  }

  constructors_[&node] = CreateConstructor(node);

  ast_to_code_map_.SetCurrentClass(nullptr);
  ClearScope();
}

int CodegenVisitor::GetVtableIndexOfMethodFeature(
    const ClassAst* class_ast, const MethodFeature* method_feature) const {
  std::vector<const ClassAst*> supers_and_class =
      class_ast->GetAllSuperClasses();
  std::reverse(supers_and_class.begin(), supers_and_class.end());
  supers_and_class.push_back(class_ast);
  std::set<std::string> already_seen_method_ids;
  int vtable_method_index = 0;
  for (const auto* cool_class : supers_and_class) {
    for (const auto* method : cool_class->GetMethodFeatures()) {
      if (method->GetId() == method_feature->GetId()) {
        return vtable_method_index;
      }
      if (already_seen_method_ids.find(method->GetId()) ==
          already_seen_method_ids.end()) {
        vtable_method_index++;
      }
      already_seen_method_ids.insert(method->GetId());
    }
  }
  throw std::invalid_argument("Method: " + method_feature->GetId() +
                              " not defined in class: " + class_ast->GetName() +
                              " or its super classes.");
}

void CodegenVisitor::SetupVtable(const ClassAst* class_ast) {
  if (GetVtable(class_ast).IsBuilt()) {
    return;
  }

  std::vector<llvm::Constant*> vtable_functions;

  if (class_ast->GetSuperClass() != nullptr) {
    SetupVtable(class_ast->GetSuperClass());

    const std::vector<llvm::Constant*>& super_vtable_functions =
        GetVtable(class_ast->GetSuperClass()).GetFunctions();

    vtable_functions.insert(vtable_functions.end(),
                            super_vtable_functions.begin(),
                            super_vtable_functions.end());
  }

  for (const auto* method : class_ast->GetMethodFeatures()) {
    llvm::Type* return_type =
        GetLlvmBasicOrPointerToClassType(method->GetReturnType());

    std::vector<llvm::Type*> arg_types;
    // first param is always implicit 'self'
    arg_types.push_back(GetLlvmClassType(class_ast)->getPointerTo());
    for (const auto& arg : method->GetArgs()) {
      arg_types.push_back(GetLlvmBasicOrPointerToClassType(arg.GetType()));
    }

    llvm::FunctionType* func_type =
        llvm::FunctionType::get(return_type, arg_types, false);
    llvm::Function* func = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage,
        class_ast->GetName() + "-" + method->GetId(), module_.get());
    functions_[method] = func;

    const int vtable_method_index =
        GetVtableIndexOfMethodFeature(class_ast, method);
    if (vtable_method_index < vtable_functions.size()) {
      // redefining a super method
      vtable_functions[vtable_method_index] = func;
    } else {
      assert(vtable_method_index == vtable_functions.size());
      vtable_functions.push_back(func);
    }
  }

  ast_to_code_map_.BuildVtable(class_ast, vtable_functions);
}

// instead of pushing constants on to the scope need to access main's struct
// data

void CodegenVisitor::Visit(const ProgramAst& node) {
  for (const auto& class_ast : node.GetClasses()) {
    ast_to_code_map_.Insert(&class_ast);
  }
  ast_to_code_map_.Insert(node.GetIoClass());
  ast_to_code_map_.Insert(node.GetObjectClass());

  llvm::Function* abort_func = CreateAbortFunc();
  llvm::Function* io_out_string_func = CreateIoOutStringFunc();
  llvm::Function* io_out_int_func = CreateIoOutIntFunc();

  // TODO repeating function to match number of method features
  // need to actually define functions for those
  ast_to_code_map_.BuildVtable(node.GetObjectClass(),
                               {abort_func, abort_func, abort_func});
  ast_to_code_map_.BuildVtable(
      node.GetIoClass(),
      {abort_func, abort_func, abort_func, io_out_string_func, io_out_int_func,
       io_out_int_func, io_out_int_func});

  SetLlvmFunction("Object", "abort", abort_func);
  SetLlvmFunction("IO", "out_string", io_out_string_func);
  SetLlvmFunction("IO", "out_int", io_out_int_func);
  SetLlvmFunction("String", "concat", CreateStringConcatFunc());
  SetLlvmFunction("String", "length", CreateStringLengthFunc());
  SetLlvmFunction("String", "substr", CreateStringSubstrFunc());

  for (const auto& class_ast : node.GetClasses()) {
    ast_to_code_map_.AddAttributes(&class_ast);
  }

  for (const auto& class_ast : node.GetClasses()) {
    class_ast.Accept(*this);
  }

  llvm::FunctionType* func_type =
      llvm::FunctionType::get(builder_.getVoidTy(), false);
  llvm::Function* func = llvm::Function::Create(
      func_type, llvm::Function::ExternalLinkage, "main", module_.get());
  llvm::BasicBlock* entry =
      llvm::BasicBlock::Create(context_, "entrypoint", func);
  builder_.SetInsertPoint(entry);

  llvm::AllocaInst* main_class =
      builder_.CreateAlloca(GetLlvmClassType("Main"));
  std::vector<llvm::Value*> args;
  args.push_back(main_class);

  builder_.CreateCall(GetConstructor("Main"), args);
  builder_.CreateCall(GetLlvmFunction("Main", "main"), args);
  builder_.CreateRetVoid();

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
  object_file_path.replace_extension(".obj");

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
  std::filesystem::path object_file_path = ast_->GetFilePath();
  object_file_path.replace_extension(".obj");

  std::string obj_input_linker_arg = object_file_path.string();
  obj_input_linker_arg += " ";

  std::filesystem::path exe_file_path = ast_->GetFilePath();

  if (exe_filename.has_value()) {
    exe_file_path.replace_filename(exe_filename.value());
  }

  exe_file_path.replace_extension(".exe");

  std::string output_exe_linker_arg = "-OUT:";
  output_exe_linker_arg += exe_file_path.string();
  output_exe_linker_arg += " ";

  // clang-format off
  std::string linker_cmd = "cmd /C \"";
  linker_cmd += "\"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/bin/Hostx86/x86/link.exe\" ";
  linker_cmd += obj_input_linker_arg;
  linker_cmd += output_exe_linker_arg;
  linker_cmd += "libcmt.lib ";
  // TODO windows requires legacy_stdio_definitions.lib in new versions of MSVC to link against printf but not in old versions
  // https://github.com/rust-lang/rust/issues/42830
  linker_cmd += "legacy_stdio_definitions.lib ";
  linker_cmd += "-LIBPATH:\"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/lib/x86\" ";
  linker_cmd += "-LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x86\" ";
  linker_cmd += "-LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/ucrt/x86\" ";
  linker_cmd += "-SUBSYSTEM:CONSOLE ";
  linker_cmd += "-MACHINE:X86";
  linker_cmd += "\"";
  // clang-format on
  std::cout << linker_cmd << std::endl;
  system(linker_cmd.c_str());
}

}  // namespace coolang
