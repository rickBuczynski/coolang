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
  BuildInheritanceGraph(const ProgramAst& program_ast) {
    std::vector<SemanticError> errors;
    std::unordered_map<std::string, std::string> type_to_super;
    for (const auto& cool_class : program_ast.GetClasses()) {
      const std::string inherits_type = cool_class.InheritsTypeAsString();

      if (inherits_type == "Bool") {
        errors.emplace_back(
            "Class " + cool_class.GetType() + " cannot inherit class Bool.",
            cool_class.GetContainingFileName());
      }
      type_to_super[cool_class.GetType()] = inherits_type;
    }

    if (!errors.empty()) {
      return errors;
    }
    return InheritanceGraph(type_to_super);
  }

 private:
  std::unordered_map<std::string, std::string> type_to_super_;
};  // namespace coolang

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_INHERITANCE_GRAPH_H
