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
    llvm::Function* func = ast_to_code_map_->LlvmFunc("IO", "out_string");

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

    // TODO this won't work for lines longer than 1024 chars
    // TODO this is wastefull for lines less than 1024 chars
    // TODO Add a GC verbose test for (new IO).in_string() should fail because
    // there is no root for (new IO). Fix by adding a root in this hardcoded
    // method like we do in the general case of codegening a user method.
    llvm::Value* in_str =
        builder_->CreateCall(c_std_->GetGcMallocStringFunc(),
                             {ast_to_code_map_->LlvmConstInt32(1024)});

    llvm::AllocaInst* in_char = builder_->CreateAlloca(builder_->getInt8Ty());
    builder_->CreateStore(
        builder_->CreateIntCast(
            builder_->CreateCall(c_std_->GetGetcharFunc(), {}),
            builder_->getInt8Ty(), true),
        in_char);

    llvm::AllocaInst* index = builder_->CreateAlloca(builder_->getInt32Ty());
    builder_->CreateStore(ast_to_code_map_->LlvmConstInt32(0), index);

    llvm::BasicBlock* loop_cond_bb =
        llvm::BasicBlock::Create(*context_, "loop-cond", func);

    llvm::BasicBlock* loop_body_bb =
        llvm::BasicBlock::Create(*context_, "loop-body", func);

    llvm::BasicBlock* loop_done_bb =
        llvm::BasicBlock::Create(*context_, "loop-done", func);

    builder_->CreateBr(loop_cond_bb);

    builder_->SetInsertPoint(loop_cond_bb);

    llvm::Value* is_line_end = builder_->CreateICmpEQ(
        builder_->CreateLoad(in_char), ast_to_code_map_->LlvmConstInt8('\n'));
    llvm::Value* is_eof = builder_->CreateICmpEQ(
        builder_->CreateLoad(in_char), ast_to_code_map_->LlvmConstInt8(EOF));

    builder_->CreateCondBr(builder_->CreateOr(is_line_end, is_eof),
                           loop_done_bb, loop_body_bb);

    builder_->SetInsertPoint(loop_body_bb);

    llvm::Value* cur_index = builder_->CreateLoad(index);

    builder_->CreateStore(builder_->CreateLoad(in_char),
                          builder_->CreateGEP(in_str, cur_index));

    llvm::Value* next_index =
        builder_->CreateAdd(cur_index, ast_to_code_map_->LlvmConstInt32(1));
    builder_->CreateStore(next_index, index);

    builder_->CreateStore(
        builder_->CreateIntCast(
            builder_->CreateCall(c_std_->GetGetcharFunc(), {}),
            builder_->getInt8Ty(), true),
        in_char);

    builder_->CreateBr(loop_cond_bb);

    builder_->SetInsertPoint(loop_done_bb);

    builder_->CreateStore(
        ast_to_code_map_->LlvmConstInt8(0),  // null terminator
        builder_->CreateGEP(in_str, builder_->CreateLoad(index)));

    builder_->CreateRet(in_str);
  }

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
