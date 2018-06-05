#ifndef COOLANG_PARSER_PARSER_H_
#define COOLANG_PARSER_PARSER_H_

#include "coolang/lexer/lexer.h"

class Parser {
 public:
  Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)){};

 private:
  std::unique_ptr<Lexer> lexer_;
};

#endif  // COOLANG_PARSER_PARSER_H_
