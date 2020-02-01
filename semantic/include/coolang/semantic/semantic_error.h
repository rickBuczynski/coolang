/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

  std::string ToString() const {
    return file_path_.filename().string() + ":" + std::to_string(line_num_) +
           ": " + error_message_ + "\n";
  }

  static std::string ToString(const std::vector<SemanticError>& errors) {
    std::string str;
    for (const auto& err : errors) {
      str += err.ToString();
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
