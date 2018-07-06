#ifndef COOLANG_SEMANTIC_SEMANTIC_ERROR_H
#define COOLANG_SEMANTIC_SEMANTIC_ERROR_H

#include <string>

namespace coolang {

class SemanticError {
 public:
  SemanticError(int line_num, std::string error_message, std::string file_name)
      : line_num_(line_num),
        error_message_(std::move(error_message)),
        file_name_(std::move(file_name)) {}

  std::string ToString(int indent_depth) const {
    return file_name_ + ":" + std::to_string(line_num_) + ": " +
           error_message_ + "\n";
  }

 private:
  int line_num_;
  std::string error_message_;
  std::string file_name_;
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_ERROR_H
