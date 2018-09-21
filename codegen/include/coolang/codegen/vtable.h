#ifndef COOLANG_CODEGEN_VTABLE_H
#define COOLANG_CODEGEN_VTABLE_H

#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/LLVMContext.h>
#include "coolang/parser/ast.h"

namespace coolang {

class Vtable {
 public:
  Vtable(llvm::LLVMContext& context, const ClassAst* class_ast)
      : class_ast_(class_ast),
        struct_type_(llvm::StructType::create(
            context, class_ast->GetName() + "-vtable")) {}

  void BuildVtable(llvm::Module* module,
                   const std::vector<llvm::Constant*>& vtable_functions);

  int GetIndexOfMethodFeature(const MethodFeature* method_feature) const;

  llvm::StructType* GetStructType() const { return struct_type_; }
  llvm::GlobalValue* GetGlobalInstance() const { return global_instance_; }
  const std::vector<llvm::Constant*>& GetFunctions() const {
    return functions_;
  }

 private:
  const ClassAst* class_ast_;
  llvm::StructType* struct_type_;
  llvm::GlobalValue* global_instance_ = nullptr;
  std::vector<llvm::Constant*> functions_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_VTABLE_H
