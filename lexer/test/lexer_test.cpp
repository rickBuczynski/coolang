#include <fstream>
#include <iostream>
#include <variant>
#include "coolang/lexer/lexer.h"
#include "coolang/lexer/token.h"
#include "gtest/gtest.h"

namespace {

std::string GetExpectedOutput(const std::string& expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetLexerOutput(const std::string& input_file_name) {
  Lexer lexer(LEXER_TEST_DATA_PATH + input_file_name);

  std::string lexer_output = "#name \"" + input_file_name + "\"" + '\n';

  while (!lexer.PeekTokenTypeIs<TokenEndOfFile>()) {
    lexer_output += TokenToString(lexer.PeekToken());
    lexer_output += '\n';
    // std::cout << TokenToString(lexer.PeekToken()) << std::endl;
    lexer.PopToken();
  }
  return lexer_output;
}

void TestLexer(const std::string& lexer_input_file) {
  const std::string lexer_output = GetLexerOutput(lexer_input_file);
  std::string expected_output =
      GetExpectedOutput(LEXER_TEST_DATA_PATH + lexer_input_file + ".out");
  EXPECT_EQ(expected_output, lexer_output);
}

TEST(LexerTest, all_else_true_cl) { TestLexer("all_else_true.cl.cool"); }
TEST(LexerTest, arith) { TestLexer("arith.cool"); }
TEST(LexerTest, atoi) { TestLexer("atoi.cool"); }
TEST(LexerTest, backslash) { TestLexer("backslash.cool"); }
TEST(LexerTest, backslash2) { TestLexer("backslash2.cool"); }
TEST(LexerTest, badidentifiers) { TestLexer("badidentifiers.cool"); }
TEST(LexerTest, badkeywords) { TestLexer("badkeywords.cool"); }
TEST(LexerTest, book_list_cl) { TestLexer("book_list.cl.cool"); }
TEST(LexerTest, bothcomments) { TestLexer("bothcomments.cool"); }
TEST(LexerTest, comment_in_string_cl) {
  TestLexer("comment_in_string.cl.cool");
}
TEST(LexerTest, endcomment) { TestLexer("endcomment.cool"); }
TEST(LexerTest, eofstring) { TestLexer("eofstring.cool"); }
TEST(LexerTest, escapedeof) { TestLexer("escapedeof.cool"); }
TEST(LexerTest, escapednull) { TestLexer("escapednull.cool"); }
TEST(LexerTest, escapedquote) { TestLexer("escapedquote.cool"); }
TEST(LexerTest, escapedunprintables) { TestLexer("escapedunprintables.cool"); }
TEST(LexerTest, escaped_chars_in_comment_cl) {
  TestLexer("escaped_chars_in_comment.cl.cool");
}
TEST(LexerTest, hairyscary) { TestLexer("hairyscary.cool"); }
TEST(LexerTest, integers2) { TestLexer("integers2.cool"); }
TEST(LexerTest, invalidcharacters) { TestLexer("invalidcharacters.cool"); }
TEST(LexerTest, invalidinvisible) { TestLexer("invalidinvisible.cool"); }
TEST(LexerTest, io) { TestLexer("io.cool"); }
TEST(LexerTest, keywords) { TestLexer("keywords.cool"); }
TEST(LexerTest, life) { TestLexer("life.cool"); }
TEST(LexerTest, lineno2) { TestLexer("lineno2.cool"); }
TEST(LexerTest, lineno3) { TestLexer("lineno3.cool"); }
TEST(LexerTest, longcomment) { TestLexer("longcomment.cool"); }
TEST(LexerTest, longstring_escapedbackslashes) {
  TestLexer("longstring_escapedbackslashes.cool");
}
TEST(LexerTest, multilinecomment) { TestLexer("multilinecomment.cool"); }
TEST(LexerTest, nestedcomment) { TestLexer("nestedcomment.cool"); }
TEST(LexerTest, new_complex) { TestLexer("new_complex.cool"); }
TEST(LexerTest, null_in_code_cl) { TestLexer("null_in_code.cl.cool"); }
TEST(LexerTest, null_in_string_cl) { TestLexer("null_in_string.cl.cool"); }
TEST(LexerTest, null_in_string_followed_by_tokens_cl) {
  TestLexer("null_in_string_followed_by_tokens.cl.cool");
}
TEST(LexerTest, null_in_string_then_escaped_newline) {
  TestLexer("null_in_string_then_escaped_newline.cool");
}
TEST(LexerTest, null_in_string_then_eof) {
  TestLexer("null_in_string_then_eof.cool");
}
TEST(LexerTest, null_in_string_unescaped_newline_cl) {
  TestLexer("null_in_string_unescaped_newline.cl.cool");
}
TEST(LexerTest, objectid_test) { TestLexer("objectid.test.cool"); }
TEST(LexerTest, opencomment) { TestLexer("opencomment.cool"); }
TEST(LexerTest, operators) { TestLexer("operators.cool"); }
TEST(LexerTest, palindrome) { TestLexer("palindrome.cool"); }
TEST(LexerTest, pathologicalstrings) { TestLexer("pathologicalstrings.cool"); }
TEST(LexerTest, s03_test) { TestLexer("s03.test.cool"); }
TEST(LexerTest, s04_test) { TestLexer("s04.test.cool"); }
TEST(LexerTest, s05_test) { TestLexer("s05.test.cool"); }
TEST(LexerTest, s14_test) { TestLexer("s14.test.cool"); }
TEST(LexerTest, s16_test) { TestLexer("s16.test.cool"); }
TEST(LexerTest, s19_test) { TestLexer("s19.test.cool"); }
TEST(LexerTest, s25_test) { TestLexer("s25.test.cool"); }
TEST(LexerTest, s26_test) { TestLexer("s26.test.cool"); }
TEST(LexerTest, s31_test) { TestLexer("s31.test.cool"); }
TEST(LexerTest, s32_test) { TestLexer("s32.test.cool"); }
TEST(LexerTest, s33_test) { TestLexer("s33.test.cool"); }
TEST(LexerTest, s34_test) { TestLexer("s34.test.cool"); }
TEST(LexerTest, simplestrings) { TestLexer("simplestrings.cool"); }
TEST(LexerTest, sort_list_cl) { TestLexer("sort_list.cl.cool"); }
TEST(LexerTest, stringcomment) { TestLexer("stringcomment.cool"); }
TEST(LexerTest, stringwithescapes) { TestLexer("stringwithescapes.cool"); }
TEST(LexerTest, twice_512_nested_comments_cl) {
  TestLexer("twice_512_nested_comments.cl.cool");
}
TEST(LexerTest, validcharacters) { TestLexer("validcharacters.cool"); }
TEST(LexerTest, weirdcharcomment) { TestLexer("weirdcharcomment.cool"); }
TEST(LexerTest, wq0607_c1) { TestLexer("wq0607-c1.cool"); }
TEST(LexerTest, wq0607_c2) { TestLexer("wq0607-c2.cool"); }
TEST(LexerTest, wq0607_c3) { TestLexer("wq0607-c3.cool"); }
TEST(LexerTest, wq0607_c4) { TestLexer("wq0607-c4.cool"); }

}  // namespace
