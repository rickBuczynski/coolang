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

TEST(SemanticTest, anattributenamedself) {
  TestSemantic("anattributenamedself.test");
}
TEST(SemanticTest, assignment) { TestSemantic("assignment.test"); }
TEST(SemanticTest, assignnoconform) { TestSemantic("assignnoconform.test"); }
TEST(SemanticTest, attrbadinit) { TestSemantic("attrbadinit.test"); }
TEST(SemanticTest, attroverride) { TestSemantic("attroverride.test"); }
TEST(SemanticTest, badargs1) { TestSemantic("badargs1.test"); }
TEST(SemanticTest, badarith) { TestSemantic("badarith.test"); }
TEST(SemanticTest, baddispatch) { TestSemantic("baddispatch.test"); }
TEST(SemanticTest, badequalitytest) { TestSemantic("badequalitytest.test"); }
TEST(SemanticTest, badequalitytest2) { TestSemantic("badequalitytest2.test"); }
TEST(SemanticTest, badmethodcallsitself) {
  TestSemantic("badmethodcallsitself.test");
}
TEST(SemanticTest, badredefineint) { TestSemantic("badredefineint.test"); }
TEST(SemanticTest, badstaticdispatch) {
  TestSemantic("badstaticdispatch.test");
}
TEST(SemanticTest, badwhilebody) { TestSemantic("badwhilebody.test"); }
TEST(SemanticTest, badwhilecond) { TestSemantic("badwhilecond.test"); }
TEST(SemanticTest, basic) { TestSemantic("basic.test"); }
TEST(SemanticTest, basicclassestree) { TestSemantic("basicclassestree.test"); }
TEST(SemanticTest, casenonecl) { TestSemantic("case-none.cl.test"); }
TEST(SemanticTest, caseidenticalbranch) {
  TestSemantic("caseidenticalbranch.test");
}
TEST(SemanticTest, cellscl) { TestSemantic("cells.cl.test"); }
TEST(SemanticTest, classes) { TestSemantic("classes.test"); }
TEST(SemanticTest, compare) { TestSemantic("compare.test"); }
TEST(SemanticTest, comparisons) { TestSemantic("comparisons.test"); }
TEST(SemanticTest, cycleinmethods) { TestSemantic("cycleinmethods.test"); }
TEST(SemanticTest, dispatch) { TestSemantic("dispatch.test"); }
TEST(SemanticTest, dupformals) { TestSemantic("dupformals.test"); }
TEST(SemanticTest, expressionblock) { TestSemantic("expressionblock.test"); }
TEST(SemanticTest, forwardinherits) { TestSemantic("forwardinherits.test"); }
TEST(SemanticTest, hairyscarycl) { TestSemantic("hairyscary.cl.test"); }
TEST(SemanticTest, ifselftype) { TestSemantic("if-self-type.test"); }
TEST(SemanticTest, if) { TestSemantic("if.test"); }
TEST(SemanticTest, inheritsbool) { TestSemantic("inheritsbool.test"); }
TEST(SemanticTest, inheritsObject) { TestSemantic("inheritsObject.test"); }
TEST(SemanticTest, inheritsselftype) { TestSemantic("inheritsselftype.test"); }
TEST(SemanticTest, inheritsstring) { TestSemantic("inheritsstring.test"); }
TEST(SemanticTest, initwithself) { TestSemantic("initwithself.test"); }
TEST(SemanticTest, io) { TestSemantic("io.test"); }
TEST(SemanticTest, isvoid) { TestSemantic("isvoid.test"); }
TEST(SemanticTest, letbadinit) { TestSemantic("letbadinit.test"); }
TEST(SemanticTest, letinit) { TestSemantic("letinit.test"); }
TEST(SemanticTest, letnoinit) { TestSemantic("letnoinit.test"); }
TEST(SemanticTest, letself) { TestSemantic("letself.test"); }
TEST(SemanticTest, letselftype) { TestSemantic("letselftype.test"); }
TEST(SemanticTest, letshadows) { TestSemantic("letshadows.test"); }
TEST(SemanticTest, listcl) { TestSemantic("list.cl.test"); }
TEST(SemanticTest, lubtest) { TestSemantic("lubtest.test"); }
TEST(SemanticTest, mainmethodmissing) {
  TestSemantic("mainmethod-missing.test");
}
TEST(SemanticTest, mainmethodwrong) { TestSemantic("mainmethod-wrong.test"); }
TEST(SemanticTest, methodcallsitself) {
  TestSemantic("methodcallsitself.test");
}
TEST(SemanticTest, methodnameclash) { TestSemantic("methodnameclash.test"); }
TEST(SemanticTest, missingclass) { TestSemantic("missingclass.test"); }
TEST(SemanticTest, neg) { TestSemantic("neg.test"); }
TEST(SemanticTest, newselftype) { TestSemantic("newselftype.test"); }
TEST(SemanticTest, nomain) { TestSemantic("nomain.test"); }
TEST(SemanticTest, objectdispatchabort) {
  TestSemantic("objectdispatchabort.test");
}
TEST(SemanticTest, outofscope) { TestSemantic("outofscope.test"); }
TEST(SemanticTest, overriderenamearg) {
  TestSemantic("overriderenamearg.test");
}
TEST(SemanticTest, overridingmethod) { TestSemantic("overridingmethod.test"); }
TEST(SemanticTest, overridingmethod2) {
  TestSemantic("overridingmethod2.test");
}
TEST(SemanticTest, overridingmethod3) {
  TestSemantic("overridingmethod3.test");
}
TEST(SemanticTest, overridingmethod4) {
  TestSemantic("overridingmethod4.test");
}
TEST(SemanticTest, redefinedclass) { TestSemantic("redefinedclass.test"); }
TEST(SemanticTest, redefinedobject) { TestSemantic("redefinedobject.test"); }
TEST(SemanticTest, returntypenoexist) {
  TestSemantic("returntypenoexist.test");
}
TEST(SemanticTest, scopes) { TestSemantic("scopes.test"); }
TEST(SemanticTest, selfassignment) { TestSemantic("self-assignment.test"); }
//TEST(SemanticTest, selfinformalparameter) {
//  TestSemantic("selfinformalparameter.test");
//}
//TEST(SemanticTest, selftypebadreturn) {
//  TestSemantic("selftypebadreturn.test");
//}
//TEST(SemanticTest, selftypeparameterposition) {
//  TestSemantic("selftypeparameterposition.test");
//}
//TEST(SemanticTest, selftyperedeclared) {
//  TestSemantic("selftyperedeclared.test");
//}
//TEST(SemanticTest, signaturechange) { TestSemantic("signaturechange.test"); }
//TEST(SemanticTest, simplearith) { TestSemantic("simplearith.test"); }
TEST(SemanticTest, simplecase) { TestSemantic("simplecase.test"); }
TEST(SemanticTest, staticdispatch) { TestSemantic("staticdispatch.test"); }
TEST(SemanticTest, stringtest) { TestSemantic("stringtest.test"); }
TEST(SemanticTest, subtypemethodreturn) {
  TestSemantic("subtypemethodreturn.test");
}
//TEST(SemanticTest, trickyatdispatch) { TestSemantic("trickyatdispatch.test"); }
//TEST(SemanticTest, trickyatdispatch2) {
//  TestSemantic("trickyatdispatch2.test");
//}

}  // namespace
