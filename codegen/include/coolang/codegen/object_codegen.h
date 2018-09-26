#ifndef COOLANG_CODEGEN_OBJECT_CODEGEN_H
#define COOLANG_CODEGEN_OBJECT_CODEGEN_H

#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/ast_to_code_map.h"

namespace coolang {

class ObjectCodegen {
 public:
  ObjectCodegen(llvm::LLVMContext* context, llvm::IRBuilder<>* builder,
                AstToCodeMap* ast_to_code_map)
      : context_(context),
        builder_(builder),
        ast_to_code_map_(ast_to_code_map) {}

  void GenAllFuncBodies() const {
    GenAbort();
    GenCopy();
    GenTypeName();

    GenBoolTypeName();
  }

 private:
  void GenAbort() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Object", "abort");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement abort instead of just returning void
    builder_->CreateRetVoid();
  }

  void GenCopy() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Object", "copy");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement copy instead of just returning void
    builder_->CreateRetVoid();
  }

  void GenTypeName() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Object", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    const int typename_index = 1;
    llvm::Value* typename_ptr =
        builder_->CreateStructGEP(ast_to_code_map_->LlvmClass("Object"),
                                  func->arg_begin(), typename_index);
    builder_->CreateRet(builder_->CreateLoad(typename_ptr));
  }

  void GenBoolTypeName() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Bool", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    builder_->CreateRet(builder_->CreateGlobalStringPtr("Bool"));
  }

  // TODO GenIntTypeName GenStringTypeName

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_OBJECT_CODEGEN_H
