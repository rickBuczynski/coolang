#include "coolang/codegen/codegen.h"

#include <iostream>
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

namespace coolang {

class CodegenVisitor : public ConstAstVisitor {
 public:
  explicit CodegenVisitor(const ProgramAst& program_ast)
      : program_ast_(&program_ast),
        module_(new llvm::Module("TODOMODULENAME", context_)),
        builder_(context_) {
    printf_func_ = CreatePrintfFunc();
  }

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
  void Visit(const SubtractExpr& node) override {}
  void Visit(const AddExpr& node) override;
  void Visit(const EqCompareExpr& node) override {}
  void Visit(const DivideExpr& node) override {}
  void Visit(const LessThanCompareExpr& node) override {}
  void Visit(const NewExpr& node) override {}
  void Visit(const AssignExpr& node) override;
  void Visit(const BoolExpr& node) override {}
  void Visit(const ClassAst& node) override;
  void Visit(const CaseBranch& node) override {}
  void Visit(const MethodFeature& node) override {}
  void Visit(const AttributeFeature& node) override {}

  void Visit(const ProgramAst& node) override;

 private:
  llvm::Constant* CreatePrintfFunc() {
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(context_));

    llvm::FunctionType* printf_type =
        llvm::FunctionType::get(builder_.getInt32Ty(), printf_arg_types, true);

    return module_->getOrInsertFunction("printf", printf_type);
  }
  llvm::Constant* printf_func_;

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

    // TODO actually concat the strings
    builder_.CreateRet(string_concat_func->arg_begin());

    return string_concat_func;
  }

  void ClearScope() { let_binding_vars_.clear(); }

  void RemoveFromScope(const std::string& id) {
    let_binding_vars_[id].pop();
    if (let_binding_vars_[id].empty()) {
      let_binding_vars_.erase(id);
    }
  }

  void AddToScope(const std::string& id, llvm::AllocaInst* val) {
    let_binding_vars_[id].push(val);
  }

  const ClassAst* GetClassByName(std::string name) const {
    if (name == "SELF_TYPE") {
      name = current_class_->GetName();
    }
    return program_ast_->GetClassByName(name);
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

  llvm::Type* GetLlvmBasicType(const std::string& class_name) {
    if (class_name == "Int") {
      return builder_.getInt32Ty();
    }
    if (class_name == "String") {
      return builder_.getInt8PtrTy();
    }
    if (class_name == "Bool") {
      return builder_.getInt1Ty();
    }
    return nullptr;
  }

  llvm::Type* GetLlvmClassType(const std::string& class_name) {
    return classes_[GetClassByName(class_name)];
  }

  llvm::Type* GetLlvmBasicOrPointerToClassType(const std::string& type_name) {
    llvm::Type* type = GetLlvmBasicType(type_name);
    if (type == nullptr) {
      type = GetLlvmClassType(type_name)->getPointerTo();
    }
    return type;
  }

  llvm::Value* GetLlvmBasicOrPointerDefaultVal(const std::string& type_name) {
    if (type_name == "Int") {
      return llvm::ConstantInt::get(context_, llvm::APInt(32, 0, true));
    }
    if (type_name == "String") {
      return builder_.CreateGlobalStringPtr("");
    }
    if (type_name == "Bool") {
      return llvm::ConstantInt::get(context_, llvm::APInt(1, 0, false));
    }
    return llvm::ConstantPointerNull::get(
        GetLlvmClassType(type_name)->getPointerTo());
  }

  std::unordered_map<const Expr*, llvm::Value*> codegened_values_;

  std::unordered_map<std::string, std::stack<llvm::AllocaInst*>>
      let_binding_vars_;

  std::unordered_map<const MethodFeature*, llvm::Function*> functions_;
  std::unordered_map<const ClassAst*, llvm::StructType*> classes_;
  std::unordered_map<const ClassAst*, llvm::Function*> constructors_;

  const MethodFeature* current_method_ = nullptr;
  const ClassAst* current_class_ = nullptr;
  const ProgramAst* program_ast_;

  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  llvm::IRBuilder<> builder_;
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
      builder_.CreateStore(
          codegened_values_[cur_let->GetInitializationExpr().get()],
          alloca_inst);
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
    auto* dest_type = classes_[class_that_defines_method]->getPointerTo();
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

  const auto called_method =
      GetLlvmFunction(node.GetLhsExpr()->GetExprType(), node.GetMethodName());

  codegened_values_[&node] = builder_.CreateCall(called_method, arg_vals);
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

  builder_.CreateBr(done_bb);

  // else block
  functions_.at(current_method_)->getBasicBlockList().push_back(else_bb);
  builder_.SetInsertPoint(else_bb);

  node.GetElseExpr()->Accept(*this);
  llvm::Value* else_val = codegened_values_.at(node.GetElseExpr());
  if (else_val->getType() != return_type) {
    else_val = builder_.CreateBitCast(else_val, return_type);
  }

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
  auto let_binding = let_binding_vars_.find(node.GetId());
  if (let_binding != let_binding_vars_.end()) {
    codegened_values_[&node] = builder_.CreateLoad(let_binding->second.top());
  }

  if (node.GetId() == "self") {
    codegened_values_[&node] = functions_.at(current_method_)->args().begin();
    return;
  }

  auto arg_iter = functions_.at(current_method_)->arg_begin();
  arg_iter++;  // skip implicit self param
  for (const auto& arg : current_method_->GetArgs()) {
    if (arg.GetId() == node.GetId()) {
      codegened_values_[&node] = arg_iter;
      return;
    }

    arg_iter++;
  }

  // TODO maybe need super class attributes?
  // are attributes private or protected?
  for (size_t i = 0; i < current_class_->GetAttributeFeatures().size(); i++) {
    const auto* attr = current_class_->GetAttributeFeatures()[i];

    if (attr->GetId() == node.GetId()) {
      llvm::Value* element_ptr = builder_.CreateStructGEP(
          classes_[current_class_],
          functions_.at(current_method_)->args().begin(), i);
      codegened_values_[&node] = builder_.CreateLoad(element_ptr);
      return;
    }
  }
}

