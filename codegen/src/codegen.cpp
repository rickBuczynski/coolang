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
    io_out_string_func_ = CreateIoOutStringFunc();
    io_out_int_func_ = CreateIoOutIntFunc();
  }

  void Visit(const CaseExpr& node) override {}
  void Visit(const StrExpr& node) override {}
  void Visit(const WhileExpr& node) override {}
  void Visit(const LetExpr& node) override {}
  void Visit(const IntExpr& node) override {}
  void Visit(const IsVoidExpr& node) override {}
  void Visit(const MethodCallExpr& node) override {}
  void Visit(const NotExpr& node) override {}
  void Visit(const IfExpr& node) override {}
  void Visit(const NegExpr& node) override {}
  void Visit(const BlockExpr& node) override {}
  void Visit(const ObjectExpr& node) override {}
  void Visit(const BinOpExpr& node) override {}
  void Visit(const MultiplyExpr& node) override {}
  void Visit(const LessThanEqualCompareExpr& node) override {}
  void Visit(const SubtractExpr& node) override {}
  void Visit(const AddExpr& node) override {}
  void Visit(const EqCompareExpr& node) override {}
  void Visit(const DivideExpr& node) override {}
  void Visit(const LessThanCompareExpr& node) override {}
  void Visit(const NewExpr& node) override {}
  void Visit(const AssignExpr& node) override {}
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

  llvm::Function* CreateIoOutStringFunc() {
    std::vector<llvm::Type*> io_out_string_args;
    io_out_string_args.push_back(builder_.getInt8Ty()->getPointerTo());

    // TODO return type should be SELF_TYPE I guess just a pointer?
    llvm::FunctionType* io_out_string_type = llvm::FunctionType::get(
        builder_.getVoidTy(), io_out_string_args, false);

    llvm::Function* io_out_string_func = llvm::Function::Create(
        io_out_string_type, llvm::Function::ExternalLinkage, "IO-out_string",
        module_.get());

    llvm::BasicBlock* io_out_string_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", io_out_string_func);
    builder_.SetInsertPoint(io_out_string_entry);

    llvm::Value* format_str = builder_.CreateGlobalStringPtr("%s");
    llvm::Value* args[] = {format_str, io_out_string_func->args().begin()};
    builder_.CreateCall(printf_func_, args);
    builder_.CreateRetVoid();

    return io_out_string_func;
  }
  llvm::Function* io_out_string_func_;

  llvm::Function* CreateIoOutIntFunc() {
    std::vector<llvm::Type*> io_out_int_args;
    io_out_int_args.push_back(builder_.getInt32Ty());

    // TODO return type should be SELF_TYPE I guess just a pointer?
    llvm::FunctionType* io_out_int_type =
        llvm::FunctionType::get(builder_.getVoidTy(), io_out_int_args, false);

    llvm::Function* io_out_int_func =
        llvm::Function::Create(io_out_int_type, llvm::Function::ExternalLinkage,
                               "IO-out_int", module_.get());

    llvm::BasicBlock* io_out_int_entry =
        llvm::BasicBlock::Create(context_, "entrypoint", io_out_int_func);
    builder_.SetInsertPoint(io_out_int_entry);
    llvm::Value* format_str = builder_.CreateGlobalStringPtr("%d");
    llvm::Value* args[] = {format_str, io_out_int_func->args().begin()};
    builder_.CreateCall(printf_func_, args);
    builder_.CreateRetVoid();

    return io_out_int_func;
  }
  llvm::Function* io_out_int_func_;

  void ClearScope() { in_scope_vars_.clear(); }

  void RemoveFromScope(const std::string& id) {
    in_scope_vars_[id].pop();
    if (in_scope_vars_[id].empty()) {
      in_scope_vars_.erase(id);
    }
  }

  void AddToScope(const std::string& id, llvm::Value* val) {
    in_scope_vars_[id].push(val);
  }

  std::unordered_map<std::string, std::stack<llvm::Value*>> in_scope_vars_;

  const ProgramAst* program_ast_;

  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  llvm::IRBuilder<> builder_;
};

void CodegenVisitor::Visit(const ClassAst& node) {
  for (const auto* attr : node.GetAttributeFeatures()) {
    if (attr->GetType() == "Int") {
      llvm::Value* val =
          llvm::ConstantInt::get(context_, llvm::APInt(32, 0, true));
      AddToScope(attr->GetId(), val);
    } else if (attr->GetType() == "String") {
      // TODO change hello to empty string
      llvm::Value* val = builder_.CreateGlobalStringPtr("hello");
      AddToScope(attr->GetId(), val);
    }
  }

  for (const auto* method : node.GetMethodFeatures()) {
    llvm::FunctionType* func_type =
        llvm::FunctionType::get(builder_.getVoidTy(), false);  // TODO
    llvm::Function* func =
        llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                               method->GetId(), module_.get());
    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(context_, "entrypoint", func);

    builder_.SetInsertPoint(entry);
    builder_.CreateCall(io_out_string_func_, in_scope_vars_["s"].top());
    builder_.CreateCall(io_out_int_func_, in_scope_vars_["i"].top());
    builder_.CreateRetVoid();
  }

  ClearScope();
}

void CodegenVisitor::Visit(const ProgramAst& node) {
  for (const auto& class_ast : node.GetClasses()) {
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
