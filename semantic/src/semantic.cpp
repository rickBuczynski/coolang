#include "coolang/semantic/inheritance_graph.h"
#include "coolang/semantic/semantic.h"

namespace coolang {

std::variant<ProgramAst, std::vector<ParseError>, std::vector<SemanticError>>
Semantic::CheckProgramSemantics() const {
  auto ast_or_parse_errors = parser_->ParseProgram();
  return std::visit(
      [](auto&& e) -> std::variant<ProgramAst, std::vector<ParseError>,
                                   std::vector<SemanticError>> {
        using T = std::decay_t<decltype(e)>;
        if constexpr (std::is_same_v<T, ProgramAst>) {
          auto inheritance_graph_or_semantic_error =
              InheritanceGraph::BuildInheritanceGraph(e);

          if (std::holds_alternative<std::vector<SemanticError>>(
                  inheritance_graph_or_semantic_error)) {
            return std::get<std::vector<SemanticError>>(
                inheritance_graph_or_semantic_error);
          }
          return std::move(e);
        } else if constexpr (std::is_same_v<T, std::vector<ParseError>>) {
          return std::move(e);
        }
      },
      std::move(ast_or_parse_errors));
}

}  // namespace coolang
