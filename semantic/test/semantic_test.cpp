// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "coolang/semantic/semantic.h"
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

std::string GetSemanticOutput(const std::string& input_file_name) {
  auto lexer = std::make_unique<coolang::Lexer>(SEMANTIC_TEST_DATA_PATH +
                                                input_file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  auto ast = std::get<coolang::ProgramAst>(parser->ParseProgram());

  const coolang::Semantic semantic;
  auto semantic_errors = semantic.CheckProgramSemantics(ast);

  if (!semantic_errors.empty()) {
    return coolang::SemanticError::ToString(semantic_errors);
  } else {
    return ast.ToString(0);
  }
}

void TestSemantic(const std::string& input_file) {
  const std::string semantic_output = GetSemanticOutput(input_file);
  // std::cout << semantic_output;

  std::string expected_output =
      GetExpectedOutput(SEMANTIC_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, semantic_output);
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
TEST(SemanticTest, dispatchwrongnumargs) {
  TestSemantic("dispatchwrongnumargs.test");
}
TEST(SemanticTest, dupformals) { TestSemantic("dupformals.test"); }
TEST(SemanticTest, expressionblock) { TestSemantic("expressionblock.test"); }
TEST(SemanticTest, forwardinherits) { TestSemantic("forwardinherits.test"); }
TEST(SemanticTest, hairyscarycl) { TestSemantic("hairyscary.cl.test"); }
TEST(SemanticTest, ifselftype) { TestSemantic("if-self-type.test"); }
TEST(SemanticTest, iftest) { TestSemantic("if.test"); }
TEST(SemanticTest, inheritsbool) { TestSemantic("inheritsbool.test"); }
TEST(SemanticTest, inheritsObject) { TestSemantic("inheritsObject.test"); }
TEST(SemanticTest, inheritsselftype) { TestSemantic("inheritsselftype.test"); }
TEST(SemanticTest, inheritsstring) { TestSemantic("inheritsstring.test"); }
TEST(SemanticTest, initwithself) { TestSemantic("initwithself.test"); }
TEST(SemanticTest, inttoobig) { TestSemantic("inttoobig.test"); }
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
TEST(SemanticTest, negnonint) { TestSemantic("negnonint.test"); }
TEST(SemanticTest, newselftype) { TestSemantic("newselftype.test"); }
TEST(SemanticTest, newundefined) { TestSemantic("newundefined.test"); }
TEST(SemanticTest, nomain) { TestSemantic("nomain.test"); }
TEST(SemanticTest, notnonbool) { TestSemantic("notnonbool.test"); }
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
TEST(SemanticTest, selfinformalparameter) {
  TestSemantic("selfinformalparameter.test");
}
TEST(SemanticTest, selftypebadreturn) {
  TestSemantic("selftypebadreturn.test");
}
TEST(SemanticTest, selftypeparameterposition) {
  TestSemantic("selftypeparameterposition.test");
}
TEST(SemanticTest, selftyperedeclared) {
  TestSemantic("selftyperedeclared.test");
}
TEST(SemanticTest, signaturechange) { TestSemantic("signaturechange.test"); }
TEST(SemanticTest, simplearith) { TestSemantic("simplearith.test"); }
TEST(SemanticTest, simplecase) { TestSemantic("simplecase.test"); }
TEST(SemanticTest, staticdispatch) { TestSemantic("staticdispatch.test"); }
TEST(SemanticTest, stringtest) { TestSemantic("stringtest.test"); }
TEST(SemanticTest, subtypemethodreturn) {
  TestSemantic("subtypemethodreturn.test");
}
TEST(SemanticTest, trickyatdispatch) { TestSemantic("trickyatdispatch.test"); }
TEST(SemanticTest, trickyatdispatch2) {
  TestSemantic("trickyatdispatch2.test");
}

}  // namespace
