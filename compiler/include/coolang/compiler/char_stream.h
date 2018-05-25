#ifndef COOLANG_COMPILER_CHAR_STREAM_H_
#define COOLANG_COMPILER_CHAR_STREAM_H_

#include <fstream>
#include <string>

class CharStream {
 public:
  CharStream(std::string input_file_name);

  int Pop();
  int Peek() { return cur_char; };

  int CurLineNum() { return cur_line_num; };

 private:
  std::ifstream infile;
  std::string cur_line;
  int cur_line_num = 0;
  int cur_char_of_line = 0;

  // TODO why did llvm tutorial use int instead of char?
  int cur_char = ' ';
};

#endif  // COOLANG_COMPILER_CHAR_STREAM_H_
