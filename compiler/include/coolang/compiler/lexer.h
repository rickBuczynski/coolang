#ifndef COOLANG_COMPILER_LEXER_H_
#define COOLANG_COMPILER_LEXER_H_

#include <string>
#include <variant>
#include "coolang/compiler/token.h"

class Lexer {
 public:
  Lexer(std::string input_file_name);
  ~Lexer();

  Token GetNextToken();

  Token GetCurTok() { return CurTok; };
  std::string GetIdentifierStr() { return IdentifierStr; };
  double GetNumVal() { return NumVal; };

 private:
  int get_next_char_in_file();
  Token gettok();

  FILE* infile;

  /// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the
  /// current token the parser is looking at.  getNextToken reads another token
  /// from the lexer and updates CurTok with its results.
  Token CurTok;

  int LastChar = ' ';

  std::string IdentifierStr;  // Filled in if tok_identifier
  double NumVal;              // Filled in if tok_number
};

#endif  // COOLANG_COMPILER_LEXER_H_
