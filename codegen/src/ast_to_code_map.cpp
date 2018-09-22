#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include "coolang/codegen/ast_to_code_map.h"
#include "coolang/codegen/vtable.h"
#include "coolang/parser/ast.h"

namespace coolang {

void AstToCodeMap::AddAttributes(const ClassAst* class_ast) {
  std::vector<llvm::Type*> class_attributes;
  class_attributes.push_back(
      GetVtable(class_ast).GetStructType()->getPointerTo());

  for (const auto* attr : class_ast->GetAttributeFeatures()) {
    llvm::Type* attr_type = GetLlvmBasicOrPointerToClassType(attr->GetType());
    class_attributes.push_back(attr_type);
  }

  types_.at(class_ast)->setBody(class_attributes);
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
    llvm::Type* return_type =
        GetLlvmBasicOrPointerToClassType(method->GetReturnType());

    std::vector<llvm::Type*> arg_types;
    // first param is always implicit 'self'
    arg_types.push_back(GetLlvmBasicOrPointerToClassType(class_ast->GetName()));
    for (const auto& arg : method->GetArgs()) {
      arg_types.push_back(GetLlvmBasicOrPointerToClassType(arg.GetType()));
    }

    llvm::FunctionType* func_type =
        llvm::FunctionType::get(return_type, arg_types, false);
    llvm::Function* func = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage,
        class_ast->GetName() + "-" + method->GetId(), module_);
    functions_[method] = func;

    // String defines methods but it can't be inherited from so no vtable
    if (class_ast->GetName() != "String") {
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

  if (class_ast->GetName() != "String") {
    vtables_.at(class_ast).BuildVtable(module_, vtable_functions);
  }

  current_class_ = nullptr;
}

void AstToCodeMap::AddConstructor(const ClassAst* class_ast) {
  using namespace std::string_literals;

  std::vector<llvm::Type*> constructor_arg_types;

  constructor_arg_types.push_back(LlvmClass(class_ast)->getPointerTo());

  llvm::FunctionType* constructor_func_type = llvm::FunctionType::get(
      builder_->getVoidTy(), constructor_arg_types, false);
  llvm::Function* constructor = llvm::Function::Create(
      constructor_func_type, llvm::Function::ExternalLinkage,
      "construct"s + "-" + class_ast->GetName(), module_);

  constructors_.insert(std::make_pair(class_ast, constructor));
}

}  // namespace coolang