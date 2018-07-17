#ifndef COOLANG_SEMANTIC_TYPE_CHECKER_H
#define COOLANG_SEMANTIC_TYPE_CHECKER_H

#include <vector>
#include "coolang/parser/ast.h"
#include "semantic_error.h"

namespace coolang {

class TypeChecker {
 public:
  static std::vector<SemanticError> CheckTypes(ProgramAst& program_ast);
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_TYPE_CHECKER_H
