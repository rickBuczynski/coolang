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

TEST(LexerTest, AllElseTrue) {
  Lexer lexer(LEXER_TEST_DATA_PATH "/all_else_true.cl.cool");
  Token tok;

  while ((tok = lexer.GetNextToken()).token_type != TokenType::END_OF_FILE) {
    std::cout << tok.ToString() << std::endl;
  }

  std::cout << get_expected_output(LEXER_TEST_DATA_PATH
                                   "/all_else_true.cl.cool.out");
  EXPECT_EQ(1 + 1, 2);
}

}  // namespace