#ifndef COOLANG_CODEGEN_CODEGEN_H
#define COOLANG_CODEGEN_CODEGEN_H

#include "coolang/parser/ast.h"

namespace llvm {
class LLVMContext;
class Module;
}  // namespace llvm

namespace coolang {

class Codegen {
 public:
  explicit Codegen(ProgramAst& ast)
      : Codegen(ast, std::nullopt, std::nullopt) {}

  Codegen(ProgramAst& ast, std::optional<std::filesystem::path> obj_path,
          std::optional<std::filesystem::path> exe_path);

  ~Codegen();  // define in cpp because of unique_ptr to fwd declared
  Codegen(const Codegen&) = delete;
  Codegen(Codegen&&) noexcept = delete;
  Codegen& operator=(const Codegen& other) = delete;
  Codegen& operator=(Codegen&& other) noexcept = delete;

  void GenerateCode() const;
  void Link() const;

 private:
  ProgramAst* ast_;

  std::unique_ptr<llvm::LLVMContext> context_;
  std::unique_ptr<llvm::Module> module_;

  std::filesystem::path obj_path_;
  std::filesystem::path exe_path_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CODEGEN_H
