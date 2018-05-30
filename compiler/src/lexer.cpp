#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include "coolang/compiler/lexer.h"
#include "coolang/compiler/token.h"

std::string lower_case(std::string word) {
  std::transform(word.begin(), word.end(), word.begin(), tolower);
  return word;
}

Lexer::Lexer(std::string input_file_name) : char_stream_(input_file_name) {}

Token Lexer::GetNextToken() { return CurTok = gettok(); }

/// gettok - Return the next token from standard input.
Token Lexer::gettok() {
  // Skip any whitespace.
  while (isspace(char_stream_.Peek())) {
    char_stream_.Pop();
  }

  if (isalpha(char_stream_.Peek())) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
    std::string identifier;

    identifier += char_stream_.Peek();
    char_stream_.Pop();

    while (isalnum((char_stream_.Peek()))) {
      identifier += char_stream_.Peek();
      char_stream_.Pop();
    }

    // keywords
    if (lower_case(identifier) == "class") {
      return Token(TokenType::CLASS, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "else") {
      return Token(TokenType::ELSE, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "fi") {
      return Token(TokenType::FI, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "if") {
      return Token(TokenType::IF, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "in") {
      return Token(TokenType::IN, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "inherits") {
      return Token(TokenType::INHERITS, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "isvoid") {
      return Token(TokenType::ISVOID, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "let") {
      return Token(TokenType::LET, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "loop") {
      return Token(TokenType::LOOP, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "pool") {
      return Token(TokenType::POOL, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "then") {
      return Token(TokenType::THEN, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "while") {
      return Token(TokenType::WHILE, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "case") {
      return Token(TokenType::CASE, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "esac") {
      return Token(TokenType::ESAC, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "new") {
      return Token(TokenType::NEW, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "of") {
      return Token(TokenType::OF, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "not") {
      return Token(TokenType::NOT, char_stream_.CurLineNum());
    }

    // bool consts are like keywords but must start with lower case
    if (lower_case(identifier) == "true" && identifier[0] == 't') {
      return Token(TokenType::BOOL_CONST, char_stream_.CurLineNum(), true);
    } else if (lower_case(identifier) == "false" && identifier[0] == 'f') {
      return Token(TokenType::BOOL_CONST, char_stream_.CurLineNum(), false);
    }

    if (isupper(identifier[0])) {
      return Token(TokenType::TYPEID, char_stream_.CurLineNum(), identifier);
    } else {
      return Token(TokenType::OBJECTID, char_stream_.CurLineNum(), identifier);
    }
  }

  if (isdigit(char_stream_.Peek())) {
    std::string int_digits;

    while (isdigit((char_stream_.Peek()))) {
      int_digits += char_stream_.Peek();
      char_stream_.Pop();
    }
    return Token(TokenType::INT_CONST, char_stream_.CurLineNum(),
                 std::stoi(int_digits));
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
      return Token(TokenType::ERROR, char_stream_.CurLineNum());
    } else if (cur_char == '-' && next_char == '-') {
      while (char_stream_.Peek() != EOF && char_stream_.Peek() != '\n') {
        char_stream_.Pop();
      }
      if (char_stream_.Peek() != EOF) return gettok();
    }

    return Token(TokenTypeForSingleCharSymbol(cur_char).value(),
                 char_stream_.CurLineNum());
  }

  // Check for end of file.  Don't eat the EOF.
  if (char_stream_.Peek() == EOF) {
    return Token(TokenType::END_OF_FILE, char_stream_.CurLineNum());
  }

  // TODO make sure I should return error here
  // Just return EOF for now to make it stop
  // Eventually should probably be an error
  return Token(TokenType::END_OF_FILE, char_stream_.CurLineNum());
}

std::optional<TokenType> Lexer::TokenTypeForSingleCharSymbol(char c) {
  switch (c) {
    case '+':
      return TokenType::PLUS;
    case '/':
      return TokenType::DIV;
    case '-':
      return TokenType::MINUS;
    case '*':
      return TokenType::MULT;
    case '=':
      return TokenType::EQ;
    case '<':
      return TokenType::LT;
    case '.':
      return TokenType::DOT;
    case '~':
      return TokenType::NEG;
    case ',':
      return TokenType::COMMA;
    case ';':
      return TokenType::SEMI;
    case ':':
      return TokenType::COLON;
    case '(':
      return TokenType::LPAREN;
    case ')':
      return TokenType::RPAREN;
    case '@':
      return TokenType::AT;
    case '{':
      return TokenType::LBRACE;
    case '}':
      return TokenType::RBRACE;
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
    } else if (prev_char == '*' && cur_char == ')' & chars_in_comment > 0) {
      open_comments--;
    }
    chars_in_comment++;
  }

  char_stream_.Pop();
}