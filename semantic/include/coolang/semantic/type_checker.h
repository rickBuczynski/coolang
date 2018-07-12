#ifndef COOLANG_SEMANTIC_TYPE_CHECKER_H
#define COOLANG_SEMANTIC_TYPE_CHECKER_H

#include <vector>
#include "coolang/parser/ast.h"
#include "coolang/semantic/inheritance_graph.h"
#include "semantic_error.h"

namespace coolang {

class TypeChecker {
 public:
  static std::vector<SemanticError> CheckTypes(
      ProgramAst& program_ast, const InheritanceGraph& inheritance_graph);

 private:
  static std::vector<const ClassAst*> GetSuperClasses(
      const ProgramAst& program_ast, const std::string& type,
      const InheritanceGraph& inheritance_graph);
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_TYPE_CHECKER_H
