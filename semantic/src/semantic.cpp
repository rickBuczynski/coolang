#include <unordered_map>
#include "coolang/semantic/inheritance_graph.h"
#include "coolang/semantic/scope_checker.h"
#include "coolang/semantic/semantic.h"
#include "coolang/semantic/type_checker.h"

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

  std::vector<SemanticError> type_check_errors =
      TypeChecker::CheckTypes(program_ast);
  if (!type_check_errors.empty()) {
    return type_check_errors;
  }

  return program_ast;
}

}  // namespace coolang
