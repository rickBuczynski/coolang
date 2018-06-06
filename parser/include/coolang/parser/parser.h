#ifndef COOLANG_PARSER_PARSER_H_
#define COOLANG_PARSER_PARSER_H_

#include "coolang/lexer/lexer.h"
#include "coolang/parser/ast.h"

class Parser {
 public:
  Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)){};
  coolang::ast::Program Parser::ParseProgram() const;

 private:
  coolang::ast::CoolClass ParseClass() const;
  coolang::ast::Feature ParseFeature() const;
  coolang::ast::MethodFeature ParseMethodFeature() const;
  coolang::ast::AttributeFeature ParseAttributeFeature() const;
  coolang::ast::Formal ParseFormal() const;

  std::unique_ptr<Lexer> lexer_;
};

#endif  // COOLANG_PARSER_PARSER_H_
