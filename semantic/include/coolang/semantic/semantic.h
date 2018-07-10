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
  std::unique_ptr<Parser> parser_;
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_H
