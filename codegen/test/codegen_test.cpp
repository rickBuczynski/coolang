#include <filesystem>
#include "coolang/codegen/codegen.h"
#include "coolang/lexer/lexer.h"
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic.h"
#include "gtest/gtest.h"

namespace {

std::string GetExpectedOutput(const std::string& expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

void RunProgram(std::filesystem::path input_file_path) {
  std::filesystem::path output_file_path =
      input_file_path.replace_extension(".runout");
  std::filesystem::path exe_path = input_file_path.replace_extension(".exe");

  std::string command = exe_path.string() + " > " + output_file_path.string();
  std::system(command.c_str());
}

std::string GetProgramOutput(std::filesystem::path input_file_path) {
  std::filesystem::path output_file_path =
      input_file_path.replace_extension(".runout");

  std::ifstream t(output_file_path.string());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetCodgenedProgramOutput(const std::string& input_file_name) {
  auto lexer = std::make_unique<coolang::Lexer>(CODEGEN_TEST_DATA_PATH +
                                                input_file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  const auto semantic = std::make_unique<coolang::Semantic>(std::move(parser));
  auto ast_or_err = semantic->CheckProgramSemantics();

  const auto codegen = std::make_unique<coolang::Codegen>(
      std::move(std::get<coolang::ProgramAst>(ast_or_err)));
  codegen->GenerateCode();
  codegen->Link();

  RunProgram(codegen->GetFilePath());
  return GetProgramOutput(codegen->GetFilePath());
}

void TestCodegen(const std::string& input_file) {
  const std::string parser_output = GetCodgenedProgramOutput(input_file);
  std::string expected_output =
      GetExpectedOutput(CODEGEN_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, parser_output);
}

TEST(CodegenTest, basicinit) { TestCodegen("basic-init.cl"); }
TEST(CodegenTest, interactionattrinitmethod) {
  TestCodegen("interaction-attrinit-method.cl");
}
TEST(CodegenTest, simplegc) { TestCodegen("simple-gc.cl"); }
TEST(CodegenTest, bigexpr) { TestCodegen("bigexpr.cl"); }
TEST(CodegenTest, letinit) { TestCodegen("letinit.cl"); }
TEST(CodegenTest, stringmethods) { TestCodegen("string-methods.cl"); }
TEST(CodegenTest, fibo) { TestCodegen("fibo.cl"); }
TEST(CodegenTest, sequence) { TestCodegen("sequence.cl"); }

}  // namespace
