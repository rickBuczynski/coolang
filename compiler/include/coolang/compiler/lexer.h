#ifndef COOLANG_COMPILER_LEXER_H_
#define COOLANG_COMPILER_LEXER_H_

#include <string>
#include <variant>
#include <fstream>
#include "coolang/compiler/token.h"

class Lexer {
 public:
  Lexer(std::string input_file_name);

  Token GetNextToken();

  Token GetCurTok() { return CurTok; };
  double GetNumVal() { return NumVal; };

 private:
  int get_next_char_in_file();
  Token gettok();

  std::ifstream infile;
  std::string cur_line;
  int cur_line_num = 0;
  int cur_char_of_line = 0;

  /// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the
  /// current token the parser is looking at.  getNextToken reads another token
  /// from the lexer and updates CurTok with its results.
  Token CurTok;

  int LastChar = ' ';

  double NumVal;              // Filled in if tok_number
};

#endif  // COOLANG_COMPILER_LEXER_H_
