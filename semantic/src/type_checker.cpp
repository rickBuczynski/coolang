#include "coolang/parser/ast.h"
#include "coolang/semantic/type_checker.h"

namespace coolang {

std::vector<SemanticError> TypeChecker::CheckTypes(ProgramAst& program_ast) {
  std::vector<SemanticError> errors;
  std::unordered_map<std::string, int> in_scope_vars;
  for (const auto& cool_class : program_ast.GetClasses()) {
    for (const auto& feature : cool_class.GetFeatures()) {
      if (auto* attr = dynamic_cast<AttributeFeature*>(feature.get())) {
        if (attr->GetId() == "self") {
          errors.emplace_back(attr->GetLineRange().end_line_num,
                              "'self' cannot be the name of an attribute.",
                              cool_class.GetContainingFileName());
        }
      }
      // TODO handle scope for class attributes
      // TODO handle scope for method parameters

      const auto& root_expr = feature->GetRootExpr();
      if (root_expr) {
        root_expr->CheckType(errors, in_scope_vars,
                             cool_class.GetContainingFileName());
      }
    }
  }
  return errors;
}

std::string ObjectExpr::CheckType(
    std::vector<SemanticError>& errors,
    std::unordered_map<std::string, int>& in_scope_vars,
    std::string file_name) {
  if (in_scope_vars.find(id_) == in_scope_vars.end()) {
    errors.emplace_back(GetLineRange().end_line_num,
                        "Undeclared identifier " + id_ + ".", file_name);
  }
  return "TODOObjectExpr";
}

std::string LetExpr::CheckType(
    std::vector<SemanticError>& errors,
    std::unordered_map<std::string, int>& in_scope_vars,
    std::string file_name) {
  if (GetInitializationExpr()) {
    GetInitializationExpr()->CheckType(errors, in_scope_vars, file_name);
  }

  in_scope_vars[GetId()]++;

  if (GetInExpr()) {
    GetInExpr()->CheckType(errors, in_scope_vars, file_name);
  } else if (GetChainedLet()) {
    // TODO can you use a variable from earlier in the chain in an init
    // expression within the chain?
    GetChainedLet()->CheckType(errors, in_scope_vars, file_name);
  }

  in_scope_vars[GetId()]--;
  if (in_scope_vars[GetId()] == 0) {
    in_scope_vars.erase(GetId());
  }

  return "TODOLetExpr";
}

}  // namespace coolang