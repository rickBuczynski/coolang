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
    CreateStringConcatFunc();
    CreateStringSubstrFunc();
    CreateStringLengthFunc();
  }

 private:
  void CreateStringConcatFunc() const {
    llvm::Function* string_concat_func =
        ast_to_code_map_->LlvmFunc("String", "concat");

    llvm::BasicBlock* string_concat_entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", string_concat_func);
    builder_->SetInsertPoint(string_concat_entry);

    auto arg_iter = string_concat_func->arg_begin();
    llvm::Value* lhs_arg = arg_iter;
    arg_iter++;
    llvm::Value* rhs_arg = arg_iter;

    // Need GC roots to preserve params since this func allocates
    llvm::AllocaInst* lhs_root = AddRootForStrParam(lhs_arg);
    llvm::AllocaInst* rhs_root = AddRootForStrParam(rhs_arg);

    llvm::Value* lhs_len =
        builder_->CreateCall(c_std_->GetStrlenFunc(), {lhs_arg});
    llvm::Value* rhs_len =
        builder_->CreateCall(c_std_->GetStrlenFunc(), {rhs_arg});
    llvm::Value* const_one =
        llvm::ConstantInt::get(*context_, llvm::APInt(32, 1, true));
    llvm::Value* concated_len = builder_->CreateAdd(lhs_len, rhs_len);
    concated_len = builder_->CreateAdd(concated_len, const_one);

    llvm::Value* concated_val =
        builder_->CreateCall(c_std_->GetGcMallocStringFunc(), {concated_len});
    builder_->CreateCall(c_std_->GetStrcpyFunc(), {concated_val, lhs_arg});
    builder_->CreateCall(c_std_->GetStrcatFunc(), {concated_val, rhs_arg});

    builder_->CreateCall(c_std_->GetGcRemoveStringRootFunc(), {rhs_root});
    builder_->CreateCall(c_std_->GetGcRemoveStringRootFunc(), {lhs_root});

    builder_->CreateRet(concated_val);
  }

  void CreateStringSubstrFunc() const {
    llvm::Function* string_substr_func =
        ast_to_code_map_->LlvmFunc("String", "substr");

    llvm::BasicBlock* string_substr_entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", string_substr_func);
    builder_->SetInsertPoint(string_substr_entry);

    auto arg_iter = string_substr_func->arg_begin();
    llvm::Value* str_lhs = arg_iter;
    arg_iter++;
    llvm::Value* int_start_index = arg_iter;
    arg_iter++;
    llvm::Value* substr_len = arg_iter;

    // Need a GC root to preserve self param since this func allocates
    llvm::AllocaInst* root = AddRootForStrParam(str_lhs);

    llvm::Value* const_one =
        llvm::ConstantInt::get(*context_, llvm::APInt(32, 1, true));
    llvm::Value* malloc_len = builder_->CreateAdd(substr_len, const_one);

    llvm::Value* substr_val =
        builder_->CreateCall(c_std_->GetGcMallocStringFunc(), {malloc_len});
    llvm::Value* substr_start_ptr =
        builder_->CreateGEP(str_lhs, int_start_index);
    builder_->CreateCall(c_std_->GetStrncpyFunc(),
                         {substr_val, substr_start_ptr, substr_len});

    llvm::Value* substr_last_ptr = builder_->CreateGEP(substr_val, substr_len);
    builder_->CreateStore(
        llvm::ConstantInt::get(*context_, llvm::APInt(8, 0, true)),
        substr_last_ptr);

    builder_->CreateCall(c_std_->GetGcRemoveStringRootFunc(), {root});

    builder_->CreateRet(substr_val);
  }

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
