#include "coolang/compiler/lexer.h"

Lexer::Lexer(std::string input_file_name) {
  infile = fopen(input_file_name.c_str(), "r");
}

Lexer::~Lexer() { fclose(infile); }

int Lexer::GetNextToken() { return CurTok = gettok(); }

/// gettok - Return the next token from standard input.
int Lexer::gettok() {
  // Skip any whitespace.
  while (isspace(LastChar)) LastChar = get_next_char_in_file();

  if (isalpha(LastChar)) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = get_next_char_in_file())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def") return tok_def;
    if (IdentifierStr == "extern") return tok_extern;
    if (IdentifierStr == "if") return tok_if;
    if (IdentifierStr == "then") return tok_then;
    if (IdentifierStr == "else") return tok_else;
    if (IdentifierStr == "for") return tok_for;
    if (IdentifierStr == "in") return tok_in;
    if (IdentifierStr == "binary") return tok_binary;
    if (IdentifierStr == "unary") return tok_unary;
    if (IdentifierStr == "var") return tok_var;
    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') {  // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = get_next_char_in_file();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = get_next_char_in_file();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF) return gettok();
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF) return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = get_next_char_in_file();
  return ThisChar;
}

int Lexer::get_next_char_in_file() { return getc(infile); }
