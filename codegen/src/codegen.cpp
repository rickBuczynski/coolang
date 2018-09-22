#include "coolang/codegen/codegen.h"

#include <iostream>
#include "coolang/codegen/ast_to_code_map.h"
#include "coolang/codegen/c_std.h"
#include "coolang/codegen/io_codegen.h"
#include "coolang/codegen/object_codegen.h"
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
        ast_to_code_map_(&context_, module_.get(), &builder_, &program_ast),
        c_std_(module_.get(), &ast_to_code_map_) {}

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

  void GenConstructor(const ClassAst& node);

  llvm::Function* GetLlvmFunction(const std::string& class_name,
                                  const std::string& method_name) {
    return ast_to_code_map_.GetLlvmFunction(class_name, method_name);
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

  llvm::Value* GetAssignmentLhsPtr(const AssignExpr& node);

  std::unordered_map<const Expr*, llvm::Value*> codegened_values_;
  std::unordered_map<std::string, std::stack<llvm::AllocaInst*>> in_scope_vars_;

  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  llvm::DataLayout data_layout_;
  llvm::IRBuilder<> builder_;
  AstToCodeMap ast_to_code_map_;
  CStd c_std_;
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
        ast_to_code_map_.GetVtable(class_calling_method)
            .GetIndexOfMethodFeature(method_feature);
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
      context_, "then", ast_to_code_map_.CurLlvmFunc());
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
  ast_to_code_map_.CurLlvmFunc()->getBasicBlockList().push_back(else_bb);
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
  ast_to_code_map_.CurLlvmFunc()->getBasicBlockList().push_back(done_bb);
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
    codegened_values_[&node] = ast_to_code_map_.CurLlvmFunc()->args().begin();
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
          ast_to_code_map_.CurLlvmFunc()->args().begin(), attribute_index);
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

void CodegenVisitor::GenConstructor(const ClassAst& node) {
  llvm::Function* constructor = ast_to_code_map_.GetConstructor(&node);

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
  ast_to_code_map_.SetLlvmFunction(&dummy_constructor_method, constructor);
  ast_to_code_map_.SetCurrentMethod(&dummy_constructor_method);

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

  ast_to_code_map_.SetCurrentMethod(nullptr);
  ast_to_code_map_.EraseMethod(&dummy_constructor_method);

  builder_.CreateRetVoid();
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

  llvm::Value* malloc_val =
      builder_.CreateCall(c_std_.GetMallocFunc(), {malloc_len_val});

  llvm::Value* new_val = builder_.CreateBitCast(
      malloc_val, GetLlvmClassType(node.GetType())->getPointerTo());

  std::vector<llvm::Value*> args;
  args.push_back(new_val);
  builder_.CreateCall(ast_to_code_map_.GetConstructor(node.GetType()), args);

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
          ast_to_code_map_.CurLlvmFunc()->args().begin(), attribute_index);
    }
  }

  return nullptr;
}

void CodegenVisitor::Visit(const ClassAst& node) {
  ast_to_code_map_.SetCurrentClass(&node);

  GenConstructor(node);

  for (const auto* method : node.GetMethodFeatures()) {
    llvm::Function* func = ast_to_code_map_.GetLlvmFunction(method);

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

    ast_to_code_map_.SetCurrentMethod(method);
    method->GetRootExpr()->Accept(*this);
    ast_to_code_map_.SetCurrentMethod(nullptr);

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

  ast_to_code_map_.SetCurrentClass(nullptr);
  ClearScope();
}

// instead of pushing constants on to the scope need to access main's struct
// data

void CodegenVisitor::Visit(const ProgramAst& node) {
  for (const auto& class_ast : node.GetClasses()) {
    ast_to_code_map_.Insert(&class_ast);
  }
  ast_to_code_map_.Insert(node.GetIoClass());
  ast_to_code_map_.Insert(node.GetObjectClass());

  ast_to_code_map_.AddMethods(node.GetObjectClass());
  ast_to_code_map_.AddMethods(node.GetIoClass());

  // just sets up ast to function mapping and creates func definitions
  ast_to_code_map_.AddMethods(node.GetStringClass());

  ObjectCodegen object_codegen(&context_, &builder_, &ast_to_code_map_);
  object_codegen.GenAllFuncBodies();

  IoCodegen io_codegen(&context_, &builder_, &ast_to_code_map_, &c_std_);
  io_codegen.GenAllFuncBodies();

  StringCodegen string_codegen(&context_, &builder_, &ast_to_code_map_,
                               &c_std_);
  string_codegen.GenAllFuncBodies();

  for (const auto& class_ast : node.GetClasses()) {
    ast_to_code_map_.AddAttributes(&class_ast);
    ast_to_code_map_.AddConstructor(&class_ast);
  }

  for (const ClassAst* class_ast : node.SortedClassesWithSupersBeforeSubs()) {
    ast_to_code_map_.AddMethods(class_ast);
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

  builder_.CreateCall(ast_to_code_map_.GetConstructor("Main"), args);
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
