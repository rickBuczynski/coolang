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

std::string GetSemanticOutput(const std::string& input_file_name) {
  auto lexer = std::make_unique<coolang::Lexer>(SEMANTIC_TEST_DATA_PATH +
                                                input_file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));

  const coolang::Semantic semantic(std::move(parser));

  auto program_or_error = semantic.CheckProgramSemantics();

  std::string parse_output = std::visit(
      [](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, coolang::ProgramAst>) {
          return arg.ToString(0);
        } else if constexpr (std::is_same_v<T,
                                            std::vector<coolang::ParseError>>) {
          std::string str;
          for (const auto& err : arg) {
            str += err.ToString(0);
          }
          str += "Compilation halted due to lex and parse errors\n";
          return str;

        } else if constexpr (std::is_same_v<
                                 T, std::vector<coolang::SemanticError>>) {
          std::string str;
          for (const auto& err : arg) {
            str += err.ToString(0);
          }
          str += "Compilation halted due to static semantic errors.\n";
          return str;
        }
      },
      program_or_error);
  std::cout << parse_output;
  return parse_output;
}

void TestSemantic(const std::string& input_file) {
  const std::string parser_output = GetSemanticOutput(input_file);
  std::string expected_output =
      GetExpectedOutput(SEMANTIC_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, parser_output);
}

TEST(SemanticTest, inheritsbool) { TestSemantic("inheritsbool.test"); }
TEST(SemanticTest, outofscope) { TestSemantic("outofscope.test"); }

TEST(SemanticTest, anattributenamedself) { TestSemantic("anattributenamedself.test"); }
TEST(SemanticTest, assignment) { TestSemantic("assignment.test"); }
TEST(SemanticTest, assignnoconform) { TestSemantic("assignnoconform.test"); }
TEST(SemanticTest, attrbadinit) { TestSemantic("attrbadinit.test"); }
TEST(SemanticTest, attroverride) { TestSemantic("attroverride.test"); }
TEST(SemanticTest, badargs1) { TestSemantic("badargs1.test"); }


}  // namespace
