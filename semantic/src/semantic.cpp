#include "coolang/semantic/semantic.h"
#include <unordered_map>
#include "coolang/semantic/inheritance_checker.h"
#include "coolang/semantic/type_checker.h"

namespace coolang {

std::vector<SemanticError> Semantic::CheckProgramSemantics(
    ProgramAst& ast) const {
  auto inheritance_graph_errors =
      InheritanceChecker::SetInheritanceRelations(ast);
  if (!inheritance_graph_errors.empty()) {
    return inheritance_graph_errors;
  }

  return TypeChecker::CheckTypes(ast);
}

}  // namespace coolang
