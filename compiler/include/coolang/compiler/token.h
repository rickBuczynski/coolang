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
  Token(TokenType _token_type) : token_type(_token_type){};

  Token(TokenType _token_type, int int_val)
      : token_type(_token_type), val(int_val){};

  Token(){};

  TokenType token_type;
  int line_num;
  std::variant<std::string, int, bool> val;

  std::string ToString() {
    return std::to_string(line_num) + ": " + TokenTypeToString(token_type);
  };
};

#endif  // COOLANG_COMPILER_TOKEN_H_
