#include <fstream>
#include <iostream>
#include <variant>
#include "coolang/lexer/lexer.h"
#include "coolang/lexer/token.h"
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
  // TODO maybe lexer should get the first token ready during constructor?
  lexer.PopToken();

  std::string lexer_output = "#name \"" + input_file_name + "\"" + '\n';

  while (!std::holds_alternative<TokenTypeEndOfFile>(
      lexer.PeekToken().value().token_type())) {
    lexer_output += lexer.PeekToken().value().ToString();
    lexer_output += '\n';

    std::cout << lexer.PeekToken().value().ToString() << std::endl;

    lexer.PopToken();
  }
  return lexer_output;
}

void TestLexer(std::string lexer_input_file) {
  std::string lexer_output = GetLexerOutput(lexer_input_file);
  std::string expected_output =
      GetExpectedOutput(LEXER_TEST_DATA_PATH + lexer_input_file + ".out");
  EXPECT_EQ(expected_output, lexer_output);
}

// clang-format off
TEST(LexerTest, allelsetrue) { TestLexer("all_else_true.cl.cool"); }
TEST(LexerTest, arith) { TestLexer("arith.cool"); }
TEST(LexerTest, atoi) { TestLexer("atoi.cool"); }
TEST(LexerTest, backslash) { TestLexer("backslash.cool"); }
TEST(LexerTest, backslash2) { TestLexer("backslash2.cool"); }
TEST(LexerTest, badidentifiers) { TestLexer("badidentifiers.cool"); }
TEST(LexerTest, badkeywords) { TestLexer("badkeywords.cool"); }
TEST(LexerTest, book_list_cl) { TestLexer("book_list.cl.cool"); }
TEST(LexerTest, bothcomments) { TestLexer("bothcomments.cool"); }
TEST(LexerTest, comment_in_string_cl) { TestLexer("comment_in_string.cl.cool"); }
TEST(LexerTest, endcomment) { TestLexer("endcomment.cool"); }
TEST(LexerTest, eofstring) { TestLexer("eofstring.cool"); }
TEST(LexerTest, escaped_chars_in_comment_cl) { TestLexer("escaped_chars_in_comment.cl.cool"); }
TEST(LexerTest, escapedeof) { TestLexer("escapedeof.cool"); }
// clang-format on

}  // namespace