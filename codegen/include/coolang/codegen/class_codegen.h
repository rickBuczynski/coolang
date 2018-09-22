#ifndef COOLANG_CODEGEN_CLASS_CODEGEN_H
#define COOLANG_CODEGEN_CLASS_CODEGEN_H

#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/LLVMContext.h>
#include "coolang/codegen/vtable.h"
#include "coolang/parser/ast.h"

namespace coolang {

class ClassCodegen {
 public:
  ClassCodegen(llvm::LLVMContext& context, const ClassAst* class_ast)
      : struct_type_(llvm::StructType::create(context, class_ast->GetName())),
        vtable_(context, class_ast) {}

  llvm::StructType* GetStructType() const { return struct_type_; }
  const Vtable& GetVtable() const { return vtable_; }

  void BuildVtable(llvm::Module* module,
                   const std::vector<llvm::Constant*>& vtable_functions) {
    vtable_.BuildVtable(module, vtable_functions);
  }

  void SetAttributes(std::vector<llvm::Type*> class_attributes) {
    struct_type_->setBody(class_attributes);
  }

  llvm::Function* GetConstructor() const { return constructor_; }
  void SetConstructor(llvm::Function* constructor) {
    constructor_ = constructor;
  }

 private:
  llvm::StructType* struct_type_;
  Vtable vtable_;
  llvm::Function* constructor_ = nullptr;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CLASS_CODEGEN_H
