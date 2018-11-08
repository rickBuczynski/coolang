#ifndef COOLANG_SEMANTIC_SEMANTIC_H
#define COOLANG_SEMANTIC_SEMANTIC_H

#include <vector>
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic_error.h"

namespace coolang {

class Semantic {
 public:
  std::vector<SemanticError> CheckProgramSemantics(ProgramAst& ast) const;

  static std::string ToString(const std::vector<SemanticError>& errors) {
    std::string str;
    for (const auto& err : errors) {
      str += err.ToString(0);
    }
    str += "Compilation halted due to static semantic errors.\n";
    return str;
  }
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_H
