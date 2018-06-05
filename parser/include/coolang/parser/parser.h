#ifndef COOLANG_PARSER_PARSER_H_
#define COOLANG_PARSER_PARSER_H_

#include "coolang/lexer/lexer.h"
#include "coolang/parser/ast.h"

class Parser {
 public:
  Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)){};
  coolang::ast::Program Parser::ParseProgram();

 private:
  coolang::ast::CoolClass ParseClass();
  coolang::ast::Feature ParseFeature();
  coolang::ast::MethodFeature ParseMethodFeature();
  coolang::ast::AttributeFeature ParseAttributeFeature();
  coolang::ast::Formal ParseFormal();

  std::unique_ptr<Lexer> lexer_;
};

#endif  // COOLANG_PARSER_PARSER_H_
