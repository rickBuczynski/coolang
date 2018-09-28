#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "coolang/codegen/vtable.h"
#include "coolang/parser/ast.h"
#include <set>

namespace coolang {

void Vtable::BuildVtable(llvm::Module* module,
                         const std::vector<llvm::Constant*>& vtable_functions) {
  std::vector<llvm::Type*> vtable_method_types;
  vtable_method_types.reserve(vtable_functions.size());
  for (auto func : vtable_functions) {
    vtable_method_types.push_back(func->getType());
  }

  struct_type_->setBody(vtable_method_types);

  module->getOrInsertGlobal(class_ast_->GetName() + "-vtable-global",
                            struct_type_);
  llvm::GlobalVariable* vtable =
      module->getNamedGlobal(class_ast_->GetName() + "-vtable-global");
  vtable->setConstant(true);
  vtable->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
  vtable->setInitializer(
      llvm::ConstantStruct::get(struct_type_, vtable_functions));
  functions_ = vtable_functions;
  global_instance_ = vtable;
}

size_t Vtable::GetIndexOfMethodFeature(const MethodFeature* method_feature) const {
  std::set<std::string> already_seen_method_ids;
  size_t vtable_method_index = 0;
  for (const auto* cool_class : class_ast_->SupersThenThis()) {
    for (const auto* method : cool_class->GetMethodFeatures()) {
      if (method->GetId() == method_feature->GetId()) {
        return vtable_method_index;
      }
      if (already_seen_method_ids.find(method->GetId()) ==
          already_seen_method_ids.end()) {
        vtable_method_index++;
      }
      already_seen_method_ids.insert(method->GetId());
    }
  }
  throw std::invalid_argument(
                              "Method: " + method_feature->GetId() +
                              " not defined in class: " +
                              class_ast_->GetName() + " or its super classes.");
}

}  // namespace coolang