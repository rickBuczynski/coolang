#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "coolang/codegen/vtable.h"
#include "coolang/parser/ast.h"

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

}  // namespace coolang