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
        ast_to_code_map_->GetLlvmFunction("String", "concat");

    llvm::BasicBlock* string_concat_entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", string_concat_func);
    builder_->SetInsertPoint(string_concat_entry);

    auto arg_iter = string_concat_func->arg_begin();
    llvm::Value* lhs_arg = arg_iter;
    arg_iter++;
    llvm::Value* rhs_arg = arg_iter;

    llvm::Value* lhs_len =
        builder_->CreateCall(c_std_->GetStrlenFunc(), {lhs_arg});
    llvm::Value* rhs_len =
        builder_->CreateCall(c_std_->GetStrlenFunc(), {rhs_arg});
    llvm::Value* const_one =
        llvm::ConstantInt::get(*context_, llvm::APInt(32, 1, true));
    llvm::Value* concated_len = builder_->CreateAdd(lhs_len, rhs_len);
    concated_len = builder_->CreateAdd(concated_len, const_one);

    // TODO this malloc leaks memory
    llvm::Value* concated_val =
        builder_->CreateCall(c_std_->GetMallocFunc(), {concated_len});
    builder_->CreateCall(c_std_->GetStrcpyFunc(), {concated_val, lhs_arg});
    builder_->CreateCall(c_std_->GetStrcatFunc(), {concated_val, rhs_arg});

    builder_->CreateRet(concated_val);
  }

  void CreateStringSubstrFunc() const {
    llvm::Function* string_substr_func =
        ast_to_code_map_->GetLlvmFunction("String", "substr");

    llvm::BasicBlock* string_substr_entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", string_substr_func);
    builder_->SetInsertPoint(string_substr_entry);

    auto arg_iter = string_substr_func->arg_begin();
    llvm::Value* str_lhs = arg_iter;
    arg_iter++;
    llvm::Value* int_start_index = arg_iter;
    arg_iter++;
    llvm::Value* substr_len = arg_iter;

    llvm::Value* const_one =
        llvm::ConstantInt::get(*context_, llvm::APInt(32, 1, true));
    llvm::Value* malloc_len = builder_->CreateAdd(substr_len, const_one);

    // TODO this malloc leaks memory
    llvm::Value* substr_val =
        builder_->CreateCall(c_std_->GetMallocFunc(), {malloc_len});
    llvm::Value* substr_start_ptr =
        builder_->CreateGEP(str_lhs, int_start_index);
    builder_->CreateCall(c_std_->GetStrncpyFunc(),
                         {substr_val, substr_start_ptr, substr_len});

    llvm::Value* substr_last_ptr = builder_->CreateGEP(substr_val, substr_len);
    builder_->CreateStore(
        llvm::ConstantInt::get(*context_, llvm::APInt(8, 0, true)),
        substr_last_ptr);

    builder_->CreateRet(substr_val);
  }

  void CreateStringLengthFunc() const {
    llvm::Function* string_length_func =
        ast_to_code_map_->GetLlvmFunction("String", "length");

    llvm::BasicBlock* string_length_entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", string_length_func);
    builder_->SetInsertPoint(string_length_entry);

    llvm::Value* len_val = builder_->CreateCall(
        c_std_->GetStrlenFunc(), {string_length_func->arg_begin()});
    builder_->CreateRet(len_val);
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
  CStd* c_std_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_STRING_CODEGEN_H
