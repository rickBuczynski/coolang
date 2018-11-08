#ifndef COOLANG_SEMANTIC_INHERITANCE_GRAPH_H
#define COOLANG_SEMANTIC_INHERITANCE_GRAPH_H

#include <set>
#include <string>
#include "coolang/parser/ast.h"
#include "coolang/semantic/semantic_error.h"

namespace coolang {

class InheritanceChecker {
 public:
  static std::vector<SemanticError> SetInheritanceRelations(
      ProgramAst& program_ast) {
    std::vector<SemanticError> errors;

    for (ClassAst& cool_class : program_ast.MutableClasses()) {
      const std::string inherits_type_string =
          cool_class.GetSuperClass()->GetName();

      std::set<std::string> cant_iherit_from{"Int", "Bool", "String",
                                             "SELF_TYPE"};

      if (cant_iherit_from.find(inherits_type_string) !=
          cant_iherit_from.end()) {
        errors.emplace_back(cool_class.GetLineRange().end_line_num,
                            "Class " + cool_class.GetName() +
                                " cannot inherit class " +
                                inherits_type_string + ".",
                            cool_class.GetContainingFilePath());
      } else {
        const ClassAst* super_class =
            program_ast.GetClassByName(inherits_type_string);

        if (super_class == nullptr) {
          errors.emplace_back(cool_class.GetLineRange().end_line_num,
                              "Class " + cool_class.GetName() +
                                  " inherits from an undefined class " +
                                  inherits_type_string + ".",
                              cool_class.GetContainingFilePath());
        } else {
          cool_class.SetSuperClass(
              program_ast.GetClassByName(inherits_type_string));
          program_ast.MutableClassByName(inherits_type_string)
              ->AddSubClass(&cool_class);
        }
      }
    }
    return errors;
  }
};  // namespace coolang

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_INHERITANCE_GRAPH_H
