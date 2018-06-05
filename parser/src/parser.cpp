#include <variant>
#include "coolang/parser/parser.h"

using coolang::ast::AttributeFeature;
using coolang::ast::CoolClass;
using coolang::ast::Feature;
using coolang::ast::Formal;
using coolang::ast::MethodFeature;
using coolang::ast::Program;

Program Parser::ParseProgram() {
  std::vector<CoolClass> classes;
  while (!lexer_->PeekTokenTypeIs<TokenEndOfFile>()) {
    classes.push_back(ParseClass());
  }
  return Program(classes);
}

CoolClass Parser::ParseClass() {
  bool error_at_class_token = !lexer_->PeekTokenTypeIs<TokenClass>();
  lexer_->PopToken();

  bool error_at_type_id = !lexer_->PeekTokenTypeIs<TokenTypeId>();
  TokenTypeId type_id = std::get<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  bool error_at_lbrace = !lexer_->PeekTokenTypeIs<TokenLbrace>();

  std::vector<Feature> features;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    features.push_back(ParseFeature());
  }

  return CoolClass(type_id.get_data(), features);
}

Feature Parser::ParseFeature() {
  // TODO look ahead one symbol in lexer and decide to parse a method or an
  // attribute
  return ParseAttributeFeature();
}

MethodFeature Parser::ParseMethodFeature() {
  // TODO
  return MethodFeature();
}

AttributeFeature Parser::ParseAttributeFeature() {
  Formal f = ParseFormal();
  // TODO parse init expr if present
  return AttributeFeature(f);
}

Formal Parser::ParseFormal() {
  bool error_at_object_id = !lexer_->PeekTokenTypeIs<TokenObjectId>();
  TokenObjectId object_id = std::get<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  bool error_at_colon = !lexer_->PeekTokenTypeIs<TokenColon>();
  lexer_->PopToken();

  bool error_at_type_id = !lexer_->PeekTokenTypeIs<TokenTypeId>();
  TokenTypeId type_id = std::get<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  return Formal(object_id.get_data(), type_id.get_data());
}