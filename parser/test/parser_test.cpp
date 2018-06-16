#include "coolang/lexer/lexer.h"
#include "coolang/parser/parser.h"
#include "gtest/gtest.h"

namespace {

std::string GetExpectedOutput(const std::string& expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetParserOutput(const std::string& input_file_name) {
  coolang::Parser parser(std::make_unique<coolang::Lexer>(
      PARSER_TEST_DATA_PATH + input_file_name));

  std::variant<coolang::ProgramAst, coolang::ParseError> program_or_error =
      parser.ParseProgram();

  std::string parse_output =
      std::visit([](auto&& e) { return e.ToString(0); }, program_or_error);
  std::cout << parse_output;

  return parse_output;
}

void TestParser(const std::string& input_file) {
  const std::string parser_output = GetParserOutput(input_file);
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
TEST(ParserTest, associativityminus) { TestParser("associativity-.test"); }
TEST(ParserTest, assigngetstype) { TestParser("assigngetstype.test"); }
TEST(ParserTest, assignseq) { TestParser("assignseq.test"); }
TEST(ParserTest, associativityplus) { TestParser("associativity+.test"); }
TEST(ParserTest, associativitydiv) { TestParser("associativitydiv.test"); }
TEST(ParserTest, associativitytimes) { TestParser("associativity-times.test"); }
TEST(ParserTest, ifexpressionblock) { TestParser("ifexpressionblock.test"); }
TEST(ParserTest, dispatcharglist) { TestParser("dispatcharglist.test"); }
TEST(ParserTest, dispatchnoargs) { TestParser("dispatchnoargs.test"); }
TEST(ParserTest, dispatchonearg) { TestParser("dispatchonearg.test"); }
TEST(ParserTest, letnoinit) { TestParser("letnoinit.test"); }
TEST(ParserTest, letinit) { TestParser("letinit.test"); }
TEST(ParserTest, letparens) { TestParser("letparens.test"); }
TEST(ParserTest, letassociativity) { TestParser("letassociativity.test"); }
TEST(ParserTest, letinitmultiplebindings) { TestParser("letinitmultiplebindings.test"); }
TEST(ParserTest, whileoneexpression) { TestParser("whileoneexpression.test"); }

//TEST(ParserTest, atoi) { TestParser("atoi.test"); }

}  // namespace
