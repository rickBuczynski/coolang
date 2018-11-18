#ifndef COOLANG_CODEGEN_CODEGEN_H
#define COOLANG_CODEGEN_CODEGEN_H

#include "coolang/codegen/platform.h"
#include "coolang/parser/ast.h"

namespace coolang {

class Codegen {
 public:
  explicit Codegen(ProgramAst& ast)
      : Codegen(ast, std::nullopt, std::nullopt) {}

  Codegen(ProgramAst& ast, std::optional<std::filesystem::path> obj_path,
          std::optional<std::filesystem::path> exe_path)
      : ast_(&ast) {
    if (obj_path.has_value()) {
      obj_path_ = obj_path.value();
    } else {
      obj_path_ = ast_->GetFilePath();
      obj_path_.replace_extension(platform::GetObjectFileExtension());
    }

    if (exe_path.has_value()) {
      exe_path_ = exe_path.value();
    } else {
      exe_path_ = ast_->GetFilePath();
      exe_path_.replace_extension(platform::GetExeFileExtension());
    }
  }

  void GenerateCode() const;
  void Link() const;

  const std::filesystem::path& GetFilePath() const {
    return ast_->GetFilePath();
  }

 private:
  ProgramAst* ast_;

  std::filesystem::path obj_path_;
  std::filesystem::path exe_path_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CODEGEN_H
