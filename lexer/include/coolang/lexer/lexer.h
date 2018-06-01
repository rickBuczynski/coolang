#ifndef COOLANG_LEXER_LEXER_H_
#define COOLANG_LEXER_LEXER_H_

#include <fstream>
#include <optional>
#include <string>
#include <variant>
#include "coolang/lexer/char_stream.h"
#include "coolang/lexer/token.h"

class Lexer {
 public:
  Lexer(std::string input_file_name);

  void PopToken();
  std::optional<Token> PeekToken() { return cur_token_; };

 private:
  Token gettok();

  // returns nullopt unless EOF in comment
  std::optional<TokenError> AdvancePastEndOfComment();

  std::optional<Token> TokenForSingleCharSymbol(char c, int line_num);

  /// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the
  /// current token the parser is looking at.  getNextToken reads another token
  /// from the lexer and updates CurTok with its results.
  std::optional<Token> cur_token_;

  CharStream char_stream_;
};

#endif  // COOLANG_LEXER_LEXER_H_
