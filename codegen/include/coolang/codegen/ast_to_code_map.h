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
               llvm::IRBuilder<>* builder, llvm::DataLayout* data_layout,
               const ProgramAst* program_ast)
      : context_(context),
        module_(module),
        builder_(builder),
        data_layout_(data_layout),
        program_ast_(program_ast) {}

  static constexpr int obj_vtable_index = 0;
  static constexpr int obj_typename_index = 1;
  static constexpr int obj_typesize_index = 2;
  // need a pointer to constructor to handle "new SELF_TYPE"
  static constexpr int obj_constructor_index = 3;

  // attrbiutes start at index 4, after the 4 things above
  static constexpr int obj_attributes_offset = 4;

  void Insert(const ClassAst* class_ast) {
    types_.insert(std::make_pair(
        class_ast, llvm::StructType::create(*context_, class_ast->GetName())));
    vtables_.insert(std::make_pair(class_ast, Vtable(*context_, class_ast)));
  }

  void AddAttributes(const ClassAst* class_ast);
  void AddMethods(const ClassAst* class_ast);
  void AddConstructor(const ClassAst* class_ast);

  static bool IsBasicType(const std::string& class_name) {
    return class_name == "Int" || class_name == "String" ||
           class_name == "Bool";
  }

  // TODO using one global value for Basic types boxed as objects wont work
  // you can cast an object back to an int/string/bool using "case" expr
  // need to actually allocate the boxed type and store the value
  void InsertBoxedBasicTypeGlobal(const std::string& basic_type) {
    const std::string global_name = "boxed-" + basic_type + "-global";

    module_->getOrInsertGlobal(global_name, LlvmClass("Object"));
    llvm::GlobalVariable* boxed = module_->getNamedGlobal(global_name);
    boxed->setConstant(false);
    boxed->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);

    const auto obj_class_size =
        data_layout_->getTypeAllocSize(LlvmClass("Object"));

    auto* obj_constructor_type =
        GetConstructorFunctionType(program_ast_->GetObjectClass())
            ->getPointerTo();

    boxed->setInitializer(llvm::ConstantStruct::get(
        LlvmClass("Object"),
        {GetVtable("Object").GetGlobalInstance(),
         llvm::ConstantPointerNull::get(builder_->getInt8PtrTy()),
         LlvmConstInt32(obj_class_size),
         llvm::ConstantPointerNull::get(obj_constructor_type)}));

    llvm::Value* typename_ptr_ptr = builder_->CreateStructGEP(
        LlvmClass("Object"), boxed, obj_typename_index);

    builder_->CreateStore(builder_->CreateGlobalStringPtr(basic_type),
                          typename_ptr_ptr);

    boxed_basic_type_globals_[basic_type] = boxed;
  }

  llvm::GlobalValue* GetBoxedBasicTypeGlobal(const std::string& basic_type) {
    return boxed_basic_type_globals_.at(basic_type);
  }

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

  llvm::Type* LlvmBasicOrClassPtrTy(const std::string& type_name) {
    llvm::Type* type = LlvmBasicType(type_name);
    if (type == nullptr) {
      type = LlvmClass(type_name)->getPointerTo();
    }
    return type;
  }

  llvm::ConstantInt* LlvmConstInt32(uint64_t i) const {
    return llvm::ConstantInt::get(*context_, llvm::APInt(32, i, true));
  }

  llvm::Value* LlvmBasicOrClassPtrDefaultVal(const std::string& type_name) {
    if (type_name == "Int") {
      return LlvmConstInt32(0);
    }
    if (type_name == "String") {
      return builder_->CreateGlobalStringPtr("");
    }
    if (type_name == "Bool") {
      return llvm::ConstantInt::get(*context_, llvm::APInt(1, 0, false));
    }
    return llvm::ConstantPointerNull::get(LlvmClass(type_name)->getPointerTo());
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

  llvm::FunctionType* GetConstructorFunctionType(const ClassAst* class_ast);

 private:
  std::unordered_map<const ClassAst*, llvm::StructType*> types_;
  std::unordered_map<const ClassAst*, Vtable> vtables_;
  std::unordered_map<const ClassAst*, llvm::Function*> constructors_;

  std::unordered_map<const MethodFeature*, llvm::Function*> functions_;

  std::unordered_map<std::string, llvm::GlobalValue*> boxed_basic_type_globals_;

  llvm::LLVMContext* context_;
  llvm::Module* module_;
  llvm::IRBuilder<>* builder_;
  llvm::DataLayout* data_layout_;

  const ProgramAst* program_ast_;
  const ClassAst* current_class_ = nullptr;
  const MethodFeature* current_method_ = nullptr;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_AST_TO_CODE_MAP_H
