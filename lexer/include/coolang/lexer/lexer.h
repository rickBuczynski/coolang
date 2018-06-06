#ifndef COOLANG_LEXER_LEXER_H_
#define COOLANG_LEXER_LEXER_H_

#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include "coolang/lexer/char_stream.h"
#include "coolang/lexer/token.h"

class Lexer {
 public:
  explicit Lexer(const std::string& input_file_path);

  std::filesystem::path GetInputFile() const { return input_file_; }

  void PopToken();
  Token PeekToken() const { return cur_token_; };

  template <class ExpectedToken>
  class PeekExpectingResult {
   public:
    PeekExpectingResult(Token token, int line_num)
        : token_(std::move(token)),
          line_num_(line_num),
          token_was_expected_(std::holds_alternative<ExpectedToken>(token)) {}

    Token GetToken() const { return token_; }
    int GetLineNum() const { return line_num_; }
    bool IsTokenWasExpected() const { return token_was_expected_; }

    ExpectedToken GetExpectedToken() const {
      return std::get<ExpectedToken>(token_);
    }

   private:
    const Token token_;
    const int line_num_;
    const bool token_was_expected_;
  };

  template <class ExpectedToken>
  PeekExpectingResult<ExpectedToken> PeekExpecting() const;

  template <class T>
  bool PeekTokenTypeIs() const {
    return std::holds_alternative<T>(PeekToken());
  }

 private:
  Token GetNextToken();

  // returns nullopt unless EOF in comment
  std::optional<TokenError> AdvancePastEndOfComment();

  std::optional<Token> TokenForSingleCharSymbol(char c, int line_num);

  CharStream char_stream_;
  // cur_token_ must be after char_stream_ since cur_token_ uses char_stream_
  // for initialization
  Token cur_token_;

  std::filesystem::path input_file_;
};

template <class ExpectedToken>
Lexer::PeekExpectingResult<ExpectedToken> Lexer::PeekExpecting() const {
  const int line_num = GetLineNum(PeekToken());
  return PeekExpectingResult<ExpectedToken>(PeekToken(), line_num);
}

#endif  // COOLANG_LEXER_LEXER_H_
