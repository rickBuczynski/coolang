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
  AstToCodeMap(llvm::LLVMContext* context, llvm::Module* module,
               llvm::IRBuilder<>* builder, const ProgramAst* program_ast)
      : context_(context),
        module_(module),
        builder_(builder),
        program_ast_(program_ast) {}

  void Insert(const ClassAst* class_ast) {
    class_codegens_.insert(
        std::make_pair(class_ast, ClassCodegen(*context_, class_ast)));
  }

  void AddAttributes(const ClassAst* class_ast);
  void AddMethods(const ClassAst* class_ast);
  void AddConstructor(const ClassAst* class_ast);

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

  llvm::Function* GetConstructor(const ClassAst* class_ast) {
    return class_codegens_.at(class_ast).GetConstructor();
  }
  llvm::Function* GetConstructor(const std::string& type_name) {
    return GetConstructor(GetClassByName(type_name));
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

  const MethodFeature* GetCurrentMethod() const { return current_method_; }
  void SetCurrentMethod(const MethodFeature* current_method) {
    current_method_ = current_method;
  }

  const ProgramAst* GetProgramAst() const { return program_ast_; }

  llvm::Function* GetLlvmFunction(const std::string& class_name,
                                  const std::string& method_name) {
    return functions_.at(
        GetClassByName(class_name)->GetMethodFeatureByName(method_name));
  }

  llvm::Function* GetLlvmFunction(const MethodFeature* method_feature) {
    return functions_.at(method_feature);
  }

  void SetLlvmFunction(const std::string& class_name,
                       const std::string& method_name, llvm::Function* func) {
    functions_[GetClassByName(class_name)
                   ->GetMethodFeatureByName(method_name)] = func;
  }

  void SetLlvmFunction(MethodFeature* method_feature, llvm::Function* func) {
    functions_[method_feature] = func;
  }

  void EraseMethod(MethodFeature* method_feature) {
    functions_.erase(method_feature);
  }

  llvm::Function* CurLlvmFunc() { return functions_.at(current_method_); }

 private:
  std::unordered_map<const ClassAst*, ClassCodegen> class_codegens_;
  std::unordered_map<const MethodFeature*, llvm::Function*> functions_;

  llvm::LLVMContext* context_;
  llvm::Module* module_;
  llvm::IRBuilder<>* builder_;

  const ProgramAst* program_ast_;
  const ClassAst* current_class_ = nullptr;
  const MethodFeature* current_method_ = nullptr;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_AST_TO_CODE_MAP_H
