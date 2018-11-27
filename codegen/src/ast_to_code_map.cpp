#include "coolang/codegen/ast_to_code_map.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include "coolang/codegen/vtable.h"
#include "coolang/parser/ast.h"

namespace coolang {

void AstToCodeMap::AddAttributes(const ClassAst* class_ast) {
  current_class_ = class_ast;

  std::vector<llvm::Type*> class_attributes;
  // obj_gc_next_obj_index
  class_attributes.push_back(LlvmClass("Object")->getPointerTo());
  // obj_gc_next_root_index
  class_attributes.push_back(LlvmClass("Object")->getPointerTo());
  // obj_gc_is_reachable_index
  // use an i8 instead of i1 since clang emits i8 for bool
  // i1 should work too since i8 is min addressable data but use i8 to be safe
  class_attributes.push_back(builder_->getInt8Ty());

  // obj_typename_index
  class_attributes.push_back(builder_->getInt8PtrTy());

  // obj_vtable_index
  class_attributes.push_back(
      GetVtable(class_ast).GetStructType()->getPointerTo());
  // obj_typesize_index (to allow copying without a different function per type)
  class_attributes.push_back(builder_->getInt32Ty());
  // obj_constructor_index (to support "new SELF_TYPE")
  class_attributes.push_back(
      GetConstructorFunctionType(class_ast)->getPointerTo());
  // obj_boxed_data_index
  class_attributes.push_back(builder_->getInt8PtrTy());

  for (const ClassAst* cur_class : class_ast->SupersThenThis()) {
    for (const auto* attr : cur_class->GetAttributeFeatures()) {
      llvm::Type* attr_type = LlvmBasicOrClassPtrTy(attr->GetType());
      class_attributes.push_back(attr_type);
    }
  }

  types_.at(class_ast)->setBody(class_attributes);

  current_class_ = nullptr;
}

void AstToCodeMap::AddMethods(const ClassAst* class_ast) {
  current_class_ = class_ast;

  std::vector<llvm::Constant*> vtable_functions;

  if (class_ast->GetSuperClass() != nullptr) {
    const std::vector<llvm::Constant*>& super_vtable_functions =
        GetVtable(class_ast->GetSuperClass()).GetFunctions();

    vtable_functions.insert(vtable_functions.end(),
                            super_vtable_functions.begin(),
                            super_vtable_functions.end());
  }

  for (const auto* method : class_ast->GetMethodFeatures()) {
    llvm::Type* return_type = LlvmBasicOrClassPtrTy(method->GetReturnType());

    std::vector<llvm::Type*> arg_types;
    // first param is always implicit 'self'
    arg_types.push_back(LlvmBasicOrClassPtrTy(class_ast->GetName()));
    for (const auto& arg : method->GetArgs()) {
      arg_types.push_back(LlvmBasicOrClassPtrTy(arg.GetType()));
    }

    llvm::FunctionType* func_type =
        llvm::FunctionType::get(return_type, arg_types, false);
    llvm::Function* func = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage,
        class_ast->GetName() + "-" + method->GetId(), module_);
    functions_[method] = func;

    if (TypeUsesVtable(class_ast->GetName())) {
      const size_t vtable_method_index =
          GetVtable(class_ast).GetIndexOfMethodFeature(method);
      if (vtable_method_index < vtable_functions.size()) {
        // redefining a super method
        vtable_functions[vtable_method_index] = func;
      } else {
        assert(vtable_method_index == vtable_functions.size());
        vtable_functions.push_back(func);
      }
    }
  }

  if (TypeUsesVtable(class_ast->GetName())) {
    vtables_.at(class_ast).BuildVtable(module_, vtable_functions);
  }

  current_class_ = nullptr;
}

void AstToCodeMap::AddConstructor(const ClassAst* class_ast) {
  using namespace std::string_literals;

  llvm::FunctionType* constructor_func_type =
      GetConstructorFunctionType(class_ast);

  llvm::Function* constructor = llvm::Function::Create(
      constructor_func_type, llvm::Function::ExternalLinkage,
      "construct"s + "-" + class_ast->GetName(), module_);

  constructors_.insert(std::make_pair(class_ast, constructor));
}

llvm::FunctionType* AstToCodeMap::GetConstructorFunctionType(
    const ClassAst* class_ast) {
  return llvm::FunctionType::get(builder_->getVoidTy(),
                                 {LlvmClass(class_ast)->getPointerTo()}, false);
}

}  // namespace coolang