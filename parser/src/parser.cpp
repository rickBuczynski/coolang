#include <variant>
#include "coolang/lexer/token.h"
#include "coolang/parser/parser.h"

using coolang::ast::AttributeFeature;
using coolang::ast::CoolClass;
using coolang::ast::Feature;
using coolang::ast::Formal;
using coolang::ast::LineRange;
using coolang::ast::MethodFeature;
using coolang::ast::Program;

class UnexpectedTokenExcpetion : public std::exception {
 public:
  explicit UnexpectedTokenExcpetion(Token unexpected_token)
      : unexpected_token_(std::move(unexpected_token)) {}

  Token GetUnexpectedToken() const { return unexpected_token_; }

 private:
  const Token unexpected_token_;
};

template <class T>
T ExpectToken(Token token) {
  try {
    return std::get<T>(token);
  } catch (std::bad_variant_access&) {
    throw UnexpectedTokenExcpetion(token);
  }
}

std::variant<Program, ParseError> Parser::ParseProgram() const {
  try {
    std::vector<CoolClass> classes;

    while (!lexer_->PeekTokenTypeIs<TokenEndOfFile>()) {
      classes.push_back(ParseClass());

      auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
      lexer_->PopToken();
    }

    auto eof_token = ExpectToken<TokenEndOfFile>(lexer_->PeekToken());

    const int program_end_line =
        classes.empty() ? eof_token.get_line_num()
                        : classes.back().GetLineRange().end_line_num;

    return Program(classes, LineRange(1, program_end_line));
  } catch (const UnexpectedTokenExcpetion& e) {
    return ParseError(e.GetUnexpectedToken(),
                      lexer_->GetInputFile().filename().string());
  }
}

CoolClass Parser::ParseClass() const {
  auto class_token = ExpectToken<TokenClass>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto lbrace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<Feature> features;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    features.push_back(ParseFeature());

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  return CoolClass(
      type_id_token.get_data(), std::nullopt, features,
      LineRange(class_token.get_line_num(), rbrace_token.get_line_num()),
      lexer_->GetInputFile().filename().string());
}

Feature Parser::ParseFeature() const {
  // TODO look ahead one symbol in lexer and decide to parse a method or an
  // attribute
  return ParseAttributeFeature();
}

MethodFeature Parser::ParseMethodFeature() const {
  // TODO
  return MethodFeature();
}

AttributeFeature Parser::ParseAttributeFeature() const {
  Formal f = ParseFormal();
  // TODO parse init expr if present
  // need to change line range if theres an init expr
  return AttributeFeature(f.GetId(), f.GetType(), std::nullopt,
                          f.GetLineRange());
}

Formal Parser::ParseFormal() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto colon_token = ExpectToken<TokenColon>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  return Formal(
      object_id_token.get_data(), type_id_token.get_data(),
      LineRange(GetLineNum(object_id_token), GetLineNum(type_id_token)));
}
