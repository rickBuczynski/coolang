#include <fstream>
#include <iostream>
#include "coolang/compiler/char_stream.h"

CharStream::CharStream(std::string input_file_name) : infile(input_file_name) {
  if (infile.fail()) {
    std::cerr << "Failed to open file: " << input_file_name << std::endl;
    std::exit(1);
  }
  // get the first char ready
  Pop();
}

char CharStream::Pop() {
  if (index_in_line >= cur_line.length()) {
    if (infile.eof()) {
      return cur_char = EOF;
	}
    std::getline(infile, cur_line);

    cur_line += '\n';
    cur_line_num++;
    index_in_line = 0;
  }

  // TODO test with empty file
  return cur_char = cur_line[index_in_line++];
}
