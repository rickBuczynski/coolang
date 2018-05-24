#ifndef COOLANG_COMPILER_TOKEN_H_
#define COOLANG_COMPILER_TOKEN_H_

#include <string>
#include <variant>

enum class TokenType {
  CLASS,
  ELSE,
  FI,
  IF,
  IN,
  INHERITS,
  LET,
  LOOP,
  POOL,
  THEN,
  WHILE,
  ASSIGN,
  CASE,
  ESAC,
  OF,
  DARROW,
  NEW,
  STR_CONST,
  INT_CONST,
  BOOL_CONST,
  TYPEID,
  OBJECTID,
  ERROR,
  // error,
  LE,
  NOT,
  ISVOID,
  PLUS,
  DIV,
  MINUS,
  MULT,
  EQ,
  LT,
  DOT,
  NEG,
  COMMA,
  SEMI,
  COLON,
  LPAREN,
  RPAREN,
  AT,
  LBRACE,
  RBRACE,
  END_OF_FILE,
};
std::string TokenTypeToString(TokenType token_type);

class Token {
 public:
  Token(TokenType token_type, int line_num)
      : token_type_(token_type), line_num_(line_num){};

  Token(TokenType _token_type, int line_num, int int_val)
      : token_type_(_token_type), line_num_(line_num), val_(int_val){};

  Token(TokenType _token_type, int line_num, bool bool_val)
      : token_type_(_token_type), line_num_(line_num), val_(bool_val){};

  Token(TokenType _token_type, int line_num, std::string string_val)
      : token_type_(_token_type), line_num_(line_num), val_(string_val){};

  Token(){};

  TokenType token_type() { return token_type_; };

  std::string ToString() {
    return std::to_string(line_num_) + ": " + TokenTypeToString(token_type_) +
           " " + ValAsString();
  };

 private:
  std::string ValAsString() {
    return std::visit(
        [](auto&& arg) -> std::string {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, int>) {
            return std::to_string(arg);
          } else if constexpr (std::is_same_v<T, bool>) {
            return arg == true ? "true" : "false";
          } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
          } else {
            static_assert(std::false_type::value, "non-exhaustive visitor!");
          }
        },
        val_);
  }

  TokenType token_type_;
  int line_num_;
  std::variant<std::string, int, bool> val_;
};

#endif  // COOLANG_COMPILER_TOKEN_H_
