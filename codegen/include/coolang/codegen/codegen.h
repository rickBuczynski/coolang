/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
  Codegen(ProgramAst& ast, std::optional<std::filesystem::path> obj_path,
          std::optional<std::filesystem::path> exe_path);

  ~Codegen();  // define in cpp because of unique_ptr to fwd declared
  Codegen(const Codegen&) = delete;
  Codegen(Codegen&&) noexcept = delete;
  Codegen& operator=(const Codegen& other) = delete;
  Codegen& operator=(Codegen&& other) noexcept = delete;

  // gc_verbose is used for testing GC, not intended for public use
  void GenerateCode(bool gc_verbose = false) const;
  void Link() const;

 private:
  ProgramAst* ast_;

  std::unique_ptr<llvm::LLVMContext> context_;
  std::unique_ptr<llvm::Module> module_;

  std::filesystem::path obj_path_;
  std::filesystem::path exe_path_;

  const std::vector<std::string> stdlib_fstems = {"gc", "io"};
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CODEGEN_H
