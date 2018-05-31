#include <variant>
#include "coolang/compiler/token.h"

std::string TokenTypeToString(TokenType token_type) {
  switch (token_type) {
    case TokenType::CLASS:
      return ("CLASS");
    case TokenType::ELSE:
      return ("ELSE");
    case TokenType::FI:
      return ("FI");
    case TokenType::IF:
      return ("IF");
    case TokenType::IN:
      return ("IN");
    case TokenType::INHERITS:
      return ("INHERITS");
    case TokenType::LET:
      return ("LET");
    case TokenType::LOOP:
      return ("LOOP");
    case TokenType::POOL:
      return ("POOL");
    case TokenType::THEN:
      return ("THEN");
    case TokenType::WHILE:
      return ("WHILE");
    case TokenType::ASSIGN:
      return ("ASSIGN");
    case TokenType::CASE:
      return ("CASE");
    case TokenType::ESAC:
      return ("ESAC");
    case TokenType::OF:
      return ("OF");
    case TokenType::DARROW:
      return ("DARROW");
    case TokenType::NEW:
      return ("NEW");
    case TokenType::STR_CONST:
      return ("STR_CONST");
    case TokenType::INT_CONST:
      return ("INT_CONST");
    case TokenType::BOOL_CONST:
      return ("BOOL_CONST");
    case TokenType::TYPEID:
      return ("TYPEID");
    case TokenType::OBJECTID:
      return ("OBJECTID");
    case TokenType::ERROR:
      return ("ERROR");
    // case TokenType::error:
    //  return ("ERROR");
    case TokenType::LE:
      return ("LE");
    case TokenType::NOT:
      return ("NOT");
    case TokenType::ISVOID:
      return ("ISVOID");
    case TokenType::PLUS:
      return ("'+'");
    case TokenType::DIV:
      return ("'/'");
    case TokenType::MINUS:
      return ("'-'");
    case TokenType::MULT:
      return ("'*'");
    case TokenType::EQ:
      return ("'='");
    case TokenType::LT:
      return ("'<'");
    case TokenType::DOT:
      return ("'.'");
    case TokenType::NEG:
      return ("'~'");
    case TokenType::COMMA:
      return ("','");
    case TokenType::SEMI:
      return ("';'");
    case TokenType::COLON:
      return ("':'");
    case TokenType::LPAREN:
      return ("'('");
    case TokenType::RPAREN:
      return ("')'");
    case TokenType::AT:
      return ("'@'");
    case TokenType::LBRACE:
      return ("'{'");
    case TokenType::RBRACE:
      return ("'}'");
    case TokenType::END_OF_FILE:
      return ("EOF");
  }
}

std::string Escaped(const std::string& str) {
  // will print excape sequences e.g. instead of a tab character will print \t
  // instead of a newline \n
  std::string escaped;
  for (int i = 0; i < str.length(); i++) {
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

std::string Token::ToString() {
  std::string token_as_string =
      "#" + std::to_string(line_num_) + " " + TokenTypeToString(token_type_);

  std::string val_as_string = ValAsString();

  if (token_type_ == TokenType::STR_CONST) {
    token_as_string += " \"" + Escaped(val_as_string) + '"';
  } else if (token_type_ == TokenType::ERROR) {
    token_as_string += " \"" + val_as_string + '"';
  } else if (val_as_string.length() > 0) {
    token_as_string += " " + val_as_string;
  }

  return token_as_string;
};

std::string Token::ValAsString() {
  return std::visit(
      [](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
          return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, bool>) {
          return arg == true ? "true" : "false";
        } else if constexpr (std::is_same_v<T, std::string>) {
          return arg;
        } else if constexpr (std::is_same_v<T, std::monostate>) {
          return "";
        }
      },
      val_);
}