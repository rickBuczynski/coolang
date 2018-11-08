#ifndef COOLANG_SEMANTIC_SEMANTIC_ERROR_H
#define COOLANG_SEMANTIC_SEMANTIC_ERROR_H

#include <filesystem>
#include <string>

namespace coolang {

class SemanticError {
 public:
  SemanticError(int line_num, std::string error_message,
                std::filesystem::path file_path)
      : line_num_(line_num),
        error_message_(std::move(error_message)),
        file_path_(std::move(file_path)) {}

  std::string ToString(int indent_depth) const {
    return file_path_.filename().string() + ":" + std::to_string(line_num_) +
           ": " + error_message_ + "\n";
  }

  static std::string ToString(const std::vector<SemanticError>& errors) {
    std::string str;
    for (const auto& err : errors) {
      str += err.ToString(0);
    }
    str += "Compilation halted due to static semantic errors.\n";
    return str;
  }

 private:
  int line_num_;
  std::string error_message_;
  std::filesystem::path file_path_;
};

}  // namespace coolang

#endif  // COOLANG_SEMANTIC_SEMANTIC_ERROR_H
