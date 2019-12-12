// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <iostream>
#include "coolang/lexer/char_stream.h"

namespace coolang {

CharStream::CharStream(const std::string& input_file_name)
    // open as binary to prevent ascii code 26 from triggering EOF
    // https://stackoverflow.com/questions/15034012/ifstream-eof-end-of-file-is-reached-before-the-real-end
    : infile_(input_file_name, std::ios_base::in | std::ios_base::binary) {
  if (infile_.fail()) {
    std::cerr << "Failed to open file: " << input_file_name << std::endl;
    std::exit(1);
  }
  Pop();
}

void CharStream::Pop() {
  if (cur_char_ == '\n') {
    cur_line_num_++;
  }

  infile_.get(cur_char_);

  if (infile_.eof()) {
    cur_char_ = EOF;
  }
}

}  // namespace coolang