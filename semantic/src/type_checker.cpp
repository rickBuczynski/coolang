#include <unordered_map>
#include "coolang/semantic/type_checker.h"

namespace coolang {

std::vector<SemanticError> TypeChecker::CheckTypes(ProgramAst& program_ast) {
  std::vector<SemanticError> errors;
  for (const auto& cool_class : program_ast.GetClasses()) {
    for (const auto& feature : cool_class.GetFeatures()) {
      const auto& root_expr = feature->GetRootExpr();
      if (root_expr) {
        root_expr->InferType();
      }
    }
  }
  return errors;
}

}  // namespace coolang