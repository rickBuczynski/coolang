#include "coolang/semantic/semantic.h"

namespace coolang {

std::variant<ProgramAst, std::vector<ParseError>, std::vector<SemanticError>>
Semantic::CheckProgramSemantics() const {
  auto ast_or_parse_errors = parser_->ParseProgram();
  return std::visit(
      [](auto&& e) -> std::variant<ProgramAst, std::vector<ParseError>,
                                   std::vector<SemanticError>> {
        return std::move(e);
      },
      std::move(ast_or_parse_errors));
}

}  // namespace coolang
