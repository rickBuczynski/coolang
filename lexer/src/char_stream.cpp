#include <fstream>
#include <iostream>
#include "coolang/lexer/char_stream.h"

CharStream::CharStream(std::string input_file_name)
    // open as binary to prevent ascii code 26 from triggering EOF
    // https://stackoverflow.com/questions/15034012/ifstream-eof-end-of-file-is-reached-before-the-real-end
    : infile(input_file_name, std::ios_base::in | std::ios_base::binary) {
  if (infile.fail()) {
    std::cerr << "Failed to open file: " << input_file_name << std::endl;
    std::exit(1);
  }
  Pop();
}

void CharStream::Pop() {
  if (cur_char == '\n') {
    cur_line_num++;
  }

  infile.get(cur_char);

  if (infile.eof()) {
    cur_char = EOF;
  }
}
