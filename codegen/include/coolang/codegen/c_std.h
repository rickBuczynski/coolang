#ifndef COOLANG_CODEGEN_C_STD_H
#define COOLANG_CODEGEN_C_STD_H

#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/ast_to_code_map.h"

namespace coolang {

class CStd {
 public:
  CStd(llvm::Module* module, AstToCodeMap* ast_to_code_map)
      : module_(module), ast_to_code_map_(ast_to_code_map) {}

  llvm::Constant* GetPrintfFunc() const { return printf_func_; }
  llvm::Constant* GetStrlenFunc() const { return strlen_func_; }
  llvm::Constant* GetStrcpyFunc() const { return strcpy_func_; }
  llvm::Constant* GetStrncpyFunc() const { return strncpy_func_; }
  llvm::Constant* GetStrcatFunc() const { return strcat_func_; }
  llvm::Constant* GetExitFunc() const { return exit_func_; }
  llvm::Constant* GetStrCmpFunc() const { return strcmp_func_; }
  llvm::Constant* GetGetcharFunc() const { return getchar_func_; }
  llvm::Constant* GetAtoiFunc() const { return atoi_func_; }

  llvm::Constant* GetGcMallocFunc() const { return gc_malloc_func_; }
  llvm::Constant* GetGcMallocAndCopyFunc() const {
    return gc_malloc_and_copy_func_;
  }
  llvm::Constant* GetGcMallocStringFunc() const {
    return gc_malloc_string_func_;
  }
  llvm::Constant* GetGcAddRootFunc() const { return gc_add_root_func_; }
  llvm::Constant* GetGcAddStringRootFunc() const {
    return gc_add_string_root_func_;
  }
  llvm::Constant* GetGcRemoveRootFunc() const { return gc_remove_root_func_; }
  llvm::Constant* GetGcRemoveStringRootFunc() const {
    return gc_remove_string_root_func_;
  }
  llvm::Constant* GetGcSystemInitFunc() const { return gc_system_init_func_; }
  llvm::Constant* GetGcSystemDestroyFunc() const {
    return gc_system_destroy_func_;
  }

 private:
  llvm::Constant* CreateCStdFuncDecl(const std::string& func_name,
                                     const std::string& return_type_str,
                                     const std::vector<std::string>& arg_types,
                                     bool is_var_arg = false) const {
    std::vector<llvm::Type*> llvm_arg_types;
    for (const auto& arg : arg_types) {
      llvm_arg_types.push_back(ast_to_code_map_->LlvmBasicType(arg));
    }

    llvm::Type* return_type;
    if (return_type_str == "Void") {
      return_type = ast_to_code_map_->LlvmVoidType();
    } else {
      return_type = ast_to_code_map_->LlvmBasicType(return_type_str);
    }

    return CreateCStdFuncDecl(func_name, return_type, llvm_arg_types,
                              is_var_arg);
  }

  llvm::Constant* CreateCStdFuncDecl(
      const std::string& func_name, llvm::Type* return_type,
      const std::vector<llvm::Type*>& llvm_arg_types,
      bool is_var_arg = false) const {
    llvm::FunctionType* func_type =
        llvm::FunctionType::get(return_type, llvm_arg_types, is_var_arg);
    return module_->getOrInsertFunction(func_name, func_type);
  }

  llvm::Module* module_;
  AstToCodeMap* ast_to_code_map_;

  llvm::Constant* printf_func_ =
      CreateCStdFuncDecl("printf", "Int", {"String"}, true);
  llvm::Constant* strlen_func_ =
      CreateCStdFuncDecl("strlen", "Int", {"String"});
  llvm::Constant* strcpy_func_ =
      CreateCStdFuncDecl("strcpy", "String", {"String", "String"});
  llvm::Constant* strncpy_func_ =
      CreateCStdFuncDecl("strncpy", "String", {"String", "String", "Int"});
  llvm::Constant* strcat_func_ =
      CreateCStdFuncDecl("strcat", "String", {"String", "String"});
  llvm::Constant* strcmp_func_ =
      CreateCStdFuncDecl("strcmp", "Int", {"String", "String"});
  llvm::Constant* atoi_func_ = CreateCStdFuncDecl("atoi", "Int", {"String"});
  llvm::Constant* getchar_func_ = CreateCStdFuncDecl("getchar", "Int", {});
  llvm::Constant* exit_func_ = CreateCStdFuncDecl("exit", "Void", {"Int"});

  llvm::Constant* gc_malloc_func_ =
      CreateCStdFuncDecl("gc_malloc", "String", {"Int"});

  llvm::Constant* gc_malloc_and_copy_func_ = CreateCStdFuncDecl(
      "gc_malloc_and_copy", ast_to_code_map_->LlvmBasicType("String"),
      {ast_to_code_map_->LlvmBasicType("Int"),
       ast_to_code_map_->LlvmClass("Object")->getPointerTo()},
      false);

  llvm::Constant* gc_malloc_string_func_ =
      CreateCStdFuncDecl("gc_malloc_string", "String", {"Int"});

  llvm::Constant* gc_add_root_func_ = CreateCStdFuncDecl(
      "gc_add_root", ast_to_code_map_->LlvmVoidType(),
      {ast_to_code_map_->LlvmClass("Object")->getPointerTo()->getPointerTo()},
      false);
  llvm::Constant* gc_add_string_root_func_ = CreateCStdFuncDecl(
      "gc_add_string_root", ast_to_code_map_->LlvmVoidType(),
      {ast_to_code_map_->LlvmBasicType("String")->getPointerTo()}, false);

  llvm::Constant* gc_remove_root_func_ = CreateCStdFuncDecl(
      "gc_remove_root", ast_to_code_map_->LlvmVoidType(),
      {ast_to_code_map_->LlvmClass("Object")->getPointerTo()->getPointerTo()},
      false);
  llvm::Constant* gc_remove_string_root_func_ = CreateCStdFuncDecl(
      "gc_remove_string_root", ast_to_code_map_->LlvmVoidType(),
      {ast_to_code_map_->LlvmBasicType("String")->getPointerTo()}, false);

  llvm::Constant* gc_system_init_func_ =
      CreateCStdFuncDecl("gc_system_init", "Void", {"Int"});
  llvm::Constant* gc_system_destroy_func_ =
      CreateCStdFuncDecl("gc_system_destroy", "Void", {});
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_C_STD_H
