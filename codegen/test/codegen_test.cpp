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

std::string GetCodgenedProgramOutput(const std::string& input_file_name) {
  auto lexer = std::make_unique<coolang::Lexer>(CODEGEN_TEST_DATA_PATH +
                                                input_file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  auto semantic = std::make_unique<coolang::Semantic>(std::move(parser));
  auto codegen = std::make_unique<coolang::Codegen>(std::move(semantic));
  codegen->GenerateCode();

  return "";
}

void TestCodegen(const std::string& input_file) {
  const std::string parser_output = GetCodgenedProgramOutput(input_file);
  std::string expected_output =
      GetExpectedOutput(CODEGEN_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, parser_output);
}

TEST(CodegenTest, assignmentval) { TestCodegen("assignment-val.cl"); }

}  // namespace
