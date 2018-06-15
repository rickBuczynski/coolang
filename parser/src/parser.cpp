#include <variant>
#include "coolang/lexer/token.h"
#include "coolang/parser/parser.h"

namespace coolang {

int TokenBinOpPrecidence(const Token& token) {
  return std::visit(
      [](auto&& arg) -> int {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, TokenDiv>) {
          return 3;
        } else if constexpr (std::is_same_v<T, TokenMult>) {
          return 3;
        } else if constexpr (std::is_same_v<T, TokenPlus>) {
          return 2;
        } else if constexpr (std::is_same_v<T, TokenMinus>) {
          return 2;
        } else if constexpr (std::is_same_v<T, TokenEq>) {
          return 1;
        } else if constexpr (std::is_same_v<T, TokenLt>) {
          return 1;
        } else if constexpr (std::is_same_v<T, TokenLe>) {
          return 1;
        } else {
          throw std::invalid_argument(TokenToString(arg) +
                                      " is not a binary operator");
        }
      },
      token);
}

bool TokenIsBinOp(const Token& token) {
  return std::visit(
      [](auto&& arg) -> bool {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, TokenPlus>) {
          return true;
        } else if constexpr (std::is_same_v<T, TokenDiv>) {
          return true;
        } else if constexpr (std::is_same_v<T, TokenMinus>) {
          return true;
        } else if constexpr (std::is_same_v<T, TokenMult>) {
          return true;
        } else if constexpr (std::is_same_v<T, TokenEq>) {
          return true;
        } else if constexpr (std::is_same_v<T, TokenLt>) {
          return true;
        } else if constexpr (std::is_same_v<T, TokenLe>) {
          return true;
        } else {
          return false;
        }
      },
      token);
}

std::unique_ptr<coolang::ast::BinOpExpr> BinOpExprForToken(
    const Token& token, ast::LineRange line_range,
    std::unique_ptr<ast::Expr> lhs_expr, std::unique_ptr<ast::Expr> rhs_expr) {
  return std::visit(
      [line_range, &lhs_expr,
       &rhs_expr](auto&& arg) -> std::unique_ptr<coolang::ast::BinOpExpr> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, TokenDiv>) {
          return std::make_unique<coolang::ast::DivideExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenMult>) {
          return std::make_unique<coolang::ast::MultiplyExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenPlus>) {
          return std::make_unique<coolang::ast::AddExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenMinus>) {
          return std::make_unique<coolang::ast::SubtractExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenEq>) {
          return std::make_unique<coolang::ast::EqCompareExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenLt>) {
          return std::make_unique<coolang::ast::LessThanCompareExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenLe>) {
          return std::make_unique<coolang::ast::LessThanEqualCompareExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else {
          throw std::invalid_argument(TokenToString(arg) +
                                      " is not a binary operator");
        }
      },
      token);
}

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

std::variant<ast::Program, ParseError> Parser::ParseProgram() const {
  try {
    std::vector<ast::CoolClass> classes;

    while (!lexer_->PeekTokenTypeIs<TokenEndOfFile>()) {
      classes.push_back(ParseClass());

      auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
      lexer_->PopToken();
    }

    auto eof_token = ExpectToken<TokenEndOfFile>(lexer_->PeekToken());

    const int program_end_line =
        classes.empty() ? eof_token.get_line_num()
                        : classes.back().GetLineRange().end_line_num;

    return ast::Program(std::move(classes),
                        ast::LineRange(1, program_end_line));
  } catch (const UnexpectedTokenExcpetion& e) {
    return ParseError(e.GetUnexpectedToken(),
                      lexer_->GetInputFile().filename().string());
  }
}

ast::CoolClass Parser::ParseClass() const {
  auto class_token = ExpectToken<TokenClass>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto lbrace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<std::unique_ptr<ast::Feature>> features;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    features.push_back(ParseFeature());

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  return ast::CoolClass(
      type_id_token.get_data(), std::nullopt, std::move(features),
      ast::LineRange(class_token.get_line_num(), rbrace_token.get_line_num()),
      lexer_->GetInputFile().filename().string());
}

std::unique_ptr<ast::Feature> Parser::ParseFeature() const {
  if (std::holds_alternative<TokenLparen>(lexer_->LookAheadToken())) {
    return ParseMethodFeature();
  }
  return ParseAttributeFeature();
}

