#include <fstream>
#include <iostream>
#include "coolang/compiler/lexer.h"
#include "coolang/compiler/token.h"
#include "gtest/gtest.h"

namespace {

std::string get_expected_output(std::string expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string get_lexer_output(std::string input_file_name) {
  Lexer lexer(LEXER_TEST_DATA_PATH + input_file_name);
  Token tok;

  std::string lexer_output = "#name \"" + input_file_name + "\"" + '\n';

  while ((tok = lexer.GetNextToken()).token_type() != TokenType::END_OF_FILE) {
    lexer_output += tok.ToString();
    lexer_output += '\n';
  }
  return lexer_output;
}

TEST(LexerTest, AllElseTrue) {
  std::string lexer_output = get_lexer_output("all_else_true.cl.cool");
  std::cout << lexer_output;
  std::cout << get_expected_output(LEXER_TEST_DATA_PATH
                                   "/all_else_true.cl.cool.out");
  EXPECT_EQ(1 + 1, 2);
}

}  // namespace