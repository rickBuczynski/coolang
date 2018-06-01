#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include "coolang/lexer/lexer.h"
#include "coolang/lexer/token.h"

std::string lower_case(std::string word) {
  std::transform(word.begin(), word.end(), word.begin(), tolower);
  return word;
}

Lexer::Lexer(std::string input_file_name) : char_stream_(input_file_name) {}

void Lexer::PopToken() { cur_token_.emplace(gettok()); }

/// gettok - Return the next token from standard input.
Token Lexer::gettok() {
  // Skip any whitespace.
  while (isspace(char_stream_.Peek())) {
    char_stream_.Pop();
  }

  if (isalpha(char_stream_.Peek())) {
    std::string identifier;

    identifier += char_stream_.Peek();
    char_stream_.Pop();

    while (isalnum((char_stream_.Peek())) || char_stream_.Peek() == '_') {
      identifier += char_stream_.Peek();
      char_stream_.Pop();
    }

    // keywords
    if (lower_case(identifier) == "class") {
      return TokenClass(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "else") {
      return TokenElse(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "fi") {
      return TokenFi(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "if") {
      return TokenIf(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "in") {
      return TokenIn(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "inherits") {
      return TokenInherits(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "isvoid") {
      return TokenIsVoid(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "let") {
      return TokenLet(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "loop") {
      return TokenLoop(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "pool") {
      return TokenPool(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "then") {
      return TokenThen(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "while") {
      return TokenWhile(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "case") {
      return TokenCase(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "esac") {
      return TokenEsac(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "new") {
      return TokenNew(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "of") {
      return TokenOf(char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "not") {
      return TokenNot(char_stream_.CurLineNum());
    }

    // bool consts are like keywords but must start with lower case
    if (lower_case(identifier) == "true" && identifier[0] == 't') {
      return TokenBoolConst(true, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "false" && identifier[0] == 'f') {
      return TokenBoolConst(false, char_stream_.CurLineNum());
    }

    if (isupper(identifier[0])) {
      return TokenTypeId(identifier, char_stream_.CurLineNum());
    } else {
      return TokenObjectId(identifier, char_stream_.CurLineNum());
    }
  }

  if (isdigit(char_stream_.Peek())) {
    std::string int_digits;

    while (isdigit((char_stream_.Peek()))) {
      int_digits += char_stream_.Peek();
      char_stream_.Pop();
    }
    return TokenIntConst(int_digits, char_stream_.CurLineNum());
  }

  if (TokenForSingleCharSymbol(char_stream_.Peek(), char_stream_.CurLineNum())
          .has_value()) {
    char cur_char = char_stream_.Peek();
    char_stream_.Pop();
    char next_char = char_stream_.Peek();

    if (cur_char == '(' && next_char == '*') {
      std::optional<TokenError> maybe_error_token = AdvancePastEndOfComment();
      if (maybe_error_token.has_value()) {
        return maybe_error_token.value();
      } else {
        return gettok();
      }
    } else if (cur_char == '*' && next_char == ')') {
      // end comment token *) outside of comment, this is an error
      char_stream_.Pop();
      std::string err_message = "Unmatched *)";
      return TokenError(err_message, char_stream_.CurLineNum());
    } else if (cur_char == '-' && next_char == '-') {
      while (char_stream_.Peek() != EOF && char_stream_.Peek() != '\n') {
        char_stream_.Pop();
      }
      if (char_stream_.Peek() != EOF) return gettok();
    } else if (cur_char == '<' && next_char == '-') {
      char_stream_.Pop();
      return TokenAssign(char_stream_.CurLineNum());
    } else if (cur_char == '<' && next_char == '=') {
      char_stream_.Pop();
      return TokenLe(char_stream_.CurLineNum());
    } else if (cur_char == '=' && next_char == '>') {
      char_stream_.Pop();
      return TokenDarrow(char_stream_.CurLineNum());
    }

    return TokenForSingleCharSymbol(cur_char, char_stream_.CurLineNum())
        .value();
  }

  if (char_stream_.Peek() == '"') {
    std::string str_const;
    char_stream_.Pop();

    while (char_stream_.Peek() != '"') {
      if (char_stream_.Peek() == '\\') {  // escape chars
        char_stream_.Pop();

        if (char_stream_.Peek() == 'b') {
          str_const += '\b';
        } else if (char_stream_.Peek() == 't') {
          str_const += '\t';
        } else if (char_stream_.Peek() == 'n') {
          str_const += '\n';
        } else if (char_stream_.Peek() == 'f') {
          str_const += '\f';
        } else if (char_stream_.Peek() == 0) {
          TokenError err = TokenError("String contains escaped null character.",
                                      char_stream_.CurLineNum());
          AdvancePastEndOfString();
          return err;
        } else if (char_stream_.Peek() == EOF) {
          return TokenError("EOF in string constant",
                            char_stream_.CurLineNum());
        } else {
          str_const += char_stream_.Peek();
        }
        char_stream_.Pop();

      } else {
        if (char_stream_.Peek() == '\n') {
          char_stream_.Pop();
          return TokenError("Unterminated string constant",
                            char_stream_.CurLineNum());
        } else if (char_stream_.Peek() == 0) {
          TokenError err = TokenError("String contains null character.",
                                      char_stream_.CurLineNum());
          AdvancePastEndOfString();
          return err;
        } else if (char_stream_.Peek() == EOF) {
          return TokenError("EOF in string constant",
                            char_stream_.CurLineNum());
        } else {
          str_const += char_stream_.Peek();
          char_stream_.Pop();
        }
      }
    }
    char_stream_.Pop();

    if (str_const.length() > TokenStrConst::MAX_STR_CONST_LEN) {
      return TokenError("String constant too long", char_stream_.CurLineNum());
    } else {
      return TokenStrConst(str_const, char_stream_.CurLineNum());
    }
  }

  // Check for end of file.  Don't eat the EOF.
  if (char_stream_.Peek() == EOF) {
    return TokenEndOfFile(char_stream_.CurLineNum());
  }

  std::string err_message;
  err_message += char_stream_.Peek();
  char_stream_.Pop();
  return TokenError(err_message, char_stream_.CurLineNum());
}

void Lexer::AdvancePastEndOfString() {
  // TODO test EOF in str after other error like null in str or \n in str
  // TODO test escaped newline after null
  while (char_stream_.Peek() != '"' && char_stream_.Peek() != '\n') {
    char_stream_.Pop();
  }
  char_stream_.Pop();
}

std::optional<Token> Lexer::TokenForSingleCharSymbol(char c, int line_num) {
  switch (c) {
    case '+':
      return TokenPlus(line_num);
    case '/':
      return TokenDiv(line_num);
    case '-':
      return TokenMinus(line_num);
    case '*':
      return TokenMult(line_num);
    case '=':
      return TokenEq(line_num);
    case '<':
      return TokenLt(line_num);
    case '.':
      return TokenDot(line_num);
    case '~':
      return TokenNeg(line_num);
    case ',':
      return TokenComma(line_num);
    case ';':
      return TokenSemi(line_num);
    case ':':
      return TokenColon(line_num);
    case '(':
      return TokenLparen(line_num);
    case ')':
      return TokenRparen(line_num);
    case '@':
      return TokenAt(line_num);
    case '{':
      return TokenLbrace(line_num);
    case '}':
      return TokenRbrace(line_num);
    default:
      return std::nullopt;
  }
}

std::optional<TokenError> Lexer::AdvancePastEndOfComment() {
  int open_comments = 1;
  // prevents pattern (*) from closing comment it need to be (**)
  int chars_in_comment = 0;
  while (open_comments > 0) {
    char prev_char = char_stream_.Peek();
    char_stream_.Pop();
    char cur_char = char_stream_.Peek();

    if (char_stream_.Peek() == EOF) {
      return TokenError("EOF in comment", char_stream_.CurLineNum());
    }

    if (prev_char == '(' && cur_char == '*') {
      open_comments++;
    } else if (prev_char == '*' && cur_char == ')' && chars_in_comment > 0) {
      open_comments--;
    }
    chars_in_comment++;
  }

  char_stream_.Pop();
  return std::nullopt;
}