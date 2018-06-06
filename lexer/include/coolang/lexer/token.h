#ifndef COOLANG_LEXER_TOKEN_H_
#define COOLANG_LEXER_TOKEN_H_

#include <string>
#include <variant>

class TokenBase {
 public:
  TokenBase(int line_num) : line_num_(line_num){};
  int get_line_num() { return line_num_; };

 private:
  int line_num_;
};

template <class T>
class TokenWith : public TokenBase {
 public:
  TokenWith(T data, int line_num) : TokenBase(line_num), data_(data) {}
  T get_data() { return data_; };

 private:
  T data_;
};

// store int token as string to avoid int overflow
// will convert to 32bit int and report overflow errors later
class TokenIntConst : public TokenWith<std::string> {
  using TokenWith::TokenWith;
};

class TokenBoolConst : public TokenWith<bool> {
  using TokenWith::TokenWith;
};

class TokenStrConst : public TokenWith<std::string> {
  using TokenWith::TokenWith;

 public:
  static constexpr int MAX_STR_CONST_LEN = 1024;
};
class TokenTypeId : public TokenWith<std::string> {
  using TokenWith::TokenWith;
};
class TokenObjectId : public TokenWith<std::string> {
  using TokenWith::TokenWith;
};
class TokenError : public TokenWith<std::string> {
  using TokenWith::TokenWith;
};

// TODO there was a lower case "error" token to be used later in the parser

class TokenClass : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenElse : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenFi : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenIf : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenIn : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenInherits : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenLet : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenLoop : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenPool : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenThen : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenWhile : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenAssign : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenCase : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenEsac : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenOf : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenDarrow : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenNew : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenLe : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenNot : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenIsVoid : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenPlus : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenDiv : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenMinus : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenMult : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenEq : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenLt : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenDot : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenNeg : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenComma : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenSemi : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenColon : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenLparen : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenRparen : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenAt : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenLbrace : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenRbrace : public TokenBase {
  using TokenBase::TokenBase;
};
class TokenEndOfFile : public TokenBase {
  using TokenBase::TokenBase;
};

// clang-format off
using Token = std::variant<
  TokenIntConst,
  TokenBoolConst,

  TokenStrConst,
  TokenTypeId,
  TokenObjectId,
  TokenError,

  TokenClass,
  TokenElse,
  TokenFi,
  TokenIf,
  TokenIn,
  TokenInherits,
  TokenLet,
  TokenLoop,
  TokenPool,
  TokenThen,
  TokenWhile,
  TokenAssign,
  TokenCase,
  TokenEsac,
  TokenOf,
  TokenDarrow,
  TokenNew,
  TokenLe,
  TokenNot,
  TokenIsVoid,
  TokenPlus,
  TokenDiv,
  TokenMinus,
  TokenMult,
  TokenEq,
  TokenLt,
  TokenDot,
  TokenNeg,
  TokenComma,
  TokenSemi,
  TokenColon,
  TokenLparen,
  TokenRparen,
  TokenAt,
  TokenLbrace,
  TokenRbrace,
  TokenEndOfFile
>;
// clang-format on

int GetLineNum(Token token);
std::string TokenToString(Token token);

#endif  // COOLANG_LEXER_TOKEN_H_
