/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

  void GenAllFuncBodies() const { GenIoInInt(); }

 private:
  void GenIoInInt() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("IO", "in_int");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* str = builder_->CreateCall(
        ast_to_code_map_->LlvmFunc("IO", "in_string"), {func->arg_begin()});

    llvm::Value* str_as_int =
        builder_->CreateCall(c_std_->GetAtoiFunc(), {str});

    builder_->CreateRet(str_as_int);
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
  CStd* c_std_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_IO_CODEGEN_H
