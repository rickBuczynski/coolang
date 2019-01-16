#ifndef COOLANG_CODEGEN_OBJECT_CODEGEN_H
#define COOLANG_CODEGEN_OBJECT_CODEGEN_H

#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/ast_to_code_map.h"

namespace coolang {

class ObjectCodegen {
 public:
  ObjectCodegen(llvm::LLVMContext* context, llvm::IRBuilder<>* builder,
                AstToCodeMap* ast_to_code_map, CStd* std)
      : context_(context),
        builder_(builder),
        ast_to_code_map_(ast_to_code_map),
        c_std_(std) {}

  void GenAllFuncBodies() const {
    GenAbort("Object");
    GenAbort("String");
    GenAbort("Bool");
    GenAbort("Int");

    GenTypeName();
    GenBoolTypeName();
    GenStringTypeName();
    GenIntTypeName();

    GenCopy();
    GenBoolCopy();
    GenStringCopy();
    GenIntCopy();
  }

  void GenExitWithMessage(const std::string& format_string,
                          std::vector<llvm::Value*> printf_args) const {
    printf_args.insert(printf_args.begin(),
                       builder_->CreateGlobalStringPtr(format_string));
    builder_->CreateCall(c_std_->GetPrintfFunc(), printf_args);
    builder_->CreateCall(c_std_->GetExitFunc(),
                         {ast_to_code_map_->LlvmConstInt32(0)});
  }

 private:
  void GenAbort(const std::string& class_name) const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc(class_name, "abort");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* type_name = builder_->CreateCall(
        ast_to_code_map_->LlvmFunc(class_name, "type_name"),
        {func->arg_begin()});

    GenExitWithMessage("Abort called from class %s\n", {type_name});

    // return value can never be used but needs to match COOL spec
    builder_->CreateRet(func->arg_begin());
  }

  void GenCopy() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Object", "copy");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* typesize = builder_->CreateLoad(builder_->CreateStructGEP(
        ast_to_code_map_->LlvmClass("Object"), func->arg_begin(),
        AstToCodeMap::obj_typesize_index));

    llvm::Value* obj_typename = builder_->CreateLoad(builder_->CreateStructGEP(
        ast_to_code_map_->LlvmClass("Object"), func->arg_begin(),
        AstToCodeMap::obj_typename_index));

    llvm::Value* root = builder_->CreateAlloca(
        ast_to_code_map_->LlvmBasicOrClassPtrTy("Object"));
    builder_->CreateStore(func->arg_begin(), root);
    builder_->CreateCall(c_std_->GetGcAddRootFunc(), {root});

    llvm::Value* copy = builder_->CreateCall(c_std_->GetGcMallocFunc(),
                                             {typesize, obj_typename});

    llvm::Value* copy_dst = builder_->CreateBitCast(
        copy, ast_to_code_map_->LlvmClass("Object")->getPointerTo());

    llvm::Value* copy_constructor_func = builder_->CreateLoad(
        builder_->CreateStructGEP(nullptr, func->arg_begin(),
                                  AstToCodeMap::obj_copy_constructor_index));
    builder_->CreateCall(copy_constructor_func, {copy_dst, func->arg_begin()});

    builder_->CreateCall(c_std_->GetGcRemoveRootFunc(), {root});

    builder_->CreateRet(copy);
  }

  void GenTypeName() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Object", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    llvm::Value* typename_gep = builder_->CreateStructGEP(
        ast_to_code_map_->LlvmClass("Object"), func->arg_begin(),
        AstToCodeMap::obj_typename_index);
    llvm::Value* typename_val = builder_->CreateLoad(typename_gep);

    llvm::Value* malloc_len = builder_->CreateAdd(
        builder_->CreateCall(c_std_->GetStrlenFunc(), {typename_val}),
        ast_to_code_map_->LlvmConstInt32(1));
    llvm::Value* malloc_val =
        builder_->CreateCall(c_std_->GetGcMallocStringFunc(), {malloc_len});

    builder_->CreateCall(c_std_->GetStrcpyFunc(), {malloc_val, typename_val});
    builder_->CreateRet(malloc_val);
  }

  llvm::Value* MallocStrConst(std::string str) const {
    llvm::Value* malloc_val = builder_->CreateCall(
        c_std_->GetGcMallocStringFunc(),
        {ast_to_code_map_->LlvmConstInt32(str.length() + 1)});
    llvm::Value* str_global = builder_->CreateGlobalStringPtr(str);
    builder_->CreateCall(c_std_->GetStrcpyFunc(), {malloc_val, str_global});
    return malloc_val;
  }

  void GenBoolTypeName() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Bool", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);
    builder_->CreateRet(MallocStrConst("Bool"));
  }

  void GenStringTypeName() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("String", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    builder_->CreateRet(MallocStrConst("String"));
  }

  void GenIntTypeName() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Int", "type_name");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    builder_->CreateRet(MallocStrConst("Int"));
  }

  void GenBoolCopy() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Bool", "copy");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    builder_->CreateRet(func->arg_begin());
  }

  void GenStringCopy() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("String", "copy");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);
    builder_->CreateRet(func->arg_begin());
  }

  void GenIntCopy() const {
    llvm::Function* func = ast_to_code_map_->LlvmFunc("Int", "copy");

    llvm::BasicBlock* entry =
        llvm::BasicBlock::Create(*context_, "entrypoint", func);
    builder_->SetInsertPoint(entry);

    builder_->CreateRet(func->arg_begin());
  }

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;
  AstToCodeMap* ast_to_code_map_;
  CStd* c_std_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_OBJECT_CODEGEN_H
