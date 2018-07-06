#ifndef COOLANG_SEMANTIC_SEMANTIC_ERROR_H
#define COOLANG_SEMANTIC_SEMANTIC_ERROR_H

#include <string>

namespace coolang {

class SemanticError {
 public:
  SemanticError(std::string error_message, std::string file_name)
      : error_message_(std::move(error_message)),
        file_name_(std::move(file_name)) {}

  std::string ToString(int indent_depth) const { return error_message_ + "\n"; }

 private:
  std::string error_message_;
  std::string file_name_;
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_ERROR_H