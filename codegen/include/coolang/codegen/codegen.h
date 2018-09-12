#ifndef COOLANG_CODEGEN_CODEGEN_H
#define COOLANG_CODEGEN_CODEGEN_H

#include "coolang/parser/ast.h"

namespace coolang {

class Codegen {
 public:
  explicit Codegen(ProgramAst& ast) : ast_(&ast) {}

  void GenerateCode() const;
  void Link(
      const std::optional<std::string>& exe_filename = std::nullopt) const;

  const std::filesystem::path& GetFilePath() const {
    return ast_->GetFilePath();
  }

 private:
  ProgramAst* ast_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CODEGEN_H
