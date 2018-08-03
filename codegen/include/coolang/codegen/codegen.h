#ifndef COOLANG_CODEGEN_CODEGEN_H
#define COOLANG_CODEGEN_CODEGEN_H

#include <vector>
#include "coolang/semantic/semantic.h"

namespace coolang {

class Codegen {
 public:
  explicit Codegen(std::unique_ptr<Semantic> semantic)
      : semantic_(std::move(semantic)) {}

  void GenerateCode();

 private:
  std::unique_ptr<Semantic> semantic_;
};

}  // namespace coolang

#endif  // COOLANG_CODEGEN_CODEGEN_H
