#ifndef COOLANG_COMPILER_LEXER_H_
#define COOLANG_COMPILER_LEXER_H_

#include <string>
#include <variant>
#include <fstream>
#include "coolang/compiler/token.h"
#include "coolang/compiler/char_stream.h"

class Lexer {
 public:
  Lexer(std::string input_file_name);

  Token GetNextToken();

  Token GetCurTok() { return CurTok; };
  double GetNumVal() { return NumVal; };

 private:
  Token gettok();

  /// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the
  /// current token the parser is looking at.  getNextToken reads another token
  /// from the lexer and updates CurTok with its results.
  Token CurTok;

  double NumVal;              // Filled in if tok_number

  CharStream char_stream_;
};

#endif  // COOLANG_COMPILER_LEXER_H_
