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
  explicit InheritanceGraph(
      std::unordered_map<std::string, std::string> type_to_super)
      : type_to_super_(std::move(type_to_super)) {}

  static std::variant<InheritanceGraph, std::vector<SemanticError>>
  BuildInheritanceGraph(ProgramAst& program_ast) {
    std::vector<SemanticError> errors;
    std::unordered_map<std::string, std::string> type_to_super;
    for (ClassAst& cool_class : program_ast.MutableClasses()) {
      const std::string inherits_type_string =
          cool_class.InheritsTypeAsString();

      if (inherits_type_string == "Bool") {
        errors.emplace_back(
            cool_class.GetLineRange().end_line_num,
            "Class " + cool_class.GetType() + " cannot inherit class Bool.",
            cool_class.GetContainingFileName());
      } else {
        type_to_super[cool_class.GetType()] = inherits_type_string;

        cool_class.SetSuperClass(
            program_ast.GetClassByName(inherits_type_string));
      }
    }

    if (!errors.empty()) {
      return errors;
    }
    return InheritanceGraph(type_to_super);
  }

  std::string GetSuperType(const std::string& type) const {
    return type_to_super_.at(type);
  }

 private:
  std::unordered_map<std::string, std::string> type_to_super_;
};  // namespace coolang

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_INHERITANCE_GRAPH_H
