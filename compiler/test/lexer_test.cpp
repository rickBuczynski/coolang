#include <fstream>
#include <iostream>
#include "coolang/compiler/lexer.h"
#include "coolang/compiler/token.h"
#include "gtest/gtest.h"

namespace {

std::string GetExpectedOutput(std::string expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetLexerOutput(std::string input_file_name) {
  Lexer lexer(LEXER_TEST_DATA_PATH + input_file_name);
  Token tok;

  std::string lexer_output = "#name \"" + input_file_name + "\"" + '\n';

  while ((tok = lexer.GetNextToken()).token_type() != TokenType::END_OF_FILE) {
    lexer_output += tok.ToString();
    lexer_output += '\n';

	std::cout << tok.ToString() << std::endl;
  }
  return lexer_output;
}

void TestLexer(std::string lexer_input_file) {
  std::string lexer_output = GetLexerOutput(lexer_input_file);
  std::string expected_output =
      GetExpectedOutput(LEXER_TEST_DATA_PATH + lexer_input_file + ".out");
  EXPECT_EQ(lexer_output, expected_output);
}

TEST(LexerTest, allelsetrue) { TestLexer("all_else_true.cl.cool"); }
//TEST(LexerTest, arith) { TestLexer("arith.cool"); }
TEST(LexerTest, longcomment) { TestLexer("longcomment.cool"); }
TEST(LexerTest, bothcomments) { TestLexer("bothcomments.cool"); }

}  // namespace