// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "coolang/semantic/semantic.h"
#include <unordered_map>
#include "coolang/semantic/inheritance_checker.h"
#include "coolang/semantic/type_checker.h"

namespace coolang {

std::vector<SemanticError> Semantic::CheckProgramSemantics(
    ProgramAst& ast) const {
  auto inheritance_graph_errors =
      InheritanceChecker::SetInheritanceRelations(ast);
  if (!inheritance_graph_errors.empty()) {
    return inheritance_graph_errors;
  }

  return TypeChecker::CheckTypes(ast);
}

}  // namespace coolang
