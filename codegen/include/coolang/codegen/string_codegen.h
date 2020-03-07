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

#ifndef COOLANG_CODEGEN_STRING_CODEGEN_H
#define COOLANG_CODEGEN_STRING_CODEGEN_H

#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/ast_to_code_map.h"
#include "coolang/codegen/c_std.h"

namespace coolang {

class StringCodegen {
 public:
  StringCodegen(llvm::LLVMContext* context, llvm::IRBuilder<>* builder,
                AstToCodeMap* ast_to_code_map, CStd* c_std)
      : context_(context),
        builder_(builder),
        ast_to_code_map_(ast_to_code_map),
        c_std_(c_std) {}

  void GenAllFuncBodies() const {
    CreateStringLengthFunc();
  }

 private:
  void CreateStringLengthFunc() const {
    llvm::Function* string_length_func =
        ast_to_code_map_->LlvmFunc("String", "length");

    llvm::BasicBlock* string_length_entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", string_length_func);
    builder_->SetInsertPoint(string_length_entry);

    llvm::Value* len_val = builder_->CreateCall(
        c_std_->GetStrlenFunc(), {string_length_func->arg_begin()});
    builder_->CreateRet(len_val);
  }

  llvm::AllocaInst* AddRootForStrParam(llvm::Value* str_param) const {
    llvm::AllocaInst* str_alloca =
        builder_->CreateAlloca(ast_to_code_map_->LlvmBasicType("String"));
    builder_->CreateStore(str_param, str_alloca);
    builder_->CreateCall(c_std_->GetGcAddStringRootFunc(), {str_alloca});
    return str_alloca;
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
  CStd* c_std_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_STRING_CODEGEN_H
