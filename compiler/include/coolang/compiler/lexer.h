#ifndef COOLANG_COMPILER_LEXER_H_
#define COOLANG_COMPILER_LEXER_H_

#include <string>

class Lexer {
 public:
  Lexer(std::string input_file_name);
  ~Lexer();

  int GetNextToken();

  enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,

    // control
    tok_if = -6,
    tok_then = -7,
    tok_else = -8,
    tok_for = -9,
    tok_in = -10,

    // operators
    tok_binary = -11,
    tok_unary = -12,

    // var definition
    tok_var = -13
  };

  int GetCurTok() { return CurTok; };
  std::string GetIdentifierStr() { return IdentifierStr; };
  double GetNumVal() { return NumVal; };

 private:
  int get_next_char_in_file();
  int gettok();

  FILE* infile;

  /// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the
  /// current token the parser is looking at.  getNextToken reads another token
  /// from the lexer and updates CurTok with its results.
  int CurTok;

  int LastChar = ' ';

  std::string IdentifierStr;  // Filled in if tok_identifier
  double NumVal;              // Filled in if tok_number
};

#endif  // COOLANG_COMPILER_LEXER_H_
