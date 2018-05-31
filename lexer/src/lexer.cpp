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
      return Token(TokenTypeClass(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "else") {
      return Token(TokenTypeElse(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "fi") {
      return Token(TokenTypeFi(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "if") {
      return Token(TokenTypeIf(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "in") {
      return Token(TokenTypeIn(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "inherits") {
      return Token(TokenTypeInherits(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "isvoid") {
      return Token(TokenTypeIsVoid(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "let") {
      return Token(TokenTypeLet(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "loop") {
      return Token(TokenTypeLoop(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "pool") {
      return Token(TokenTypePool(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "then") {
      return Token(TokenTypeThen(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "while") {
      return Token(TokenTypeWhile(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "case") {
      return Token(TokenTypeCase(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "esac") {
      return Token(TokenTypeEsac(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "new") {
      return Token(TokenTypeNew(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "of") {
      return Token(TokenTypeOf(), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "not") {
      return Token(TokenTypeNot(), char_stream_.CurLineNum());
    }

    // bool consts are like keywords but must start with lower case
    if (lower_case(identifier) == "true" && identifier[0] == 't') {
      return Token(TokenTypeBoolConst(true), char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "false" && identifier[0] == 'f') {
      return Token(TokenTypeBoolConst(false), char_stream_.CurLineNum());
    }

    if (isupper(identifier[0])) {
      return Token(TokenTypeTypeId(identifier), char_stream_.CurLineNum());
    } else {
      return Token(TokenTypeObjectId(identifier), char_stream_.CurLineNum());
    }
  }

  if (isdigit(char_stream_.Peek())) {
    std::string int_digits;

    while (isdigit((char_stream_.Peek()))) {
      int_digits += char_stream_.Peek();
      char_stream_.Pop();
    }
    return Token(TokenTypeIntConst(std::stoi(int_digits)),
                 char_stream_.CurLineNum());
  }

  if (TokenTypeForSingleCharSymbol(char_stream_.Peek()).has_value()) {
    char cur_char = char_stream_.Peek();
    char_stream_.Pop();
    char next_char = char_stream_.Peek();

    if (cur_char == '(' && next_char == '*') {
      AdvanceToEndOfComment();
      return gettok();
    } else if (cur_char == '*' && next_char == ')') {
      // end comment token *) outside of comment, this is an error
      char_stream_.Pop();
      std::string err_message = "Unmatched *)";
      return Token(TokenTypeError(err_message), char_stream_.CurLineNum());
    } else if (cur_char == '-' && next_char == '-') {
      while (char_stream_.Peek() != EOF && char_stream_.Peek() != '\n') {
        char_stream_.Pop();
      }
      if (char_stream_.Peek() != EOF) return gettok();
    } else if (cur_char == '<' && next_char == '-') {
      char_stream_.Pop();
      return Token(TokenTypeAssign(), char_stream_.CurLineNum());
    } else if (cur_char == '<' && next_char == '=') {
      char_stream_.Pop();
      return Token(TokenTypeLe(), char_stream_.CurLineNum());
    } else if (cur_char == '=' && next_char == '>') {
      char_stream_.Pop();
      return Token(TokenTypeDarrow(), char_stream_.CurLineNum());
    }

    return Token(TokenTypeForSingleCharSymbol(cur_char).value(),
                 char_stream_.CurLineNum());
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
        } else if (char_stream_.Peek() == EOF) {
          return Token(TokenTypeError("EOF in string constant"),
                       char_stream_.CurLineNum());
        } else {
          str_const += char_stream_.Peek();
        }
        char_stream_.Pop();

      } else {
        if (char_stream_.Peek() == '\n') {
          char_stream_.Pop();
          return Token(TokenTypeError("Unterminated string constant"),
                       char_stream_.CurLineNum());
        } else if (char_stream_.Peek() == EOF) {
          return Token(TokenTypeError("EOF in string constant"),
                       char_stream_.CurLineNum());
        } else {
          str_const += char_stream_.Peek();
          char_stream_.Pop();
        }
      }
    }
    char_stream_.Pop();

    return Token(TokenTypeStrConst(str_const), char_stream_.CurLineNum());
  }

  // Check for end of file.  Don't eat the EOF.
  if (char_stream_.Peek() == EOF) {
    return Token(TokenTypeEndOfFile(), char_stream_.CurLineNum());
  }

  std::string err_message;
  err_message += char_stream_.Peek();
  char_stream_.Pop();
  return Token(TokenTypeError(err_message), char_stream_.CurLineNum());
}

std::optional<TokenType> Lexer::TokenTypeForSingleCharSymbol(char c) {
  switch (c) {
    case '+':
      return TokenTypePlus();
    case '/':
      return TokenTypeDiv();
    case '-':
      return TokenTypeMinus();
    case '*':
      return TokenTypeMult();
    case '=':
      return TokenTypeEq();
    case '<':
      return TokenTypeLt();
    case '.':
      return TokenTypeDot();
    case '~':
      return TokenTypeNeg();
    case ',':
      return TokenTypeComma();
    case ';':
      return TokenTypeSemi();
    case ':':
      return TokenTypeColon();
    case '(':
      return TokenTypeLparen();
    case ')':
      return TokenTypeRparen();
    case '@':
      return TokenTypeAt();
    case '{':
      return TokenTypeLbrace();
    case '}':
      return TokenTypeRbrace();
    default:
      return std::nullopt;
  }
}

void Lexer::AdvanceToEndOfComment() {
  int open_comments = 1;
  // prevents pattern (*) from closing comment it need to be (**)
  int chars_in_comment = 0;
  while (open_comments > 0) {
    char prev_char = char_stream_.Peek();
    char_stream_.Pop();
    char cur_char = char_stream_.Peek();

    if (prev_char == '(' && cur_char == '*') {
      open_comments++;
    } else if (prev_char == '*' && cur_char == ')' && chars_in_comment > 0) {
      open_comments--;
    }
    chars_in_comment++;
  }

  char_stream_.Pop();
}