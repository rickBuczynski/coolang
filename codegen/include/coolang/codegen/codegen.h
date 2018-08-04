#ifndef COOLANG_CODEGEN_CODEGEN_H
#define COOLANG_CODEGEN_CODEGEN_H

#include "coolang/parser/ast.h"

namespace coolang {

class Codegen {
 public:
  explicit Codegen(ProgramAst ast) : ast_(std::move(ast)) {}

  void GenerateCode() const;
  void Link() const;

 private:
  ProgramAst ast_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CODEGEN_H
