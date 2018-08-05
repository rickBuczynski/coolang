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
        module_(new llvm::Module("asdf", context_)),
        builder_(context_) {}

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
  void Visit(const ClassAst& node) override {}
  void Visit(const CaseBranch& node) override {}
  void Visit(const MethodFeature& node) override {}
  void Visit(const AttributeFeature& node) override {}

  void Visit(const ProgramAst& node) override;

 private:
  const ProgramAst* program_ast_;

  llvm::LLVMContext context_;
  std::unique_ptr<llvm::Module> module_;
  llvm::IRBuilder<> builder_;
};

void CodegenVisitor::Visit(const ProgramAst& node) {
  llvm::FunctionType* func_type =
      llvm::FunctionType::get(builder_.getVoidTy(), false);
  llvm::Function* main_func = llvm::Function::Create(
      func_type, llvm::Function::ExternalLinkage, "main", module_.get());
  llvm::BasicBlock* entry =
      llvm::BasicBlock::Create(context_, "entrypoint", main_func);
  builder_.SetInsertPoint(entry);

  llvm::Value* helloWorld = builder_.CreateGlobalStringPtr("hello world!\n");

  std::vector<llvm::Type*> puts_args;
  puts_args.push_back(builder_.getInt8Ty()->getPointerTo());
  const llvm::ArrayRef<llvm::Type*> args_ref(puts_args);

  llvm::FunctionType* putsType =
      llvm::FunctionType::get(builder_.getInt32Ty(), args_ref, false);
  llvm::Constant* putsFunc = module_->getOrInsertFunction("puts", putsType);

  builder_.CreateCall(putsFunc, helloWorld);
  builder_.CreateRetVoid();

  module_->print(llvm::errs(), nullptr);

  // Initialize the target registry etc.
  LLVMInitializeX86TargetInfo();
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetMC();
  LLVMInitializeX86AsmParser();
  LLVMInitializeX86AsmPrinter();

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  module_->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    llvm::errs() << Error;
    return;
  }

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Optional<llvm::Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  module_->setDataLayout(TheTargetMachine->createDataLayout());

  std::error_code EC;

  std::filesystem::path object_file_path = program_ast_->GetFilePath();
  object_file_path.replace_extension(".obj");

  llvm::raw_fd_ostream dest(object_file_path.string(), EC,
                            llvm::sys::fs::F_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return;
  }

  llvm::legacy::PassManager pass;
  auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
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
  std::filesystem::path exe_file_path = ast_->GetFilePath();
  exe_file_path.replace_extension(".exe");

  std::string output_exe_linker_arg = "-OUT:";
  output_exe_linker_arg += exe_file_path.string();
  output_exe_linker_arg += " ";

  // clang-format off
  std::string linker_cmd = "cmd /C \"";
  linker_cmd += "\"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/bin/Hostx86/x86/link.exe\" ";
  linker_cmd += "C:/Users/RickB/cpp/coolang/build/codegen/out.obj ";
  linker_cmd += output_exe_linker_arg;
  linker_cmd += "libcmt.lib ";
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
