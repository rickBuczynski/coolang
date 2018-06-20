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

  std::variant<coolang::ProgramAst, std::vector<coolang::ParseError>>
      program_or_error = parser.ParseProgram();

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
        }
      },
      program_or_error);
  //std::cout << parse_output;
  return parse_output;
}

void TestParser(const std::string& input_file) {
  const std::string parser_output = GetParserOutput(input_file);
  std::string expected_output =
      GetExpectedOutput(PARSER_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, parser_output);
}

TEST(ParserTest, addedlet) { TestParser("addedlet.test"); }
TEST(ParserTest, arithprecedence) { TestParser("arithprecedence.test"); }
TEST(ParserTest, assigngetstype) { TestParser("assigngetstype.test"); }
TEST(ParserTest, assignment) { TestParser("assignment.test"); }
TEST(ParserTest, assignseq) { TestParser("assignseq.test"); }
TEST(ParserTest, associativityplus) { TestParser("associativity+.test"); }
TEST(ParserTest, associativityminus) { TestParser("associativity-.test"); }
TEST(ParserTest, associativitytimes) { TestParser("associativity-times.test"); }
TEST(ParserTest, associativitydiv) { TestParser("associativitydiv.test"); }
TEST(ParserTest, atoi) { TestParser("atoi.test"); }
TEST(ParserTest, attrcapitalname) { TestParser("attrcapitalname.test"); }
TEST(ParserTest, badblock) { TestParser("badblock.test"); }
TEST(ParserTest, baddispatch1) { TestParser("baddispatch1.test"); }
TEST(ParserTest, baddispatch2) { TestParser("baddispatch2.test"); }
TEST(ParserTest, baddispatch3) { TestParser("baddispatch3.test"); }
TEST(ParserTest, baddispatch4) { TestParser("baddispatch4.test"); }
TEST(ParserTest, badexprlist) { TestParser("badexprlist.test"); }
TEST(ParserTest, badfeaturenames) { TestParser("badfeaturenames.test"); }
TEST(ParserTest, badfeatures) { TestParser("badfeatures.test"); }
TEST(ParserTest, casemultiplebranch) { TestParser("casemultiplebranch.test"); }
TEST(ParserTest, casenoexpr) { TestParser("casenoexpr.test"); }
TEST(ParserTest, classbadinherits) { TestParser("classbadinherits.test"); }
TEST(ParserTest, classbadname) { TestParser("classbadname.test"); }
TEST(ParserTest, classmethodonearg) { TestParser("classmethodonearg.test"); }
TEST(ParserTest, classnoname) { TestParser("classnoname.test"); }
TEST(ParserTest, classonefield) { TestParser("classonefield.test"); }
TEST(ParserTest, classtwofields) { TestParser("classtwofields.test"); }
TEST(ParserTest, comparisonsassoc) { TestParser("comparisons-assoc.test"); }
TEST(ParserTest, complex) { TestParser("complex.test"); }
TEST(ParserTest, dispatcharglist) { TestParser("dispatcharglist.test"); }
TEST(ParserTest, dispatchnoargs) { TestParser("dispatchnoargs.test"); }
TEST(ParserTest, dispatchonearg) { TestParser("dispatchonearg.test"); }
TEST(ParserTest, emptyassign) { TestParser("emptyassign.test"); }
TEST(ParserTest, emptymethodbody) { TestParser("emptymethodbody.test"); }
TEST(ParserTest, emptyprogram) { TestParser("emptyprogram.test"); }
TEST(ParserTest, emptystaticdispatch) {
  TestParser("emptystaticdispatch.test");
}
TEST(ParserTest, equalsassociativity) {
  TestParser("equalsassociativity.test");
}
TEST(ParserTest, extrasemicolonblock) {
  TestParser("extrasemicolonblock.test");
}
TEST(ParserTest, firstbindingerrored) {
  TestParser("firstbindingerrored.test");
}
TEST(ParserTest, firstclasserrored) { TestParser("firstclasserrored.test"); }
TEST(ParserTest, formallists) { TestParser("formallists.test"); }
TEST(ParserTest, ifexpressionblock) { TestParser("ifexpressionblock.test"); }
TEST(ParserTest, ifnested) { TestParser("ifnested.test"); }
TEST(ParserTest, ifnoelse) { TestParser("ifnoelse.test"); }
TEST(ParserTest, ifnoelsebranch) { TestParser("ifnoelsebranch.test"); }
TEST(ParserTest, ifnofi) { TestParser("ifnofi.test"); }
TEST(ParserTest, ifnothenbranch) { TestParser("ifnothenbranch.test"); }
TEST(ParserTest, isvoidbadtype) { TestParser("isvoidbadtype.test"); }
TEST(ParserTest, letassociativity) { TestParser("letassociativity.test"); }
TEST(ParserTest, letinit) { TestParser("letinit.test"); }
TEST(ParserTest, letinitmultiplebindings) {
  TestParser("letinitmultiplebindings.test");
}
TEST(ParserTest, letnoinit) { TestParser("letnoinit.test"); }
TEST(ParserTest, letparens) { TestParser("letparens.test"); }
TEST(ParserTest, lteassociativity) { TestParser("lteassociativity.test"); }
TEST(ParserTest, multipleatdispatches) {
  TestParser("multipleatdispatches.test");
}
TEST(ParserTest, multipleattributes) { TestParser("multipleattributes.test"); }
TEST(ParserTest, multipleclasses) { TestParser("multipleclasses.test"); }
TEST(ParserTest, multipledispatches) { TestParser("multipledispatches.test"); }
TEST(ParserTest, multiplemethoderrors) {
  TestParser("multiplemethoderrors.test");
}
TEST(ParserTest, nestedblocks) { TestParser("nestedblocks.test"); }
TEST(ParserTest, nestedlet) { TestParser("nestedlet.test"); }
TEST(ParserTest, newbadtype) { TestParser("newbadtype.test"); }
TEST(ParserTest, returntypebad) { TestParser("returntypebad.test"); }
TEST(ParserTest, secondbindingerrored) {
  TestParser("secondbindingerrored.test");
}
TEST(ParserTest, staticdispatchnoargs) {
  TestParser("staticdispatchnoargs.test");
}
TEST(ParserTest, unaryassociativity) { TestParser("unaryassociativity.test"); }
TEST(ParserTest, whiletest) { TestParser("while.test"); }
TEST(ParserTest, whileexpressionblock) {
  TestParser("whileexpressionblock.test");
}
TEST(ParserTest, whilenoloop) { TestParser("whilenoloop.test"); }
TEST(ParserTest, whileoneexpression) { TestParser("whileoneexpression.test"); }

}  // namespace
