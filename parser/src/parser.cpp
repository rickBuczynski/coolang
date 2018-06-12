#include <variant>
#include "coolang/lexer/token.h"
#include "coolang/parser/parser.h"

using coolang::ast::AddExpr;
using coolang::ast::AssignExpr;
using coolang::ast::AttributeFeature;
using coolang::ast::CoolClass;
using coolang::ast::Expr;
using coolang::ast::Feature;
using coolang::ast::Formal;
using coolang::ast::IntExpr;
using coolang::ast::LetExpr;
using coolang::ast::LineRange;
using coolang::ast::MethodFeature;
using coolang::ast::ObjectExpr;
using coolang::ast::Program;
using coolang::ast::BlockExpr;

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

    return Program(std::move(classes), LineRange(1, program_end_line));
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

  std::vector<std::unique_ptr<Feature>> features;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    features.push_back(ParseFeature());

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  return CoolClass(
      type_id_token.get_data(), std::nullopt, std::move(features),
      LineRange(class_token.get_line_num(), rbrace_token.get_line_num()),
      lexer_->GetInputFile().filename().string());
}

std::unique_ptr<Feature> Parser::ParseFeature() const {
  if (std::holds_alternative<TokenLparen>(lexer_->LookAheadToken())) {
    return ParseMethodFeature();
  }
  return ParseAttributeFeature();
}

std::unique_ptr<MethodFeature> Parser::ParseMethodFeature() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto left_paren_token = ExpectToken<TokenLparen>(lexer_->PeekToken());
  lexer_->PopToken();

  // TODO arg list

  auto right_paren_token = ExpectToken<TokenRparen>(lexer_->PeekToken());
  lexer_->PopToken();

  auto colon_token = ExpectToken<TokenColon>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto left_brace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<Expr> expr = ParseExpr();

  auto right_brace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  const LineRange line_range =
      LineRange(GetLineNum(object_id_token), GetLineNum(right_brace_token));

  return std::make_unique<MethodFeature>(object_id_token.get_data(),
                                         type_id_token.get_data(),
                                         std::move(expr), line_range);
}

std::unique_ptr<AttributeFeature> Parser::ParseAttributeFeature() const {
  Formal f = ParseFormal();
  // TODO parse init expr if present
  // need to change line range if theres an init expr
  return std::make_unique<AttributeFeature>(f.GetId(), f.GetType(),
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

std::unique_ptr<Expr> Parser::ParseExpr() const {
  std::unique_ptr<Expr> lhs_expr;

  if (std::holds_alternative<TokenIntConst>(lexer_->PeekToken())) {
    lhs_expr = ParseIntExpr();
  } else if (std::holds_alternative<TokenObjectId>(lexer_->PeekToken())) {
    if (std::holds_alternative<TokenAssign>(lexer_->LookAheadToken())) {
      lhs_expr = ParseAssignExpr();
    } else {
      lhs_expr = ParseObjectExpr();
    }
  } else if (std::holds_alternative<TokenLet>(lexer_->PeekToken())) {
    lhs_expr = ParseLetExpr();
  } else if (std::holds_alternative<TokenLbrace>(lexer_->PeekToken())) {
    lhs_expr = ParseBlockExpr();
  }

  // TODO will this work for expr + expr + expr
  // or do I need a loop to handle left recursion like in that article?
  if (std::holds_alternative<TokenPlus>(lexer_->PeekToken())) {
    auto plus_token = ExpectToken<TokenPlus>(lexer_->PeekToken());
    lexer_->PopToken();

    std::unique_ptr<Expr> rhs_expr = ParseExpr();

    LineRange line_range(lhs_expr->GetLineRange().start_line_num,
                         rhs_expr->GetLineRange().start_line_num);

    return std::make_unique<AddExpr>(line_range, std::move(lhs_expr),
                                     std::move(rhs_expr));
  }

  return lhs_expr;
}

std::unique_ptr<IntExpr> Parser::ParseIntExpr() const {
  auto int_const_token = ExpectToken<TokenIntConst>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(int_const_token), GetLineNum(int_const_token));

  return std::make_unique<IntExpr>(int_const_token.get_data(), line_range);
}

std::unique_ptr<LetExpr> Parser::ParseLetExpr() const {
  auto let_token = ExpectToken<TokenLet>(lexer_->PeekToken());
  lexer_->PopToken();

  Formal f = ParseFormal();

  auto token_in = ExpectToken<TokenIn>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<Expr> in_expr = ParseExpr();

  const auto line_range =
      LineRange(GetLineNum(let_token), in_expr->GetLineRange().end_line_num);

  // TODO init expr is empty default constructor
  return std::make_unique<LetExpr>(line_range, f.GetId(), f.GetType(),
                                   std::unique_ptr<Expr>(), std::move(in_expr));
}

std::unique_ptr<ObjectExpr> Parser::ParseObjectExpr() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(object_id_token), GetLineNum(object_id_token));

  return std::make_unique<ObjectExpr>(line_range, object_id_token.get_data());
}

std::unique_ptr<BlockExpr> Parser::ParseBlockExpr() const {
  auto lbrace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<std::unique_ptr<Expr>> exprs;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    exprs.push_back(ParseExpr());

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(lbrace_token), GetLineNum(rbrace_token));

  return std::make_unique<BlockExpr>(line_range, std::move(exprs));
}

std::unique_ptr<AssignExpr> Parser::ParseAssignExpr() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto assign_token = ExpectToken<TokenAssign>(lexer_->PeekToken());
  lexer_->PopToken();

  auto rhs_expr = ParseExpr();

  const auto line_range = LineRange(GetLineNum(object_id_token),
                                    rhs_expr->GetLineRange().end_line_num);

  return std::make_unique<AssignExpr>(object_id_token.get_data(),
                                      std::move(rhs_expr), line_range);
}
