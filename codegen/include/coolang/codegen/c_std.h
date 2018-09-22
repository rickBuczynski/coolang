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
  llvm::Constant* GetMallocFunc() const { return malloc_func_; }

 private:
  llvm::Constant* CreateCStdFuncDecl(const std::string& func_name,
                                     const std::string& return_type,
                                     const std::vector<std::string>& arg_types,
                                     bool is_var_arg = false) {
    std::vector<llvm::Type*> llvm_arg_types;
    for (const auto& arg : arg_types) {
      llvm_arg_types.push_back(ast_to_code_map_->GetLlvmBasicType(arg));
    }

    llvm::FunctionType* func_type =
        llvm::FunctionType::get(ast_to_code_map_->GetLlvmBasicType(return_type),
                                llvm_arg_types, is_var_arg);

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
  // use String (becomes char*) as return type for malloc
  // since llvm has no void* type
  llvm::Constant* malloc_func_ =
      CreateCStdFuncDecl("malloc", "String", {"Int"});
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_C_STD_H
