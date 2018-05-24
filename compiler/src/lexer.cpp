#include <algorithm>
#include <fstream>
#include "coolang/compiler/lexer.h"
#include "coolang/compiler/token.h"

std::string lower_case(std::string word) {
  std::transform(word.begin(), word.end(), word.begin(), tolower);
  return word;
}

Lexer::Lexer(std::string input_file_name) : infile(input_file_name) {}

Token Lexer::GetNextToken() { return CurTok = gettok(); }

/// gettok - Return the next token from standard input.
Token Lexer::gettok() {
  // Skip any whitespace.
  while (isspace(LastChar)) LastChar = get_next_char_in_file();

  if (isalpha(LastChar)) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
    std::string identifier;
    identifier += LastChar;

    while (isalnum((LastChar = get_next_char_in_file()))) {
      identifier += LastChar;
    }

    // keywords
    if (lower_case(identifier) == "class") {
      return Token(TokenType::CLASS, cur_line_num);
    } else if (lower_case(identifier) == "else") {
      return Token(TokenType::ELSE, cur_line_num);
    }

    // bool consts are like keywords but must start with lower case
    if (lower_case(identifier) == "true" && identifier[0] == 't') {
      return Token(TokenType::BOOL_CONST, cur_line_num, true);
    } else if (lower_case(identifier) == "false" && identifier[0] == 'f') {
      return Token(TokenType::BOOL_CONST, cur_line_num, false);
    }

    if (isupper(identifier[0])) {
      return Token(TokenType::TYPEID, cur_line_num, identifier);
    } else {
      return Token(TokenType::OBJECTID, cur_line_num, identifier);
    }
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
  if (LastChar == EOF) return Token(TokenType::END_OF_FILE, cur_line_num);

  return Token(TokenType::ERROR, cur_line_num);
}

int Lexer::get_next_char_in_file() {
  if (cur_char_of_line >= cur_line.length()) {
    if (infile.eof()) return EOF;
    std::getline(infile, cur_line);

    cur_line += '\n';
    cur_line_num++;
    cur_char_of_line = 0;
  }

  return cur_line[cur_char_of_line++];
}
