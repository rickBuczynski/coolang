#include <unordered_map>
#include "coolang/semantic/inheritance_graph.h"
#include "coolang/semantic/semantic.h"

namespace coolang {

std::variant<ProgramAst, std::vector<ParseError>, std::vector<SemanticError>>
Semantic::CheckProgramSemantics() const {
  auto ast_or_parse_errors = parser_->ParseProgram();

  if (std::holds_alternative<std::vector<ParseError>>(ast_or_parse_errors)) {
    return std::get<std::vector<ParseError>>(ast_or_parse_errors);
  }
  auto program_ast = std::get<ProgramAst>(std::move(ast_or_parse_errors));

  auto inheritance_graph_or_semantic_error =
      InheritanceGraph::BuildInheritanceGraph(program_ast);

  if (std::holds_alternative<std::vector<SemanticError>>(
          inheritance_graph_or_semantic_error)) {
    return std::get<std::vector<SemanticError>>(
        inheritance_graph_or_semantic_error);
  }

  std::vector<SemanticError> variable_scope_errors =
      CheckVariableScope(program_ast);
  if (!variable_scope_errors.empty()) {
    return variable_scope_errors;
  }

  return program_ast;
}

std::vector<SemanticError> Semantic::CheckVariableScope(
    const ProgramAst& program_ast) const {
  std::vector<SemanticError> errors;
  for (const auto& cool_class : program_ast.GetClasses()) {
    for (const auto& feature : cool_class.GetFeatures()) {
      // TODO handle class attributes
      // TODO handle method parameters
      const auto& root_expr = feature->GetRootExpr();

      std::unordered_map<std::string, int> in_scope_vars;
      CheckVariableScope(root_expr.get(), cool_class.GetContainingFileName(),
                         in_scope_vars, errors);
    }
  }
  return errors;
}

void Semantic::CheckVariableScope(
    Expr* expr, std::string file_name,
    std::unordered_map<std::string, int>& in_scope_vars,
    std::vector<SemanticError>& errors) const {
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
