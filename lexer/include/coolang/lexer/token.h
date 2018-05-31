#ifndef COOLANG_LEXER_TOKEN_H_
#define COOLANG_LEXER_TOKEN_H_

#include <string>
#include <variant>

template <class T>
class TokenTypeWith {
 public:
  TokenTypeWith(T data) : data_(data) {}
  T get_data() { return data_; };

 private:
  T data_;
};

class TokenTypeIntConst : public TokenTypeWith<int> {
  using TokenTypeWith::TokenTypeWith;
};

class TokenTypeBoolConst : public TokenTypeWith<bool> {
  using TokenTypeWith::TokenTypeWith;
};

class TokenTypeStrConst : public TokenTypeWith<std::string> {
  using TokenTypeWith::TokenTypeWith;
};
class TokenTypeTypeId : public TokenTypeWith<std::string> {
  using TokenTypeWith::TokenTypeWith;
};
class TokenTypeObjectId : public TokenTypeWith<std::string> {
  using TokenTypeWith::TokenTypeWith;
};
class TokenTypeError : public TokenTypeWith<std::string> {
  using TokenTypeWith::TokenTypeWith;
};

// TODO there was a lower case "error" token to be used later in the parser

class TokenTypeClass {};
class TokenTypeElse {};
class TokenTypeFi {};
class TokenTypeIf {};
class TokenTypeIn {};
class TokenTypeInherits {};
class TokenTypeLet {};
class TokenTypeLoop {};
class TokenTypePool {};
class TokenTypeThen {};
class TokenTypeWhile {};
class TokenTypeAssign {};
class TokenTypeCase {};
class TokenTypeEsac {};
class TokenTypeOf {};
class TokenTypeDarrow {};
class TokenTypeNew {};
class TokenTypeLe {};
class TokenTypeNot {};
class TokenTypeIsVoid {};
class TokenTypePlus {};
class TokenTypeDiv {};
class TokenTypeMinus {};
class TokenTypeMult {};
class TokenTypeEq {};
class TokenTypeLt {};
class TokenTypeDot {};
class TokenTypeNeg {};
class TokenTypeComma {};
class TokenTypeSemi {};
class TokenTypeColon {};
class TokenTypeLparen {};
class TokenTypeRparen {};
class TokenTypeAt {};
class TokenTypeLbrace {};
class TokenTypeRbrace {};
class TokenTypeEndOfFile {};

// clang-format off
using TokenType = std::variant<
  TokenTypeIntConst,
  TokenTypeBoolConst,

  TokenTypeStrConst,
  TokenTypeTypeId,
  TokenTypeObjectId,
  TokenTypeError,

  TokenTypeClass,
  TokenTypeElse,
  TokenTypeFi,
  TokenTypeIf,
  TokenTypeIn,
  TokenTypeInherits,
  TokenTypeLet,
  TokenTypeLoop,
  TokenTypePool,
  TokenTypeThen,
  TokenTypeWhile,
  TokenTypeAssign,
  TokenTypeCase,
  TokenTypeEsac,
  TokenTypeOf,
  TokenTypeDarrow,
  TokenTypeNew,
  TokenTypeLe,
  TokenTypeNot,
  TokenTypeIsVoid,
  TokenTypePlus,
  TokenTypeDiv,
  TokenTypeMinus,
  TokenTypeMult,
  TokenTypeEq,
  TokenTypeLt,
  TokenTypeDot,
  TokenTypeNeg,
  TokenTypeComma,
  TokenTypeSemi,
  TokenTypeColon,
  TokenTypeLparen,
  TokenTypeRparen,
  TokenTypeAt,
  TokenTypeLbrace,
  TokenTypeRbrace,
  TokenTypeEndOfFile
>;
// clang-format on

std::string TokenTypeToString(TokenType token_type);

class Token {
 public:
  Token(TokenType token_type, int line_num)
      : token_type_(token_type), line_num_(line_num){};

  TokenType token_type() { return token_type_; };

  std::string ToString();

 private:
  TokenType token_type_;
  int line_num_;
};

#endif  // COOLANG_LEXER_TOKEN_H_
