// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iomanip>
#include <iostream>
#include <sstream>
#include <variant>
#include "coolang/lexer/token.h"

namespace coolang {

std::string Escaped(const std::string& str) {
  // will print excape sequences e.g. instead of a tab character will print \t
  // instead of a newline \n
  std::string escaped;
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == '\\') {
      escaped += '\\';
      escaped += '\\';
    } else if (str[i] == '\b') {
      escaped += '\\';
      escaped += 'b';
    } else if (str[i] == '\t') {
      escaped += '\\';
      escaped += 't';
    } else if (str[i] == '\n') {
      escaped += '\\';
      escaped += 'n';
    } else if (str[i] == '\f') {
      escaped += '\\';
      escaped += 'f';
    } else if (str[i] == '"') {
      escaped += '\\';
      escaped += '"';
    } else if (iscntrl(str[i])) {
      std::ostringstream stringStream;
      stringStream << std::setfill('0') << std::setw(3) << std::oct
                   << static_cast<int>(str[i]);
      escaped += '\\';
      escaped += stringStream.str();
    } else {
      escaped += str[i];
    }
  }
  return escaped;
}

int GetLineNum(Token token) {
  return std::visit([](auto&& e) { return e.get_line_num(); }, token);
}

std::string TokenToString(const Token& token) {
  const int line_num = GetLineNum(token);
  return "#" + std::to_string(line_num) + " " + TokenTypeSpecificStr(token);
}

std::string TokenTypeSpecificStr(Token token, const std::string& separator) {
  return std::visit(
      [separator](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        using namespace std::string_literals;
        if constexpr (std::is_same_v<T, TokenIntConst>) {
          return "INT_CONST"s + separator + arg.get_data();

        } else if constexpr (std::is_same_v<T, TokenBoolConst>) {
          return "BOOL_CONST"s + separator +
                 (arg.get_data() ? "true" : "false");

        } else if constexpr (std::is_same_v<T, TokenStrConst>) {
          return "STR_CONST"s + separator + "\"" + Escaped(arg.get_data()) +
                 "\"";

        } else if constexpr (std::is_same_v<T, TokenTypeId>) {
          return "TYPEID"s + separator + arg.get_data();

        } else if constexpr (std::is_same_v<T, TokenObjectId>) {
          return "OBJECTID"s + separator + arg.get_data();

        } else if constexpr (std::is_same_v<T, TokenError>) {
          return "ERROR"s + separator + "\"" + Escaped(arg.get_data()) + "\"";

        } else if constexpr (std::is_same_v<T, TokenClass>) {
          return "CLASS";
        } else if constexpr (std::is_same_v<T, TokenElse>) {
          return "ELSE";
        } else if constexpr (std::is_same_v<T, TokenFi>) {
          return "FI";
        } else if constexpr (std::is_same_v<T, TokenIf>) {
          return "IF";
        } else if constexpr (std::is_same_v<T, TokenIn>) {
          return "IN";
        } else if constexpr (std::is_same_v<T, TokenInherits>) {
          return "INHERITS";
        } else if constexpr (std::is_same_v<T, TokenLet>) {
          return "LET";
        } else if constexpr (std::is_same_v<T, TokenLoop>) {
          return "LOOP";
        } else if constexpr (std::is_same_v<T, TokenPool>) {
          return "POOL";
        } else if constexpr (std::is_same_v<T, TokenThen>) {
          return "THEN";
        } else if constexpr (std::is_same_v<T, TokenWhile>) {
          return "WHILE";
        } else if constexpr (std::is_same_v<T, TokenAssign>) {
          return "ASSIGN";
        } else if constexpr (std::is_same_v<T, TokenCase>) {
          return "CASE";
        } else if constexpr (std::is_same_v<T, TokenEsac>) {
          return "ESAC";
        } else if constexpr (std::is_same_v<T, TokenOf>) {
          return "OF";
        } else if constexpr (std::is_same_v<T, TokenDarrow>) {
          return "DARROW";
        } else if constexpr (std::is_same_v<T, TokenNew>) {
          return "NEW";
        } else if constexpr (std::is_same_v<T, TokenLe>) {
          return "LE";
        } else if constexpr (std::is_same_v<T, TokenNot>) {
          return "NOT";
        } else if constexpr (std::is_same_v<T, TokenIsVoid>) {
          return "ISVOID";
        } else if constexpr (std::is_same_v<T, TokenPlus>) {
          return "'+'";
        } else if constexpr (std::is_same_v<T, TokenDiv>) {
          return "'/'";
        } else if constexpr (std::is_same_v<T, TokenMinus>) {
          return "'-'";
        } else if constexpr (std::is_same_v<T, TokenMult>) {
          return "'*'";
        } else if constexpr (std::is_same_v<T, TokenEq>) {
          return "'='";
        } else if constexpr (std::is_same_v<T, TokenLt>) {
          return "'<'";
        } else if constexpr (std::is_same_v<T, TokenDot>) {
          return "'.'";
        } else if constexpr (std::is_same_v<T, TokenNeg>) {
          return "'~'";
        } else if constexpr (std::is_same_v<T, TokenComma>) {
          return "','";
        } else if constexpr (std::is_same_v<T, TokenSemi>) {
          return "';'";
        } else if constexpr (std::is_same_v<T, TokenColon>) {
          return "':'";
        } else if constexpr (std::is_same_v<T, TokenLparen>) {
          return "'('";
        } else if constexpr (std::is_same_v<T, TokenRparen>) {
          return "')'";
        } else if constexpr (std::is_same_v<T, TokenAt>) {
          return "'@'";
        } else if constexpr (std::is_same_v<T, TokenLbrace>) {
          return "'{'";
        } else if constexpr (std::is_same_v<T, TokenRbrace>) {
          return "'}'";
        } else if constexpr (std::is_same_v<T, TokenEndOfFile>) {
          return "EOF";
        }
      },
      token);
}

}  // namespace coolang