void CodegenVisitor::Visit(const AddExpr& node) {
  node.GetLhsExpr()->Accept(*this);
  llvm::Value* lhs_value = codegened_values_.at(node.GetLhsExpr().get());

  node.GetRhsExpr()->Accept(*this);
  llvm::Value* rhs_value = codegened_values_.at(node.GetRhsExpr().get());

  codegened_values_[&node] = builder_.CreateAdd(lhs_value, rhs_value);
}

llvm::Function* CodegenVisitor::CreateConstructor(const ClassAst& node) {
  using namespace std::string_literals;

  std::vector<llvm::Type*> constructor_arg_types;
  constructor_arg_types.push_back(classes_[&node]->getPointerTo());

  llvm::FunctionType* constructor_func_type = llvm::FunctionType::get(
      builder_.getVoidTy(), constructor_arg_types, false);
  llvm::Function* constructor = llvm::Function::Create(
      constructor_func_type, llvm::Function::ExternalLinkage,
      "construct"s + "-" + node.GetName(), module_.get());
  llvm::BasicBlock* constructor_entry =
      llvm::BasicBlock::Create(context_, "entrypoint", constructor);
  builder_.SetInsertPoint(constructor_entry);

  // TODO constructor should init super class attrs too

  // first store default values
  for (size_t i = 0; i < node.GetAttributeFeatures().size(); i++) {
    const auto* attr = node.GetAttributeFeatures()[i];

    llvm::Value* element_ptr = builder_.CreateStructGEP(
        classes_[&node], constructor->args().begin(), i);

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
      llvm::Value* element_ptr = builder_.CreateStructGEP(
          classes_[&node], constructor->args().begin(), i);
      builder_.CreateStore(codegened_values_.at(attr->GetRootExpr().get()),
                           element_ptr);
    }
  }

  current_method_ = nullptr;
  functions_.erase(&dummy_constructor_method);

  builder_.CreateRetVoid();
  return constructor;
}

void CodegenVisitor::Visit(const AssignExpr& node) {
  node.GetRhsExpr()->Accept(*this);

  const auto let_binding = let_binding_vars_.find(node.GetId());
  if (let_binding != let_binding_vars_.end()) {
    builder_.CreateStore(codegened_values_[node.GetRhsExpr()],
                         let_binding->second.top());
  }
  // TODO handle assign of method params and object attributes

  codegened_values_[&node] = codegened_values_.at(node.GetRhsExpr());
}

void CodegenVisitor::Visit(const ClassAst& node) {
  current_class_ = &node;

  for (const auto* method : node.GetMethodFeatures()) {
    llvm::Type* return_type =
        GetLlvmBasicOrPointerToClassType(method->GetReturnType());

    std::vector<llvm::Type*> arg_types;
    // first param is always implicit 'self'
    arg_types.push_back(classes_[&node]->getPointerTo());
    for (const auto& arg : method->GetArgs()) {
      arg_types.push_back(GetLlvmBasicOrPointerToClassType(arg.GetType()));
    }

    llvm::FunctionType* func_type =
        llvm::FunctionType::get(return_type, arg_types, false);
    llvm::Function* func = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage,
        node.GetName() + "-" + method->GetId(), module_.get());

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(context_, "entrypoint", func);
    builder_.SetInsertPoint(entry);

    current_method_ = method;
    functions_[current_method_] = func;
    method->GetRootExpr()->Accept(*this);
    current_method_ = nullptr;

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

  current_class_ = nullptr;
  ClearScope();
}

// instead of pushing constants on to the scope need to access main's struct
// data

void CodegenVisitor::Visit(const ProgramAst& node) {
  for (const auto& class_ast : node.GetClasses()) {
    classes_[&class_ast] =
        llvm::StructType::create(context_, class_ast.GetName());
  }
  classes_[node.GetIoClass()] =
      llvm::StructType::create(context_, node.GetIoClass()->GetName());
  classes_[node.GetObjectClass()] =
      llvm::StructType::create(context_, node.GetObjectClass()->GetName());

  SetLlvmFunction("Object", "abort", CreateAbortFunc());
  SetLlvmFunction("IO", "out_string", CreateIoOutStringFunc());
  SetLlvmFunction("IO", "out_int", CreateIoOutIntFunc());
  SetLlvmFunction("String", "concat", CreateStringConcatFunc());

  for (const auto& class_ast : node.GetClasses()) {
    std::vector<llvm::Type*> class_attributes;
    for (const auto* attr : class_ast.GetAttributeFeatures()) {
      llvm::Type* attr_type = GetLlvmBasicOrPointerToClassType(attr->GetType());
      class_attributes.push_back(attr_type);
    }
    classes_[&class_ast]->setBody(class_attributes);
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

  std::filesystem::path object_file_path = program_ast_->GetFilePath();
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

  std::cout << "ast_->GetFileName() " << program_ast_->GetFileName() << "\n";

  std::cout << "Wrote " << object_file_path.string() << "\n";
}

void Codegen::GenerateCode() const {
  CodegenVisitor codegen_visitor(*ast_);
  ast_->Accept(codegen_visitor);
}

void Codegen::Link() const {
  std::filesystem::path object_file_path = ast_->GetFilePath();
  object_file_path.replace_extension(".obj");

  std::string obj_input_linker_arg = object_file_path.string();
  obj_input_linker_arg += " ";

  std::filesystem::path exe_file_path = ast_->GetFilePath();
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
