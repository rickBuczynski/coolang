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
  while (!lexer_->PeekTokenTypeIs<TokenEndOfFile>()) {
    classes.push_back(ParseClass());
  }

  const int end_line = !classes.empty()
                           ? classes.back().GetLineRange().end_line_num
                           : GetLineNum(lexer_->PeekToken());

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
  }

  bool error_at_rbrace = !lexer_->PeekTokenTypeIs<TokenRbrace>();
  lexer_->PopToken();

  bool error_at_semicolon = !lexer_->PeekTokenTypeIs<TokenSemi>();
  const int end_line = GetLineNum(lexer_->PeekToken());
  lexer_->PopToken();

  return CoolClass(type_id.get_data(), std::nullopt, features,
                   LineRange(start_line, end_line));
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
  return AttributeFeature(f.GetId(), f.GetType(), std::nullopt, f.GetLineRange());
}

Formal Parser::ParseFormal() const {
  bool error_at_object_id = !lexer_->PeekTokenTypeIs<TokenObjectId>();
  TokenObjectId object_id = std::get<TokenObjectId>(lexer_->PeekToken());
  const int start_line = GetLineNum(lexer_->PeekToken());
  lexer_->PopToken();

  bool error_at_colon = !lexer_->PeekTokenTypeIs<TokenColon>();
  lexer_->PopToken();

  bool error_at_type_id = !lexer_->PeekTokenTypeIs<TokenTypeId>();
  TokenTypeId type_id = std::get<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  // TODO this is part of the class production not formal
  bool error_at_semicolon = !lexer_->PeekTokenTypeIs<TokenSemi>();
  const int end_line = GetLineNum(lexer_->PeekToken());
  lexer_->PopToken();

  return Formal(object_id.get_data(), type_id.get_data(),
                LineRange(start_line, end_line));
}