#ifndef COOLANG_CODEGEN_AST_TO_CODE_MAP_H
#define COOLANG_CODEGEN_AST_TO_CODE_MAP_H

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include "coolang/codegen/class_codegen.h"
#include "coolang/codegen/vtable.h"
#include "coolang/parser/ast.h"

namespace coolang {

class AstToCodeMap {
 public:
  AstToCodeMap(llvm::LLVMContext* context, llvm::IRBuilder<>* builder,
               const ProgramAst* program_ast)
      : context_(context), builder_(builder), program_ast_(program_ast) {}

  void Insert(const ClassAst* class_ast) {
    class_codegens_.insert(
        std::make_pair(class_ast, ClassCodegen(*context_, class_ast)));
  }

   void AddAttributes(const ClassAst* class_ast) {
    std::vector<llvm::Type*> class_attributes;
    class_attributes.push_back(
        GetVtable(class_ast).GetStructType()->getPointerTo());

    for (const auto* attr : class_ast->GetAttributeFeatures()) {
      llvm::Type* attr_type = GetLlvmBasicOrPointerToClassType(attr->GetType());
      class_attributes.push_back(attr_type);
    }

    class_codegens_.at(class_ast).SetAttributes(class_attributes);
  }

  llvm::Type* GetLlvmBasicType(const std::string& class_name) const {
    if (class_name == "Int") {
      return builder_->getInt32Ty();
    }
    if (class_name == "String") {
      return builder_->getInt8PtrTy();
    }
    if (class_name == "Bool") {
      return builder_->getInt1Ty();
    }
    return nullptr;
  }

  llvm::Type* GetLlvmClassType(const std::string& class_name) {
    return GetLlvmClassType(GetClassByName(class_name));
  }

  llvm::Type* GetLlvmClassType(const ClassAst* class_ast) {
    return class_codegens_.at(class_ast).GetStructType();
  }

  const Vtable& GetVtable(const ClassAst* class_ast) {
    return class_codegens_.at(class_ast).GetVtable();
  }

  void BuildVtable(const ClassAst* class_ast, llvm::Module* module,
                   const std::vector<llvm::Constant*>& vtable_functions) {
    class_codegens_.at(class_ast).BuildVtable(module, vtable_functions);
  }


  llvm::Type* GetLlvmBasicOrPointerToClassType(const std::string& type_name) {
    llvm::Type* type = GetLlvmBasicType(type_name);
    if (type == nullptr) {
      type = GetLlvmClassType(type_name)->getPointerTo();
    }
    return type;
  }

  llvm::Value* GetLlvmBasicOrPointerDefaultVal(const std::string& type_name) {
    if (type_name == "Int") {
      return llvm::ConstantInt::get(*context_, llvm::APInt(32, 0, true));
    }
    if (type_name == "String") {
      return builder_->CreateGlobalStringPtr("");
    }
    if (type_name == "Bool") {
      return llvm::ConstantInt::get(*context_, llvm::APInt(1, 0, false));
    }
    return llvm::ConstantPointerNull::get(
        GetLlvmClassType(type_name)->getPointerTo());
  }

  const ClassAst* GetClassByName(std::string name) const {
    if (name == "SELF_TYPE") {
      name = current_class_->GetName();
    }
    return program_ast_->GetClassByName(name);
  }

  const ClassAst* GetCurrentClass() const { return current_class_; }
  void SetCurrentClass(const ClassAst* current_class) {
    current_class_ = current_class;
  }

  const ProgramAst* GetProgramAst() const { return program_ast_; }

 private:
  std::unordered_map<const ClassAst*, ClassCodegen> class_codegens_;

  llvm::LLVMContext* context_;
  llvm::IRBuilder<>* builder_;

  const ProgramAst* program_ast_;
  const ClassAst* current_class_ = nullptr;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_AST_TO_CODE_MAP_H
