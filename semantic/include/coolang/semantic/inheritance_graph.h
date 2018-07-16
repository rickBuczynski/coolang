#ifndef COOLANG_SEMANTIC_INHERITANCE_GRAPH_H
#define COOLANG_SEMANTIC_INHERITANCE_GRAPH_H

#include <string>
#include <unordered_map>
#include <variant>
#include "coolang/parser/ast.h"
#include "coolang/semantic/semantic_error.h"

namespace coolang {

class InheritanceGraph {
 public:
  static std::vector<SemanticError> BuildInheritanceGraph(
      ProgramAst& program_ast) {
    std::vector<SemanticError> errors;

    for (ClassAst& cool_class : program_ast.MutableClasses()) {
      const std::string inherits_type_string =
          cool_class.InheritsTypeAsString();

      if (inherits_type_string == "Bool") {
        errors.emplace_back(
            cool_class.GetLineRange().end_line_num,
            "Class " + cool_class.GetType() + " cannot inherit class Bool.",
            cool_class.GetContainingFileName());
      } else {
        cool_class.SetSuperClass(
            program_ast.GetClassByName(inherits_type_string));
      }
    }
    return errors;
  }
};  // namespace coolang

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_INHERITANCE_GRAPH_H
