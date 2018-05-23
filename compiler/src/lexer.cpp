#include <algorithm>
#include "coolang/compiler/lexer.h"
#include "coolang/compiler/token.h"

std::string lower_case(std::string word) {
  std::transform(word.begin(), word.end(), word.begin(), tolower);
  return word;
}

Lexer::Lexer(std::string input_file_name) {
  infile = fopen(input_file_name.c_str(), "r");
}

Lexer::~Lexer() { fclose(infile); }

Token Lexer::GetNextToken() { return CurTok = gettok(); }

/// gettok - Return the next token from standard input.
Token Lexer::gettok() {
  // Skip any whitespace.
  while (isspace(LastChar)) LastChar = get_next_char_in_file();

  std::locale loc;

  if (isalpha(LastChar)) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = get_next_char_in_file())))
      IdentifierStr += LastChar;

    if (lower_case(IdentifierStr) == "else") return Token(TokenType::ELSE);
    return Token(TokenType::ERROR);
  }

  if (isdigit(LastChar) || LastChar == '.') {  // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = get_next_char_in_file();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return Token(TokenType::INT_CONST, (int)NumVal);
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = get_next_char_in_file();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF) return gettok();
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF) return Token(TokenType::END_OF_FILE);

  return Token(TokenType::ERROR);
}

int Lexer::get_next_char_in_file() { return getc(infile); }
