/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COOLANG_PARSER_PARSER_H_
#define COOLANG_PARSER_PARSER_H_

#include <filesystem>
#include <unordered_map>
#include "coolang/lexer/lexer.h"
#include "coolang/lexer/token.h"
#include "coolang/parser/ast.h"

namespace coolang {

class ParseError {
 public:
  ParseError(Token unexpected_token, std::filesystem::path file_path)
      : unexpected_token_(std::move(unexpected_token)),
        file_path_(std::move(file_path)) {}

  std::string ToString(int indent_depth) const {
    using namespace std::string_literals;
    return ""s + '"' + file_path_.filename().string() + '"' + ", line " +
           std::to_string(GetLineNum(unexpected_token_)) +
           ": syntax error at or near " +
           TokenTypeSpecificStr(unexpected_token_, " = ") + '\n';
  }

  static std::string ToString(const std::vector<ParseError>& errors) {
    std::string str;
    for (const auto& err : errors) {
      str += err.ToString(0);
    }
    str += "Compilation halted due to lex and parse errors\n";
    return str;
  }

 private:
  const Token unexpected_token_;
  const std::filesystem::path file_path_;
};

class Parser {
 public:
  explicit Parser(std::unique_ptr<Lexer> lexer) : lexer_(std::move(lexer)){};
  std::variant<ProgramAst, std::vector<ParseError>> ParseProgram();

 private:
  ClassAst ParseClass();

  std::unique_ptr<Feature> ParseFeature();
  std::unique_ptr<MethodFeature> ParseMethodFeature();
  std::unique_ptr<AttributeFeature> ParseAttributeFeature();

  Formal ParseFormal() const;

  std::unique_ptr<Expr> ParseExpr(int min_precedence);
  std::unique_ptr<IfExpr> ParseIfExpr();
  std::unique_ptr<CaseExpr> ParseCaseExpr();
  std::unique_ptr<WhileExpr> ParseWhileExpr();
  std::unique_ptr<NotExpr> ParseNotExpr();
  std::unique_ptr<IsVoidExpr> ParseIsVoidExpr();
  std::unique_ptr<NewExpr> ParseNewExpr();
  std::unique_ptr<NegExpr> ParseNegExpr();
  std::unique_ptr<AssignExpr> ParseAssignExpr();
  std::unique_ptr<IntExpr> ParseIntExpr() const;
  std::unique_ptr<BoolExpr> ParseBoolExpr() const;
  std::unique_ptr<StrExpr> ParseStrExpr() const;
  std::unique_ptr<LetExpr> ParseLetExpr();
  std::unique_ptr<ObjectExpr> ParseObjectExpr() const;
  std::unique_ptr<BlockExpr> ParseBlockExpr();

  // parse MethodCallExpr from ID to )
  // the lhs calling the method
  // and the . should alread have been parsed
  // if this is a call like DoThing() with no lhs then lhs should be empty
  std::unique_ptr<MethodCallExpr> ParseMethodCallExprRhs(
      std::unique_ptr<Expr> lhs,
      std::optional<std::string> static_dispatch_type);

  std::unordered_map<std::string, std::unique_ptr<ClassAst>> dummy_classes_;

  std::vector<ParseError> parse_errors_;
  std::unique_ptr<Lexer> lexer_;
};

}  // namespace coolang

#endif  // COOLANG_PARSER_PARSER_H_
