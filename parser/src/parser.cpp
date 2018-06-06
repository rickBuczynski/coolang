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

Program Parser::ParseProgram() const {
  std::vector<CoolClass> classes;

  int end_line = 1;
  while (!lexer_->PeekTokenTypeIs<TokenEndOfFile>()) {
    classes.push_back(ParseClass());

    bool error_at_semicolon = !lexer_->PeekTokenTypeIs<TokenSemi>();
    end_line = GetLineNum(lexer_->PeekToken());
    lexer_->PopToken();
  }

  if (classes.empty()) {
    end_line = GetLineNum(lexer_->PeekToken());
  }

  return Program(classes, LineRange(1, end_line));
}

CoolClass Parser::ParseClass() const {
  bool error_at_class_token = !lexer_->PeekTokenTypeIs<TokenClass>();
  const int start_line = GetLineNum(lexer_->PeekToken());
  lexer_->PopToken();

  bool error_at_type_id = !lexer_->PeekTokenTypeIs<TokenTypeId>();
  TokenTypeId type_id = std::get<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  bool error_at_lbrace = !lexer_->PeekTokenTypeIs<TokenLbrace>();
  lexer_->PopToken();

  std::vector<Feature> features;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    features.push_back(ParseFeature());

    bool error_at_semicolon = !lexer_->PeekTokenTypeIs<TokenSemi>();
    const int end_line = GetLineNum(lexer_->PeekToken());
    lexer_->PopToken();
  }

  bool error_at_rbrace = !lexer_->PeekTokenTypeIs<TokenRbrace>();
  const int end_line = GetLineNum(lexer_->PeekToken());
  lexer_->PopToken();

  return CoolClass(type_id.get_data(), std::nullopt, features,
                   LineRange(start_line, end_line),
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
  TokenObjectId object_id_token = std::get<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  TokenColon colon_token = std::get<TokenColon>(lexer_->PeekToken());
  lexer_->PopToken();

  TokenTypeId type_id_token = std::get<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  return Formal(
      object_id_token.get_data(),
      type_id_token.get_data(),
      LineRange(GetLineNum(object_id_token), GetLineNum(type_id_token)));
}
