#include "coolang/lexer/lexer.h"
#include "coolang/parser/parser.h"
#include "gtest/gtest.h"

namespace {

std::string GetExpectedOutput(std::string expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetParserOutput(std::string input_file_name) {
  Parser parser(
      std::make_unique<Lexer>(PARSER_TEST_DATA_PATH + input_file_name));

  std::variant<coolang::ast::Program, ParseError> program_or_error =
      parser.ParseProgram();

  std::string parse_output =
      std::visit([](auto&& e) { return e.ToString(0); }, program_or_error);
  std::cout << parse_output;

  return parse_output;
}

void TestParser(std::string input_file) {
  std::string parser_output = GetParserOutput(input_file);
  std::string expected_output =
      GetExpectedOutput(PARSER_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, parser_output);
}

TEST(ParserTest, classonefield) { TestParser("classonefield.test"); }
TEST(ParserTest, classnoname) { TestParser("classnoname.test"); }
TEST(ParserTest, classtwofields) { TestParser("classtwofields.test"); }
TEST(ParserTest, assignment) { TestParser("assignment.test"); }
TEST(ParserTest, addedlet) { TestParser("addedlet.test"); }
TEST(ParserTest, arithprecedence) { TestParser("arithprecedence.test"); }

}  // namespace
