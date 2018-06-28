#ifndef COOLANG_SEMANTIC_SEMANTIC_H
#define COOLANG_SEMANTIC_SEMANTIC_H

#include <optional>
#include <string>
#include <vector>
#include "coolang/parser/parser.h"

namespace coolang {

class SemanticError {
 public:
  explicit SemanticError(std::string file_name)
      : file_name_(std::move(file_name)) {}

  std::string ToString(int indent_depth) const {
    using namespace std::string_literals;
    return ""s + '"' + file_name_ + '"' + ", semantic error\n";
  }

 private:
  const std::string file_name_;
};

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
