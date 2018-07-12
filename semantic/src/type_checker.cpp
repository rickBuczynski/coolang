#include <stack>
#include "coolang/parser/ast.h"
#include "coolang/semantic/inheritance_graph.h"
#include "coolang/semantic/type_checker.h"

namespace coolang {

void PopAndEraseIfEmpty(
    std::unordered_map<std::string, std::stack<std::string>>& in_scope_vars,
    const std::string& id) {
  in_scope_vars[id].pop();
  if (in_scope_vars[id].empty()) {
    in_scope_vars.erase(id);
  }
}

std::vector<SemanticError> TypeChecker::CheckTypes(
    ProgramAst& program_ast, const InheritanceGraph& inheritance_graph) {
  std::vector<SemanticError> errors;
  std::unordered_map<std::string, std::stack<std::string>> in_scope_vars;
  for (const auto& cool_class : program_ast.GetClasses()) {
    // add all attributes to scope before diving into expressions since these
    // attributes are visible throughout the class
    for (const auto* attr : cool_class.GetAttributeFeatures()) {
      if (attr->GetId() == "self") {
        errors.emplace_back(attr->GetLineRange().end_line_num,
                            "'self' cannot be the name of an attribute.",
                            cool_class.GetContainingFileName());
        return errors;
      }
      std::vector<const ClassAst*> super_classes =
          GetSuperClasses(program_ast, cool_class.GetType(), inheritance_graph);
      for (const auto* super_class : super_classes) {
        // TODO this is slow, could be faster
        for (const auto super_attr : super_class->GetAttributeFeatures()) {
          if (super_attr->GetId() == attr->GetId()) {
            errors.emplace_back(
                attr->GetLineRange().end_line_num,
                "Attribute " + cool_class.GetType() + "." + attr->GetId() +
                    " is already defined in class " + super_class->GetType() +
                    " at " + super_class->GetContainingFileName() + ":" +
                    std::to_string(super_attr->GetLineRange().end_line_num) +
                    ".",
                cool_class.GetContainingFileName());
            return errors;
          }
        }
      }

      in_scope_vars[attr->GetId()].push(attr->GetType());
    }

    for (const auto& feature : cool_class.GetFeatures()) {
      // TODO handle scope for method parameters
      if (feature->GetRootExpr()) {
        feature->GetRootExpr()->CheckType(errors, in_scope_vars,
                                          cool_class.GetContainingFileName());
      }
    }

    // remove all attributes frome scope
    for (const auto& feature : cool_class.GetFeatures()) {
      if (auto* attr = dynamic_cast<AttributeFeature*>(feature.get())) {
        PopAndEraseIfEmpty(in_scope_vars, attr->GetId());
      }
    }
  }
  return errors;
}

std::vector<const ClassAst*> TypeChecker::GetSuperClasses(
    const ProgramAst& program_ast, const std::string& type,
    const InheritanceGraph& inheritance_graph) {
  std::vector<const ClassAst*> super_classes;
  std::string super_type = inheritance_graph.GetSuperType(type);
  while (super_type != "Object" && super_type != "IO") {
    const auto& cool_class = program_ast.GetClassByName(super_type);
    super_classes.push_back(&cool_class);
    super_type = inheritance_graph.GetSuperType(super_type);
  }
  return super_classes;
}

std::string ObjectExpr::CheckType(
    std::vector<SemanticError>& errors,
    std::unordered_map<std::string, std::stack<std::string>>& in_scope_vars,
    std::string file_name) {
  if (in_scope_vars.find(id_) == in_scope_vars.end()) {
    errors.emplace_back(GetLineRange().end_line_num,
                        "Undeclared identifier " + id_ + ".", file_name);
  }
  return "TODOObjectExpr";
}

std::string LetExpr::CheckType(
    std::vector<SemanticError>& errors,
    std::unordered_map<std::string, std::stack<std::string>>& in_scope_vars,
    std::string file_name) {
  if (GetInitializationExpr()) {
    GetInitializationExpr()->CheckType(errors, in_scope_vars, file_name);
  }

  in_scope_vars[id_].push(type_);

  if (GetInExpr()) {
    GetInExpr()->CheckType(errors, in_scope_vars, file_name);
  } else if (GetChainedLet()) {
    // TODO can you use a variable from earlier in the chain in an init
    // expression within the chain?
    GetChainedLet()->CheckType(errors, in_scope_vars, file_name);
  }

  PopAndEraseIfEmpty(in_scope_vars, id_);

  return "TODOLetExpr";
}

std::string AssignExpr::CheckType(
    std::vector<SemanticError>& errors,
    std::unordered_map<std::string, std::stack<std::string>>& in_scope_vars,
    std::string file_name) {
  auto rhs_type = rhs_expr_->CheckType(errors, in_scope_vars, file_name);
  auto lhs_type = in_scope_vars[GetId()].top();

  if (rhs_type != lhs_type) {
    errors.emplace_back(GetLineRange().end_line_num,
                        "Type " + rhs_type +
                            " of assigned expression does not conform to "
                            "declared type " +
                            lhs_type + " of identifier " + GetId() + ".",
                        file_name);
  }

  return expr_type_ = rhs_type;
}

}  // namespace coolang