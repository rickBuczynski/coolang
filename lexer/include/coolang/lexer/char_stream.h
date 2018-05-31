#ifndef COOLANG_LEXER_CHAR_STREAM_H_
#define COOLANG_LEXER_CHAR_STREAM_H_

#include <fstream>
#include <string>

class CharStream {
 public:
  CharStream(std::string input_file_name);

  void Pop();
  char Peek() { return cur_char; };

  int CurLineNum() { return cur_line_num; };

 private:
  std::ifstream infile;

  int cur_line_num = 1;

  // set an initial value so we don't increment cur_line_num when comparing to
  // newline during first pop
  char cur_char = 0;
};

#endif  // COOLANG_LEXER_CHAR_STREAM_H_
