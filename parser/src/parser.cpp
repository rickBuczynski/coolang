#include <variant>
#include "coolang/lexer/token.h"
#include "coolang/parser/parser.h"

namespace coolang {

int TokenBinOpPrecedence(const Token& token) {
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

bool TokenIsAssociativeBinOp(const Token& token) {
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
        } else {
          return false;
        }
      },
      token);
}

std::unique_ptr<BinOpExpr> BinOpExprForToken(const Token& token,
                                             LineRange line_range,
                                             std::unique_ptr<Expr> lhs_expr,
                                             std::unique_ptr<Expr> rhs_expr) {
  return std::visit(
      [line_range, &lhs_expr,
       &rhs_expr](auto&& arg) -> std::unique_ptr<BinOpExpr> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, TokenDiv>) {
          return std::make_unique<DivideExpr>(line_range, std::move(lhs_expr),
                                              std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenMult>) {
          return std::make_unique<MultiplyExpr>(line_range, std::move(lhs_expr),
                                                std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenPlus>) {
          return std::make_unique<AddExpr>(line_range, std::move(lhs_expr),
                                           std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenMinus>) {
          return std::make_unique<SubtractExpr>(line_range, std::move(lhs_expr),
                                                std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenEq>) {
          return std::make_unique<EqCompareExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenLt>) {
          return std::make_unique<LessThanCompareExpr>(
              line_range, std::move(lhs_expr), std::move(rhs_expr));
        } else if constexpr (std::is_same_v<T, TokenLe>) {
          return std::make_unique<LessThanEqualCompareExpr>(
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

std::variant<ProgramAst, std::vector<ParseError>> Parser::ParseProgram() {
  std::optional<ProgramAst> program_ast;
  try {
    std::vector<ClassAst> classes;

    // force empty program to trigger an error
    // instead of just empty AST
    ExpectToken<TokenClass>(lexer_->PeekToken());

    while (!lexer_->PeekTokenTypeIs<TokenEndOfFile>()) {
      try {
        classes.push_back(ParseClass());

        auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
        lexer_->PopToken();
      } catch (const UnexpectedTokenExcpetion& e) {
        const auto parse_error = ParseError(
            e.GetUnexpectedToken(), lexer_->GetInputFile().filename().string());
        parse_errors_.push_back(parse_error);
        lexer_->AdvanceToNext<TokenClass>();
      }
    }

    auto eof_token = ExpectToken<TokenEndOfFile>(lexer_->PeekToken());

    const int program_end_line =
        classes.empty() ? eof_token.get_line_num()
                        : classes.back().GetLineRange().end_line_num;

    program_ast.emplace(
        ProgramAst(std::move(classes), LineRange(1, program_end_line)));
  } catch (const UnexpectedTokenExcpetion& e) {
    const auto parse_error = ParseError(
        e.GetUnexpectedToken(), lexer_->GetInputFile().filename().string());
    parse_errors_.push_back(parse_error);
  }

  if (!parse_errors_.empty()) {
    return parse_errors_;
  } else {
    return std::move(program_ast.value());
  }
}

ClassAst Parser::ParseClass() {
  auto class_token = ExpectToken<TokenClass>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  std::optional<std::string> inherits_type;
  if (lexer_->PeekTokenTypeIs<TokenInherits>()) {
    lexer_->PopToken();
    auto inherits_type_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
    lexer_->PopToken();

    inherits_type = inherits_type_token.get_data();
  }

  auto lbrace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<std::unique_ptr<Feature>> features;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    try {
      features.push_back(ParseFeature());
    } catch (const UnexpectedTokenExcpetion& e) {
      const auto parse_error = ParseError(
          e.GetUnexpectedToken(), lexer_->GetInputFile().filename().string());
      parse_errors_.push_back(parse_error);
      lexer_->AdvanceToNext<TokenSemi>();
    }
    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  return ClassAst(
      type_id_token.get_data(), inherits_type, std::move(features),
      LineRange(class_token.get_line_num(), rbrace_token.get_line_num()),
      lexer_->GetInputFile().filename().string());
}

std::unique_ptr<Feature> Parser::ParseFeature() {
  if (std::holds_alternative<TokenLparen>(lexer_->LookAheadToken())) {
    try {
      return ParseMethodFeature();
    } catch (const UnexpectedTokenExcpetion& e) {
      lexer_->AdvanceToNext<TokenRbrace>();
      throw e;
    }
  }
  return ParseAttributeFeature();
}

std::unique_ptr<MethodFeature> Parser::ParseMethodFeature() {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto left_paren_token = ExpectToken<TokenLparen>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<Formal> args;
  while (!lexer_->PeekTokenTypeIs<TokenRparen>()) {
    args.push_back(ParseFormal());

    if (lexer_->PeekTokenTypeIs<TokenComma>()) lexer_->PopToken();
    // TODO better solution:
    // if (!lexer_->PeekTokenTypeIs<TokenRparen>()) {
    //  ExpectToken<TokenComma>(lexer_->PeekToken());
    //  lexer_->PopToken();
    //}
  }

  auto right_paren_token = ExpectToken<TokenRparen>(lexer_->PeekToken());
  lexer_->PopToken();

  auto colon_token = ExpectToken<TokenColon>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto left_brace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<Expr> expr = ParseExpr(0);

  auto right_brace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  const LineRange line_range =
      LineRange(GetLineNum(object_id_token), GetLineNum(right_brace_token));

  return std::make_unique<MethodFeature>(line_range, object_id_token.get_data(),
                                         args, type_id_token.get_data(),
                                         std::move(expr));
}

std::unique_ptr<AttributeFeature> Parser::ParseAttributeFeature() {
  Formal f = ParseFormal();

  std::unique_ptr<Expr> initialization_expr;
  if (lexer_->PeekTokenTypeIs<TokenAssign>()) {
    lexer_->PopToken();
    initialization_expr = ParseExpr(0);
  }

  // need to change line range if theres an init expr
  return std::make_unique<AttributeFeature>(
      f.GetLineRange(), f.GetId(), f.GetType(), std::move(initialization_expr));
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

std::unique_ptr<Expr> Parser::ParseExpr(int min_precedence) {
  std::unique_ptr<Expr> lhs_expr;

  if (std::holds_alternative<TokenIntConst>(lexer_->PeekToken())) {
    lhs_expr = ParseIntExpr();
  } else if (std::holds_alternative<TokenBoolConst>(lexer_->PeekToken())) {
    lhs_expr = ParseBoolExpr();
  } else if (std::holds_alternative<TokenStrConst>(lexer_->PeekToken())) {
    lhs_expr = ParseStrExpr();
  } else if (std::holds_alternative<TokenObjectId>(lexer_->PeekToken())) {
    if (std::holds_alternative<TokenAssign>(lexer_->LookAheadToken())) {
      lhs_expr = ParseAssignExpr();
    } else if (std::holds_alternative<TokenLparen>(lexer_->LookAheadToken())) {
      lhs_expr = ParseMethodCallExprRhs(std::unique_ptr<Expr>{}, std::nullopt);
    } else {
      lhs_expr = ParseObjectExpr();
    }
  } else if (std::holds_alternative<TokenLet>(lexer_->PeekToken())) {
    lhs_expr = ParseLetExpr();
  } else if (std::holds_alternative<TokenLbrace>(lexer_->PeekToken())) {
    lhs_expr = ParseBlockExpr();
  } else if (std::holds_alternative<TokenLparen>(lexer_->PeekToken())) {
    auto lparen_token = ExpectToken<TokenLparen>(lexer_->PeekToken());
    lexer_->PopToken();

    lhs_expr = ParseExpr(0);

    auto rparen_token = ExpectToken<TokenRparen>(lexer_->PeekToken());
    lexer_->PopToken();

    // change line range of expr to be line range of parens
    const LineRange line_range(GetLineNum(lparen_token),
                               GetLineNum(rparen_token));
    lhs_expr->SetLineRange(line_range);
  } else if (std::holds_alternative<TokenNeg>(lexer_->PeekToken())) {
    lhs_expr = ParseNegExpr();
  } else if (std::holds_alternative<TokenIf>(lexer_->PeekToken())) {
    lhs_expr = ParseIfExpr();
  } else if (std::holds_alternative<TokenWhile>(lexer_->PeekToken())) {
    lhs_expr = ParseWhileExpr();
  } else if (std::holds_alternative<TokenCase>(lexer_->PeekToken())) {
    lhs_expr = ParseCaseExpr();
  } else if (std::holds_alternative<TokenNot>(lexer_->PeekToken())) {
    lhs_expr = ParseNotExpr();
  } else if (std::holds_alternative<TokenIsVoid>(lexer_->PeekToken())) {
    lhs_expr = ParseIsVoidExpr();
  } else if (std::holds_alternative<TokenNew>(lexer_->PeekToken())) {
    lhs_expr = ParseNewExpr();
  }

  while (lexer_->PeekTokenTypeIs<TokenDot>() ||
         lexer_->PeekTokenTypeIs<TokenAt>()) {
    std::optional<std::string> static_dispatch_type;
    if (lexer_->PeekTokenTypeIs<TokenAt>()) {
      lexer_->PopToken();
      auto token_static_dispatch_type =
          ExpectToken<TokenTypeId>(lexer_->PeekToken());
      static_dispatch_type = token_static_dispatch_type.get_data();
      lexer_->PopToken();
    }

    lexer_->PopToken();
    lhs_expr = ParseMethodCallExprRhs(std::move(lhs_expr),
                                      std::move(static_dispatch_type));
  }

  std::optional<Token> prev_binop_token;
  while (lhs_expr && TokenIsBinOp(lexer_->PeekToken()) &&
         TokenBinOpPrecedence(lexer_->PeekToken()) >= min_precedence) {
    Token binop_token = lexer_->PeekToken();

    if (prev_binop_token.has_value() &&
        prev_binop_token->index() == binop_token.index() &&
        !TokenIsAssociativeBinOp(binop_token)) {
      throw UnexpectedTokenExcpetion(binop_token);
    }
    lexer_->PopToken();
    prev_binop_token = binop_token;

    std::unique_ptr<Expr> rhs_expr =
        ParseExpr(TokenBinOpPrecedence(binop_token) + 1);

    const LineRange line_range(lhs_expr->GetLineRange().start_line_num,
                               rhs_expr->GetLineRange().end_line_num);

    lhs_expr = BinOpExprForToken(binop_token, line_range, std::move(lhs_expr),
                                 std::move(rhs_expr));
  }

  if (lhs_expr) {
    return lhs_expr;
  }
  throw UnexpectedTokenExcpetion(lexer_->PeekToken());
}

std::unique_ptr<IfExpr> Parser::ParseIfExpr() {
  auto if_token = ExpectToken<TokenIf>(lexer_->PeekToken());
  lexer_->PopToken();
  auto if_condition_expr = ParseExpr(0);

  ExpectToken<TokenThen>(lexer_->PeekToken());
  lexer_->PopToken();
  auto then_expr = ParseExpr(0);

  ExpectToken<TokenElse>(lexer_->PeekToken());
  lexer_->PopToken();
  auto else_expr = ParseExpr(0);

  auto fi_token = ExpectToken<TokenFi>(lexer_->PeekToken());
  lexer_->PopToken();

  const LineRange line_range(GetLineNum(if_token), GetLineNum(fi_token));

  return std::make_unique<IfExpr>(line_range, std::move(if_condition_expr),
                                  std::move(then_expr), std::move(else_expr));
}

std::unique_ptr<CaseExpr> Parser::ParseCaseExpr() {
  auto case_token = ExpectToken<TokenCase>(lexer_->PeekToken());
  lexer_->PopToken();
  auto case_expr = ParseExpr(0);

  ExpectToken<TokenOf>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<CaseBranch> branches;
  while (!lexer_->PeekTokenTypeIs<TokenEsac>()) {
    try {
      Formal f = ParseFormal();

      ExpectToken<TokenDarrow>(lexer_->PeekToken());
      lexer_->PopToken();

      std::unique_ptr<Expr> expr = ParseExpr(0);

      const LineRange line_range(f.GetLineRange().start_line_num,
                                 expr->GetLineRange().end_line_num);

      branches.emplace_back(line_range, f.GetId(), f.GetType(),
                            std::move(expr));
    } catch (const UnexpectedTokenExcpetion& e) {
      const auto parse_error = ParseError(
          e.GetUnexpectedToken(), lexer_->GetInputFile().filename().string());
      parse_errors_.push_back(parse_error);
      lexer_->AdvanceToNext<TokenSemi>();
    }

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto esac_token = ExpectToken<TokenEsac>(lexer_->PeekToken());
  lexer_->PopToken();

  const LineRange line_range(GetLineNum(case_token), GetLineNum(esac_token));

  return std::make_unique<CaseExpr>(line_range, std::move(case_expr),
                                    std::move(branches));
}

std::unique_ptr<WhileExpr> Parser::ParseWhileExpr() {
  auto while_token = ExpectToken<TokenWhile>(lexer_->PeekToken());
  lexer_->PopToken();
  auto condition_expr = ParseExpr(0);

  ExpectToken<TokenLoop>(lexer_->PeekToken());
  lexer_->PopToken();
  auto loop_expr = ParseExpr(0);

  auto pool_token = ExpectToken<TokenPool>(lexer_->PeekToken());
  lexer_->PopToken();

  const LineRange line_range(GetLineNum(while_token), GetLineNum(pool_token));

  return std::make_unique<WhileExpr>(line_range, std::move(condition_expr),
                                     std::move(loop_expr));
}

std::unique_ptr<NotExpr> Parser::ParseNotExpr() {
  auto not_token = ExpectToken<TokenNot>(lexer_->PeekToken());
  lexer_->PopToken();

  auto child = ParseExpr(0);

  const LineRange line_range(GetLineNum(not_token),
                             child->GetLineRange().end_line_num);
  return std::make_unique<NotExpr>(line_range, std::move(child));
}

std::unique_ptr<IsVoidExpr> Parser::ParseIsVoidExpr() {
  auto not_token = ExpectToken<TokenIsVoid>(lexer_->PeekToken());
  lexer_->PopToken();

  auto child = ParseExpr(0);

  const LineRange line_range(GetLineNum(not_token),
                             child->GetLineRange().end_line_num);
  return std::make_unique<IsVoidExpr>(line_range, std::move(child));
}

std::unique_ptr<NewExpr> Parser::ParseNewExpr() {
  auto new_token = ExpectToken<TokenNew>(lexer_->PeekToken());
  lexer_->PopToken();

  auto type_id_token = ExpectToken<TokenTypeId>(lexer_->PeekToken());
  lexer_->PopToken();

  const LineRange line_range(GetLineNum(new_token), GetLineNum(type_id_token));

  return std::make_unique<NewExpr>(line_range, type_id_token.get_data());
}

std::unique_ptr<NegExpr> Parser::ParseNegExpr() {
  auto neg_token = ExpectToken<TokenNeg>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<Expr> child = ParseExpr(0);

  const LineRange line_range(GetLineNum(neg_token),
                             child->GetLineRange().end_line_num);

  return std::make_unique<NegExpr>(line_range, std::move(child));
}

std::unique_ptr<IntExpr> Parser::ParseIntExpr() const {
  auto int_const_token = ExpectToken<TokenIntConst>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(int_const_token), GetLineNum(int_const_token));

  return std::make_unique<IntExpr>(int_const_token.get_data(), line_range);
}

std::unique_ptr<BoolExpr> Parser::ParseBoolExpr() const {
  auto bool_const_token = ExpectToken<TokenBoolConst>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(bool_const_token), GetLineNum(bool_const_token));

  return std::make_unique<BoolExpr>(line_range, bool_const_token.get_data());
}

std::unique_ptr<StrExpr> Parser::ParseStrExpr() const {
  auto str_const_token = ExpectToken<TokenStrConst>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(str_const_token), GetLineNum(str_const_token));

  return std::make_unique<StrExpr>(line_range, str_const_token.get_data());
}

std::unique_ptr<LetExpr> Parser::ParseLetExpr() {
  auto let_token = ExpectToken<TokenLet>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<Formal> formals;
  std::vector<std::unique_ptr<Expr>> initialization_exprs;

  while (!lexer_->PeekTokenTypeIs<TokenIn>()) {
    try {
      const Formal formal = ParseFormal();
      std::unique_ptr<Expr> initialization_expr;

      if (lexer_->PeekTokenTypeIs<TokenAssign>()) {
        lexer_->PopToken();
        initialization_expr = ParseExpr(0);
      }

      initialization_exprs.push_back(std::move(initialization_expr));
      formals.push_back(formal);
    } catch (const UnexpectedTokenExcpetion& e) {
      const auto parse_error = ParseError(
          e.GetUnexpectedToken(), lexer_->GetInputFile().filename().string());
      parse_errors_.push_back(parse_error);
      // TODO what if the error is in the last binding with no comma after?
      lexer_->AdvanceToNext<TokenComma>();
    }

    if (!lexer_->PeekTokenTypeIs<TokenIn>()) {
      ExpectToken<TokenComma>(lexer_->PeekToken());
      lexer_->PopToken();
    }
  }

  auto token_in = ExpectToken<TokenIn>(lexer_->PeekToken());
  lexer_->PopToken();

  std::unique_ptr<Expr> in_expr = ParseExpr(0);

  auto let_expr = std::unique_ptr<LetExpr>{};
  for (int i = formals.size() - 1; i >= 0; i--) {
    const int start_line = i == 0 ? GetLineNum(let_token)
                                  : formals[i].GetLineRange().start_line_num;
    int end_line;
    if (i == formals.size() - 1) {
      end_line = in_expr->GetLineRange().end_line_num;
    } else if (initialization_exprs[i]) {
      end_line = initialization_exprs[i]->GetLineRange().end_line_num;
    } else {
      end_line = formals[i].GetLineRange().end_line_num;
    }
    const auto line_range = LineRange(start_line, end_line);

    let_expr = std::make_unique<LetExpr>(
        line_range, formals[i].GetId(), formals[i].GetType(),
        std::move(initialization_exprs[i]), std::move(in_expr),
        std::move(let_expr));
    in_expr = std::unique_ptr<Expr>{};
  }

  return let_expr;
}

std::unique_ptr<ObjectExpr> Parser::ParseObjectExpr() const {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(object_id_token), GetLineNum(object_id_token));

  return std::make_unique<ObjectExpr>(line_range, object_id_token.get_data());
}

std::unique_ptr<BlockExpr> Parser::ParseBlockExpr() {
  auto lbrace_token = ExpectToken<TokenLbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  if (lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    // empty expr list is not valid
    throw UnexpectedTokenExcpetion(lexer_->PeekToken());
  }

  std::vector<std::unique_ptr<Expr>> exprs;
  while (!lexer_->PeekTokenTypeIs<TokenRbrace>()) {
    try {
      exprs.push_back(ParseExpr(0));
    } catch (const UnexpectedTokenExcpetion& e) {
      const auto parse_error = ParseError(
          e.GetUnexpectedToken(), lexer_->GetInputFile().filename().string());
      parse_errors_.push_back(parse_error);
      lexer_->AdvanceToNext<TokenSemi>();
    }

    auto semi_token = ExpectToken<TokenSemi>(lexer_->PeekToken());
    lexer_->PopToken();
  }

  auto rbrace_token = ExpectToken<TokenRbrace>(lexer_->PeekToken());
  lexer_->PopToken();

  const auto line_range =
      LineRange(GetLineNum(lbrace_token), GetLineNum(rbrace_token));

  return std::make_unique<BlockExpr>(line_range, std::move(exprs));
}

std::unique_ptr<MethodCallExpr> Parser::ParseMethodCallExprRhs(
    std::unique_ptr<Expr> lhs,
    std::optional<std::string> static_dispatch_type) {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  ExpectToken<TokenLparen>(lexer_->PeekToken());
  lexer_->PopToken();

  std::vector<std::unique_ptr<Expr>> args;
  while (!lexer_->PeekTokenTypeIs<TokenRparen>()) {
    args.push_back(ParseExpr(0));

    if (!lexer_->PeekTokenTypeIs<TokenRparen>()) {
      ExpectToken<TokenComma>(lexer_->PeekToken());
      lexer_->PopToken();
      // comma after last arg is an error
      if (lexer_->PeekTokenTypeIs<TokenRparen>()) {
        throw UnexpectedTokenExcpetion(lexer_->PeekToken());
      }
    }
  }
  const auto rparen_token = ExpectToken<TokenRparen>(lexer_->PeekToken());
  lexer_->PopToken();

  const int start_line_num =
      lhs ? lhs->GetLineRange().start_line_num : GetLineNum(object_id_token);

  const LineRange line_range(start_line_num, GetLineNum(rparen_token));

  return std::make_unique<MethodCallExpr>(
      line_range, std::move(lhs), std::move(static_dispatch_type),
      object_id_token.get_data(), std::move(args));
}

std::unique_ptr<AssignExpr> Parser::ParseAssignExpr() {
  auto object_id_token = ExpectToken<TokenObjectId>(lexer_->PeekToken());
  lexer_->PopToken();

  auto assign_token = ExpectToken<TokenAssign>(lexer_->PeekToken());
  lexer_->PopToken();

  // TODO assign is consider a binop with low precedence in cool manual
  auto rhs_expr = ParseExpr(0);

  const auto line_range = LineRange(GetLineNum(object_id_token),
                                    rhs_expr->GetLineRange().end_line_num);

  return std::make_unique<AssignExpr>(object_id_token.get_data(),
                                      std::move(rhs_expr), line_range);
}

}  // namespace coolang