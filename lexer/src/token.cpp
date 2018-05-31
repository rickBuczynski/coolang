#include <variant>
#include "coolang/lexer/token.h"

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
    } else {
      escaped += str[i];
    }
  }
  return escaped;
}

std::string TokenTypeToString(TokenType token_type) {
  return std::visit(
      [](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        using namespace std::string_literals;
        if constexpr (std::is_same_v<T, TokenTypeIntConst>) {
          return "INT_CONST "s + std::to_string(arg.get_data());

        } else if constexpr (std::is_same_v<T, TokenTypeBoolConst>) {
          return "BOOL_CONST "s + (arg.get_data() ? "true" : "false");

        } else if constexpr (std::is_same_v<T, TokenTypeStrConst>) {
          return "STR_CONST "s + "\"" + Escaped(arg.get_data()) + "\"";

        } else if constexpr (std::is_same_v<T, TokenTypeTypeId>) {
          return "TYPEID "s + arg.get_data();

        } else if constexpr (std::is_same_v<T, TokenTypeObjectId>) {
          return "OBJECTID "s + arg.get_data();

        } else if constexpr (std::is_same_v<T, TokenTypeError>) {
          return "ERROR "s + "\"" + arg.get_data() + "\"";

        } else if constexpr (std::is_same_v<T, TokenTypeClass>) {
          return ("CLASS");
        } else if constexpr (std::is_same_v<T, TokenTypeElse>) {
          return ("ELSE");
        } else if constexpr (std::is_same_v<T, TokenTypeFi>) {
          return ("FI");
        } else if constexpr (std::is_same_v<T, TokenTypeIf>) {
          return ("IF");
        } else if constexpr (std::is_same_v<T, TokenTypeIn>) {
          return ("IN");
        } else if constexpr (std::is_same_v<T, TokenTypeInherits>) {
          return ("INHERITS");
        } else if constexpr (std::is_same_v<T, TokenTypeLet>) {
          return ("LET");
        } else if constexpr (std::is_same_v<T, TokenTypeLoop>) {
          return ("LOOP");
        } else if constexpr (std::is_same_v<T, TokenTypePool>) {
          return ("POOL");
        } else if constexpr (std::is_same_v<T, TokenTypeThen>) {
          return ("THEN");
        } else if constexpr (std::is_same_v<T, TokenTypeWhile>) {
          return ("WHILE");
        } else if constexpr (std::is_same_v<T, TokenTypeAssign>) {
          return ("ASSIGN");
        } else if constexpr (std::is_same_v<T, TokenTypeCase>) {
          return ("CASE");
        } else if constexpr (std::is_same_v<T, TokenTypeEsac>) {
          return ("ESAC");
        } else if constexpr (std::is_same_v<T, TokenTypeOf>) {
          return ("OF");
        } else if constexpr (std::is_same_v<T, TokenTypeDarrow>) {
          return ("DARROW");
        } else if constexpr (std::is_same_v<T, TokenTypeNew>) {
          return ("NEW");
        } else if constexpr (std::is_same_v<T, TokenTypeLe>) {
          return ("LE");
        } else if constexpr (std::is_same_v<T, TokenTypeNot>) {
          return ("NOT");
        } else if constexpr (std::is_same_v<T, TokenTypeIsVoid>) {
          return ("ISVOID");
        } else if constexpr (std::is_same_v<T, TokenTypePlus>) {
          return ("'+'");
        } else if constexpr (std::is_same_v<T, TokenTypeDiv>) {
          return ("'/'");
        } else if constexpr (std::is_same_v<T, TokenTypeMinus>) {
          return ("'-'");
        } else if constexpr (std::is_same_v<T, TokenTypeMult>) {
          return ("'*'");
        } else if constexpr (std::is_same_v<T, TokenTypeEq>) {
          return ("'='");
        } else if constexpr (std::is_same_v<T, TokenTypeLt>) {
          return ("'<'");
        } else if constexpr (std::is_same_v<T, TokenTypeDot>) {
          return ("'.'");
        } else if constexpr (std::is_same_v<T, TokenTypeNeg>) {
          return ("'~'");
        } else if constexpr (std::is_same_v<T, TokenTypeComma>) {
          return ("','");
        } else if constexpr (std::is_same_v<T, TokenTypeSemi>) {
          return ("';'");
        } else if constexpr (std::is_same_v<T, TokenTypeColon>) {
          return ("':'");
        } else if constexpr (std::is_same_v<T, TokenTypeLparen>) {
          return ("'('");
        } else if constexpr (std::is_same_v<T, TokenTypeRparen>) {
          return ("')'");
        } else if constexpr (std::is_same_v<T, TokenTypeAt>) {
          return ("'@'");
        } else if constexpr (std::is_same_v<T, TokenTypeLbrace>) {
          return ("'{'");
        } else if constexpr (std::is_same_v<T, TokenTypeRbrace>) {
          return ("'}'");
        } else if constexpr (std::is_same_v<T, TokenTypeEndOfFile>) {
          return ("EOF");
        }
      },
      token_type);
}

std::string Token::ToString() {
  return "#" + std::to_string(line_num_) + " " + TokenTypeToString(token_type_);
};
