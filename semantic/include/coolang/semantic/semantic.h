#ifndef COOLANG_SEMANTIC_SEMANTIC_H
#define COOLANG_SEMANTIC_SEMANTIC_H

#include <unordered_map>
#include <vector>
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic_error.h"

namespace coolang {

class Semantic {
 public:
  explicit Semantic(std::unique_ptr<Parser> parser)
      : parser_(std::move(parser)) {}

  std::variant<ProgramAst, std::vector<ParseError>, std::vector<SemanticError>>
  CheckProgramSemantics() const;

 private:
  std::vector<SemanticError> CheckVariableScope(
      const ProgramAst& program_ast) const;
  void CheckVariableScope(Expr* expr, std::string file_name,
                          std::unordered_map<std::string, int>& in_scope_vars,
                          std::vector<SemanticError>& errors) const;
  std::unique_ptr<Parser> parser_;
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_H
