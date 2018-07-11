#include <unordered_map>
#include "coolang/semantic/scope_checker.h"

namespace coolang {

std::vector<SemanticError> ScopeChecker::CheckVariableScope(
    const ProgramAst& program_ast) {
  std::vector<SemanticError> errors;
  for (const auto& cool_class : program_ast.GetClasses()) {
    for (const auto& feature : cool_class.GetFeatures()) {
      if (auto* attr = dynamic_cast<AttributeFeature*>(feature.get())) {
        if (attr->GetId() == "self") {
          errors.emplace_back(attr->GetLineRange().end_line_num,
                              "'self' cannot be the name of an attribute.",
                              cool_class.GetContainingFileName());
        }
      }
      // TODO handle class attributes
      // TODO handle method parameters
      const auto& root_expr = feature->GetRootExpr();

      std::unordered_map<std::string, int> in_scope_vars;
      if (root_expr) {
        CheckVariableScope(root_expr.get(), cool_class.GetContainingFileName(),
                           in_scope_vars, errors);
      }
    }
  }
  return errors;
}

void ScopeChecker::CheckVariableScope(
    Expr* expr, std::string file_name,
    std::unordered_map<std::string, int>& in_scope_vars,
    std::vector<SemanticError>& errors) {
  if (auto* obj_expr = dynamic_cast<ObjectExpr*>(expr)) {
    if (in_scope_vars.find(obj_expr->GetId()) == in_scope_vars.end()) {
      errors.emplace_back(obj_expr->GetLineRange().end_line_num,
                          "Undeclared identifier " + obj_expr->GetId() + ".",
                          file_name);
    }
  } else if (auto* let_expr = dynamic_cast<LetExpr*>(expr)) {
    if (let_expr->GetInitializationExpr()) {
      CheckVariableScope(let_expr->GetInitializationExpr().get(), file_name,
                         in_scope_vars, errors);
    }

    in_scope_vars[let_expr->GetId()]++;

    if (let_expr->GetInExpr()) {
      CheckVariableScope(let_expr->GetInExpr().get(), file_name, in_scope_vars,
                         errors);
    } else if (let_expr->GetChainedLet()) {
      // TODO can you use a variable from earlier in the chain in an init
      // expression within the chain?
      CheckVariableScope(let_expr->GetChainedLet().get(), file_name,
                         in_scope_vars, errors);
    }

    in_scope_vars[let_expr->GetId()]--;
    if (in_scope_vars[let_expr->GetId()] == 0) {
      in_scope_vars.erase(let_expr->GetId());
    }
  } else {
    for (auto child : expr->GetChildExprs()) {
      CheckVariableScope(child, file_name, in_scope_vars, errors);
    }
  }

  // TODO handle case branches
}

}  // namespace coolang