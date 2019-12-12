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

#ifndef COOLANG_LEXER_CHAR_STREAM_H_
#define COOLANG_LEXER_CHAR_STREAM_H_

#include <fstream>
#include <string>

namespace coolang {

class CharStream {
 public:
  explicit CharStream(const std::string& input_file_name);

  void Pop();
  char Peek() const { return cur_char_; }

  int CurLineNum() const { return cur_line_num_; }

 private:
  std::ifstream infile_;

  int cur_line_num_ = 1;

  // set an initial value so we don't increment cur_line_num when comparing to
  // newline during first pop
  char cur_char_ = 0;
};

}  // namespace coolang

#endif  // COOLANG_LEXER_CHAR_STREAM_H_
