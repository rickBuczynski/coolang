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

#ifndef COOLANG_CODEGEN_AST_TO_CODE_MAP_H
#define COOLANG_CODEGEN_AST_TO_CODE_MAP_H

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

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
        program_ast_(program_ast) {
    for (const auto& class_ast : program_ast->GetClasses()) {
      Insert(&class_ast);
    }
    Insert(program_ast->GetIoClass());
    Insert(program_ast->GetObjectClass());

    gc_ptrs_info_ty_ = llvm::StructType::create(*context_, "GcPtrsInfo");
    gc_ptrs_info_ty_->setBody(
        {builder_->getInt32Ty(),
         LlvmClass("Object")->getPointerTo()->getPointerTo(),
         builder_->getInt32Ty(), LlvmBasicType("String")->getPointerTo(),
         gc_ptrs_info_ty_->getPointerTo()});
  }
  static constexpr int gc_ptrs_count_index = 0;
  static constexpr int gc_ptrs_array_index = 1;
  static constexpr int gc_str_count_index = 2;
  static constexpr int gc_strs_array_index = 3;
  static constexpr int gc_ptrs_next_index = 4;

  static constexpr int obj_gc_next_obj_index = 0;
  static constexpr int obj_gc_prev_obj_index = 1;
  static constexpr int obj_gc_is_reachable_index = 2;

  static constexpr int obj_typename_index = 3;

  static constexpr int obj_vtable_index = 4;
  static constexpr int obj_typesize_index = 5;
  // need a pointer to constructor to handle "new SELF_TYPE"
  static constexpr int obj_constructor_index = 6;
  // need copy_constructor for copy method
  static constexpr int obj_copy_constructor_index = 7;
  static constexpr int obj_boxed_data_index = 8;

  // attributes start after the things above
  static constexpr int obj_attributes_offset = 9;

  void AddAttributes(const ClassAst* class_ast);
  void AddMethods(const ClassAst* class_ast);
  void AddConstructor(const ClassAst* class_ast);

  static bool TypeUsesVtable(const std::string& class_name) {
    // Int Bool and String are implemented as unboxed values so there's nowhere
    // to store a vtable pointer. And they can't be inherited from so there's no
    // need for a vtable anyway
    return !(class_name == "Int" || class_name == "String" ||
             class_name == "Bool");
  }

  llvm::Type* LlvmVoidType() const { return builder_->getVoidTy(); }

  llvm::Type* LlvmBasicType(std::string class_name) const {
    if (class_name == "SELF_TYPE") {
      class_name = current_class_->GetName();
    }

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

  llvm::StructType* LlvmClass(const std::string& class_name) {
    return LlvmClass(GetClassByName(class_name));
  }

  llvm::StructType* LlvmClass(const ClassAst* class_ast) {
    return types_.at(class_ast);
  }

  const Vtable& GetVtable(const ClassAst* class_ast) {
    return vtables_.at(class_ast);
  }

  const Vtable& GetVtable(const std::string& class_name) {
    return vtables_.at(GetClassByName(class_name));
  }

  llvm::Function* GetConstructor(const ClassAst* class_ast) {
    return constructors_.at(class_ast);
  }
  llvm::Function* GetConstructor(const std::string& type_name) {
    return GetConstructor(GetClassByName(type_name));
  }

  llvm::Function* GetCopyConstructor(const ClassAst* class_ast) {
    return copy_constructors_.at(class_ast);
  }
  llvm::Function* GetCopyConstructor(const std::string& type_name) {
    return GetConstructor(GetClassByName(type_name));
  }

  llvm::StructType* GcPtrsInfoTy() const { return gc_ptrs_info_ty_; }

  llvm::Type* LlvmBasicOrClassPtrTy(const std::string& type_name) {
    llvm::Type* type = LlvmBasicType(type_name);
    if (type == nullptr) {
      type = LlvmClass(type_name)->getPointerTo();
    }
    return type;
  }

  llvm::ConstantInt* LlvmConstInt32(int32_t i) const {
    return llvm::ConstantInt::get(*context_, llvm::APInt(32, i, true));
  }

  llvm::ConstantInt* LlvmConstInt8(int8_t i) const {
    return llvm::ConstantInt::get(*context_, llvm::APInt(8, i, true));
  }

  const ClassAst* GetClassByName(std::string name) const {
    if (name == "SELF_TYPE") {
      name = current_class_->GetName();
    }
    return program_ast_->GetClassByName(name);
  }

  const ClassAst* CurClass() const { return current_class_; }
  void SetCurClass(const ClassAst* current_class) {
    current_class_ = current_class;
  }

  const MethodFeature* GetCurrentMethod() const { return current_method_; }
  void SetCurrentMethod(const MethodFeature* current_method) {
    current_method_ = current_method;
  }

  const ProgramAst* GetProgramAst() const { return program_ast_; }

  llvm::Function* LlvmFunc(const std::string& class_name,
                           const std::string& method_name) {
    return functions_.at(
        GetClassByName(class_name)->GetMethodFeatureByName(method_name));
  }

  llvm::Function* LlvmFunc(const MethodFeature* method_feature) {
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
  llvm::FunctionType* GetConstructorFunctionType(const ClassAst* class_ast);
  llvm::FunctionType* GetCopyConstructorFunctionType();

  void Insert(const ClassAst* class_ast) {
    types_.insert(std::make_pair(
        class_ast, llvm::StructType::create(*context_, class_ast->GetName())));
    vtables_.insert(std::make_pair(class_ast, Vtable(*context_, class_ast)));
  }

  void AddCopyConstructor(const ClassAst* class_ast);

  std::unordered_map<const ClassAst*, llvm::StructType*> types_;
  std::unordered_map<const ClassAst*, Vtable> vtables_;
  std::unordered_map<const ClassAst*, llvm::Function*> constructors_;
  std::unordered_map<const ClassAst*, llvm::Function*> copy_constructors_;

  std::unordered_map<const MethodFeature*, llvm::Function*> functions_;

  llvm::LLVMContext* context_;
  llvm::Module* module_;
  llvm::IRBuilder<>* builder_;

  const ProgramAst* program_ast_;
  const ClassAst* current_class_ = nullptr;
  const MethodFeature* current_method_ = nullptr;

  llvm::StructType* gc_ptrs_info_ty_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_AST_TO_CODE_MAP_H
