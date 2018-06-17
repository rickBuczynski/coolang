#ifndef COOLANG_LEXER_LEXER_H_
#define COOLANG_LEXER_LEXER_H_

#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include "coolang/lexer/char_stream.h"
#include "coolang/lexer/token.h"

namespace coolang {

class Lexer {
 public:
  explicit Lexer(const std::string& input_file_path);

  std::filesystem::path GetInputFile() const { return input_file_; }

  void PopToken();
  Token PeekToken() const { return cur_token_; };
  Token LookAheadToken();

  template <class T>
  bool PeekTokenTypeIs() const {
    return std::holds_alternative<T>(PeekToken());
  }

  template <class T>
  void AdvanceToNext() {
    while (!PeekTokenTypeIs<T>()) {
      PopToken();
    }
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

  std::optional<Token> look_ahead_token_;
};

}  // namespace coolang

#endif  // COOLANG_LEXER_LEXER_H_
