#include <algorithm>
#include <fstream>
#include <iostream>
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

    while (isalnum((char_stream_.Pop()))) {
      identifier += char_stream_.Peek();
    }

    // keywords
    if (lower_case(identifier) == "class") {
      return Token(TokenType::CLASS, char_stream_.CurLineNum());
    } else if (lower_case(identifier) == "else") {
      return Token(TokenType::ELSE, char_stream_.CurLineNum());
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

  if (char_stream_.Peek() == '(') {
    if (char_stream_.Pop() == '*') {
      AdvanceToEndOfComment();
      return gettok();
    }
  }

  // Check for end of file.  Don't eat the EOF.
  if (char_stream_.Peek() == EOF) {
    return Token(TokenType::END_OF_FILE, char_stream_.CurLineNum());
  }

  // TODO decide if this should stop the lexer or keep going
  return Token(TokenType::ERROR, char_stream_.CurLineNum());
}

void Lexer::AdvanceToEndOfComment() {
  int open_comments = 1;
  // prevents pattern (*) from closing comment it need to be (**)
  int chars_in_comment = 0;
  while (open_comments > 0) {
    int prev_char = char_stream_.Peek();
    int cur_char = char_stream_.Pop();

    if (prev_char == '(' && cur_char == '*') {
      open_comments++;
    } else if (prev_char == '*' && cur_char == ')' & chars_in_comment > 0) {
      open_comments--;
    }
    chars_in_comment++;
  }

  char_stream_.Pop();
}