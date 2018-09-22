#ifndef COOLANG_CODEGEN_IO_CODEGEN_H
#define COOLANG_CODEGEN_IO_CODEGEN_H

#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/ast_to_code_map.h"

namespace coolang {

class IoCodegen {
 public:
  IoCodegen(llvm::LLVMContext* context, llvm::IRBuilder<>* builder,
            AstToCodeMap* ast_to_code_map)
      : context_(context),
        builder_(builder),
        ast_to_code_map_(ast_to_code_map) {}

  void GenAllFuncBodies(llvm::Constant* printf_func) const {
    GenIoOutString(printf_func);
    GenIoOutInt(printf_func);
    GenIoInString();
    GenIoInInt();
  }

 private:
  void GenIoOutString(llvm::Constant* printf_func) const {
    llvm::Function* func =
        ast_to_code_map_->GetLlvmFunction("IO", "out_string");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* format_str = builder_->CreateGlobalStringPtr("%s");
    auto arg_iterator = func->arg_begin();
    arg_iterator++;
    llvm::Value* args[] = {format_str, arg_iterator};
    builder_->CreateCall(printf_func, args);
    builder_->CreateRet(func->arg_begin());
  }

  void GenIoOutInt(llvm::Constant* printf_func) const {
    llvm::Function* func = ast_to_code_map_->GetLlvmFunction("IO", "out_int");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* format_str = builder_->CreateGlobalStringPtr("%d");
    auto arg_iterator = func->arg_begin();
    arg_iterator++;
    llvm::Value* args[] = {format_str, arg_iterator};
    builder_->CreateCall(printf_func, args);
    builder_->CreateRet(func->arg_begin());
  }

  void GenIoInString() const {
    llvm::Function* func = ast_to_code_map_->GetLlvmFunction("IO", "in_string");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement in_string instead of just returning void
    builder_->CreateRetVoid();
  }

  void GenIoInInt() const {
    llvm::Function* func = ast_to_code_map_->GetLlvmFunction("IO", "in_int");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement in_int instead of just returning void
    builder_->CreateRetVoid();
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_IO_CODEGEN_H
