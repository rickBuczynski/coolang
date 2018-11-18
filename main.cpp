#include <gflags/gflags.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <variant>
#include "coolang/codegen/codegen.h"
#include "coolang/lexer/lexer.h"
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic.h"

DEFINE_string(obj_path, "", "Path to where the object file should be output.");
DEFINE_string(exe_path, "",
              "Path to where the executable program should be output.");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc != 2) {
    std::cerr << "usage: pass 1 arg, the path of the file to compile";
    return EXIT_FAILURE;
  }
  std::string file_name = argv[1];

  auto lexer = std::make_unique<coolang::Lexer>(file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  auto ast_or_err = parser->ParseProgram();

  if (std::holds_alternative<std::vector<coolang::ParseError>>(ast_or_err)) {
    auto err = std::get<std::vector<coolang::ParseError>>(ast_or_err);
    std::cerr << coolang::ParseError::ToString(err);
    return EXIT_FAILURE;
  }

  auto ast = std::get<coolang::ProgramAst>(std::move(ast_or_err));

  const coolang::Semantic semantic;
  auto semantic_errors = semantic.CheckProgramSemantics(ast);
  if (!semantic_errors.empty()) {
    std::cerr << coolang::SemanticError::ToString(semantic_errors);
    return EXIT_FAILURE;
  }

  std::optional<std::filesystem::path> obj_path;
  if (!FLAGS_obj_path.empty()) {
    obj_path = FLAGS_obj_path;
  }

  std::optional<std::filesystem::path> exe_path;
  if (!FLAGS_exe_path.empty()) {
    exe_path = FLAGS_exe_path;
  }

  const auto codegen =
      std::make_unique<coolang::Codegen>(ast, obj_path, exe_path);
  codegen->GenerateCode();
  codegen->Link();

  return EXIT_SUCCESS;
}
