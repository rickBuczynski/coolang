#ifndef COOLANG_CODEGEN_IO_CODEGEN_H
#define COOLANG_CODEGEN_IO_CODEGEN_H

#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/ast_to_code_map.h"
#include "coolang/codegen/c_std.h"

namespace coolang {

class IoCodegen {
 public:
  IoCodegen(llvm::LLVMContext* context, llvm::IRBuilder<>* builder,
            AstToCodeMap* ast_to_code_map, CStd* c_std)
      : context_(context),
        builder_(builder),
        ast_to_code_map_(ast_to_code_map),
        c_std_(c_std) {}

  void GenAllFuncBodies() const {
    GenIoOutString();
    GenIoOutInt();
    GenIoInString();
    GenIoInInt();
  }

 private:
  void GenIoOutString() const {
    llvm::Function* func =
        ast_to_code_map_->LlvmFunc("IO", "out_string");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* format_str = builder_->CreateGlobalStringPtr("%s");
    auto arg_iterator = func->arg_begin();
    arg_iterator++;
    llvm::Value* args[] = {format_str, arg_iterator};
    builder_->CreateCall(c_std_->GetPrintfFunc(), args);
    builder_->CreateRet(func->arg_begin());
  }

  void GenIoOutInt() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("IO", "out_int");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* format_str = builder_->CreateGlobalStringPtr("%d");
    auto arg_iterator = func->arg_begin();
    arg_iterator++;
    llvm::Value* args[] = {format_str, arg_iterator};
    builder_->CreateCall(c_std_->GetPrintfFunc(), args);
    builder_->CreateRet(func->arg_begin());
  }

  void GenIoInString() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("IO", "in_string");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement in_string instead of just returning void
    builder_->CreateRetVoid();
  }

  void GenIoInInt() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("IO", "in_int");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement in_int instead of just returning void
    builder_->CreateRetVoid();
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
  CStd* c_std_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_IO_CODEGEN_H
