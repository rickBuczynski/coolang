#include <fstream>
#include <iostream>
#include "coolang/compiler/char_stream.h"

CharStream::CharStream(std::string input_file_name) : infile(input_file_name) {
  if (infile.fail()) {
    std::cerr << "Failed to open file: " << input_file_name << std::endl;
    std::exit(1);
  }
  std::getline(infile, cur_line);
  cur_line += '\n';

  cur_char = cur_line[index_in_line];
}

void CharStream::Pop() {
  index_in_line++;

  if (index_in_line >= cur_line.length()) {
    if (infile.eof()) {
      cur_char = EOF;
      return;
	}
    std::getline(infile, cur_line);
    cur_line += '\n';

    cur_line_num++;
    index_in_line = 0;
  }

  cur_char = cur_line[index_in_line];

  // TODO test with empty file
}
