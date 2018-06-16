#ifndef COOLANG_PARSER_PARSER_H_
#define COOLANG_PARSER_PARSER_H_

#include "coolang/lexer/lexer.h"
#include "coolang/lexer/token.h"
#include "coolang/parser/ast.h"

namespace coolang {

class ParseError {
 public:
  ParseError(Token unexpected_token, std::string file_name)
      : unexpected_token_(std::move(unexpected_token)),
        file_name_(std::move(file_name)) {}

  std::string ToString(int indent_depth) const {
    // TODO either use indent_depth or refactor to get rid of it as unneeded

    using namespace std::string_literals;
    return ""s + '"' + file_name_ + '"' + ", line " +
           std::to_string(GetLineNum(unexpected_token_)) +
           ": syntax error at or near " +
           TokenTypeSpecificStr(unexpected_token_, " = ") + '\n' +
           "Compilation halted due to lex and parse errors\n";
  }

 private:
  const Token unexpected_token_;
  const std::string file_name_;
};

class Parser {
 public:
  explicit Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)){};
  std::variant<ProgramAst, ParseError> ParseProgram() const;

 private:
  ClassAst ParseClass() const;

  std::unique_ptr<Feature> ParseFeature() const;
  std::unique_ptr<MethodFeature> ParseMethodFeature() const;
  std::unique_ptr<AttributeFeature> ParseAttributeFeature() const;

  Formal ParseFormal() const;

  // TODO maybe misspelled precedence
  std::unique_ptr<Expr> ParseExpr(int min_precedence) const;
  std::unique_ptr<IfExpr> ParseIfExpr() const;
  std::unique_ptr<NegExpr> ParseNegExpr() const;
  std::unique_ptr<AssignExpr> ParseAssignExpr() const;
  std::unique_ptr<IntExpr> ParseIntExpr() const;
  std::unique_ptr<BoolExpr> ParseBoolExpr() const;
  std::unique_ptr<StrExpr> ParseStrExpr() const;
  std::unique_ptr<LetExpr> ParseLetExpr() const;
  std::unique_ptr<ObjectExpr> ParseObjectExpr() const;
  std::unique_ptr<BlockExpr> ParseBlockExpr() const;

  std::unique_ptr<Lexer> lexer_;
};

}  // namespace coolang

#endif  // COOLANG_PARSER_PARSER_H_
