#ifndef COOLANG_COMPILER_CHAR_STREAM_H_
#define COOLANG_COMPILER_CHAR_STREAM_H_

#include <fstream>
#include <string>

class CharStream {
 public:
  CharStream(std::string input_file_name);

  char Pop();
  char Peek() { return cur_char; };

  int CurLineNum() { return cur_line_num; };

 private:
  std::ifstream infile;
  std::string cur_line;
  int cur_line_num = 0;
  int index_in_line = 0;

  char cur_char = ' ';
};

#endif  // COOLANG_COMPILER_CHAR_STREAM_H_
