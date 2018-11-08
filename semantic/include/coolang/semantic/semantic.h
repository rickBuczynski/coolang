#ifndef COOLANG_SEMANTIC_SEMANTIC_H
#define COOLANG_SEMANTIC_SEMANTIC_H

#include <vector>
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic_error.h"

namespace coolang {

class Semantic {
 public:
  std::vector<SemanticError> CheckProgramSemantics(ProgramAst& ast) const;
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_H
