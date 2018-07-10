#ifndef COOLANG_SEMANTIC_SCOPE_CHECKER_H
#define COOLANG_SEMANTIC_SCOPE_CHECKER_H

#include <unordered_map>
#include <vector>
#include "coolang/parser/ast.h"
#include "semantic_error.h"

namespace coolang {

class ScopeChecker {
 public:
  static std::vector<SemanticError> CheckVariableScope(
      const ProgramAst& program_ast);

  static void CheckVariableScope(
      Expr* expr, std::string file_name,
      std::unordered_map<std::string, int>& in_scope_vars,
      std::vector<SemanticError>& errors);
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SCOPE_CHECKER_H