std::unique_ptr<ast::MethodFeature> Parser::ParseMethodFeature() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto left_paren_token = ExpectToken<TokenLparen>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<ast::Formal> args;
  while (!lexer_->PeekTokenTypeIs<TokenRparen>()) {
    args.push_back(ParseFormal());

    if (lexer_->PeekTokenTypeIs<TokenComma>()) lexer_->PopToken();
  }

  auto right_paren_token = ExpectToken<TokenRparen>(lexer_->PeekToken());
  lexer_->PopToken();

  auto colon_token = ExpectToken<TokenColon>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto left_brace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<ast::Expr> expr = ParseExpr(0);

  auto right_brace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  const ast::LineRange line_range = ast::LineRange(
      GetLineNum(object_id_token), GetLineNum(right_brace_token));

  return std::make_unique<ast::MethodFeature>(
      line_range, object_id_token.get_data(), args, type_id_token.get_data(),
      std::move(expr));
}

std::unique_ptr<ast::AttributeFeature> Parser::ParseAttributeFeature() const {
  ast::Formal f = ParseFormal();

  std::unique_ptr<ast::Expr> initialization_expr;
  if (lexer_->PeekTokenTypeIs<TokenAssign>()) {
    lexer_->PopToken();
    initialization_expr = ParseExpr(0);
  }

  // need to change line range if theres an init expr
  return std::make_unique<ast::AttributeFeature>(
      f.GetLineRange(), f.GetId(), f.GetType(), std::move(initialization_expr));
}

ast::Formal Parser::ParseFormal() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto colon_token = ExpectToken<TokenColon>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  return ast::Formal(
      object_id_token.get_data(), type_id_token.get_data(),
      ast::LineRange(GetLineNum(object_id_token), GetLineNum(type_id_token)));
}

std::unique_ptr<ast::Expr> Parser::ParseExpr(int min_precidence) const {
  std::unique_ptr<ast::Expr> lhs_expr;

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

  while (TokenIsBinOp(lexer_->PeekToken()) &&
         TokenBinOpPrecidence(lexer_->PeekToken()) >= min_precidence) {
    Token binop_token = lexer_->PeekToken();
    lexer_->PopToken();

    std::unique_ptr<ast::Expr> rhs_expr =
        ParseExpr(TokenBinOpPrecidence(binop_token) + 1);

    const ast::LineRange line_range(lhs_expr->GetLineRange().start_line_num,
                                    rhs_expr->GetLineRange().end_line_num);

    lhs_expr = BinOpExprForToken(binop_token, line_range, std::move(lhs_expr),
                                 std::move(rhs_expr));
  }

  if (lhs_expr) {
    return lhs_expr;
  }
  throw UnexpectedTokenExcpetion(lexer_->PeekToken());
}

std::unique_ptr<ast::IntExpr> Parser::ParseIntExpr() const {
  auto int_const_token = ExpectToken<TokenIntConst>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      ast::LineRange(GetLineNum(int_const_token), GetLineNum(int_const_token));

  return std::make_unique<ast::IntExpr>(int_const_token.get_data(), line_range);
}

std::unique_ptr<ast::LetExpr> Parser::ParseLetExpr() const {
  auto let_token = ExpectToken<TokenLet>(lexer_->PeekToken());
  lexer_->PopToken();

  ast::Formal f = ParseFormal();

  auto token_in = ExpectToken<TokenIn>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<ast::Expr> in_expr = ParseExpr(0);

  const auto line_range = ast::LineRange(GetLineNum(let_token),
                                         in_expr->GetLineRange().end_line_num);

  // TODO init expr is empty default constructor
  return std::make_unique<ast::LetExpr>(line_range, f.GetId(), f.GetType(),
                                        std::unique_ptr<ast::Expr>(),
                                        std::move(in_expr));
}

std::unique_ptr<ast::ObjectExpr> Parser::ParseObjectExpr() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      ast::LineRange(GetLineNum(object_id_token), GetLineNum(object_id_token));

  return std::make_unique<ast::ObjectExpr>(line_range,
                                           object_id_token.get_data());
}

std::unique_ptr<ast::BlockExpr> Parser::ParseBlockExpr() const {
  auto lbrace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<std::unique_ptr<ast::Expr>> exprs;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    exprs.push_back(ParseExpr(0));

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      ast::LineRange(GetLineNum(lbrace_token), GetLineNum(rbrace_token));

  return std::make_unique<ast::BlockExpr>(line_range, std::move(exprs));
}

std::unique_ptr<ast::AssignExpr> Parser::ParseAssignExpr() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto assign_token = ExpectToken<TokenAssign>(lexer_->PeekToken());
  lexer_->PopToken();

  // TODO assign is consider a binop with low precidence in cool manual
  auto rhs_expr = ParseExpr(0);

  const auto line_range = ast::LineRange(GetLineNum(object_id_token),
                                         rhs_expr->GetLineRange().end_line_num);

  return std::make_unique<ast::AssignExpr>(object_id_token.get_data(),
                                           std::move(rhs_expr), line_range);
}

}  // namespace coolang