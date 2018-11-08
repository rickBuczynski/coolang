#include <filesystem>
#include <iostream>
#include <string>
#include <variant>
#include "coolang/codegen/codegen.h"
#include "coolang/codegen/platform.h"
#include "coolang/lexer/lexer.h"
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic.h"

int main(int argc, char *argv[]) {
  if (argc != 1) {
    std::cerr << "usage: pass 1 arg, the path of the file to compile";
    return EXIT_FAILURE;
  }
  std::string file_name = argv[0];

  auto lexer = std::make_unique<coolang::Lexer>(file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  auto ast_or_err = parser->ParseProgram();

  if (std::holds_alternative<std::vector<coolang::ParseError>>(ast_or_err)) {
    auto err = std::get<std::vector<coolang::ParseError>>(ast_or_err);
    std::cerr << coolang::ParseError::ToString(err);
    return EXIT_FAILURE;
  }

  auto ast = std::get<coolang::ProgramAst>(parser->ParseProgram());

  const coolang::Semantic semantic;
  auto semantic_errors = semantic.CheckProgramSemantics(ast);
  if (!semantic_errors.empty()) {
    std::cerr << coolang::SemanticError::ToString(semantic_errors);
    return EXIT_FAILURE;
  }

  const auto codegen = std::make_unique<coolang::Codegen>(ast);
  codegen->GenerateCode();

  return EXIT_SUCCESS;
}
