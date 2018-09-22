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
    llvm::Function* func =
        ast_to_code_map_->LlvmFunc("Object", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    // TODO actually implement type_name instead of just returning void
    builder_->CreateRetVoid();
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_OBJECT_CODEGEN_H
