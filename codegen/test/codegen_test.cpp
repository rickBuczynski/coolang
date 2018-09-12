#include <filesystem>
#include "coolang/codegen/codegen.h"
#include "coolang/lexer/lexer.h"
#include "coolang/parser/parser.h"
#include "coolang/semantic/semantic.h"
#include "gtest/gtest.h"

namespace {

std::string ReplacedPatchWithPaatch(std::string str) {
  const auto patch_substr_loc = str.find("patch");
  if (patch_substr_loc == std::string::npos) {
    return str;
  } else {
    str.replace(patch_substr_loc, strlen("patch"), "paatch");
    return str;
  }
}

std::string GetExpectedOutput(const std::string& expected_output_file) {
  std::ifstream t(expected_output_file);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

void RunProgram(std::filesystem::path input_file_path) {
  std::filesystem::path output_file_path =
      input_file_path.replace_extension(".runout");
  std::filesystem::path exe_path = input_file_path.replace_extension(".exe");
  exe_path.replace_filename(
      ReplacedPatchWithPaatch(exe_path.filename().string()));

  std::string command = exe_path.string() + " > " + output_file_path.string();
  std::system(command.c_str());
}

std::string GetProgramOutput(std::filesystem::path input_file_path) {
  std::filesystem::path output_file_path =
      input_file_path.replace_extension(".runout");

  std::ifstream t(output_file_path.string());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetCodgenedProgramOutput(const std::string& input_file_name) {
  auto lexer = std::make_unique<coolang::Lexer>(CODEGEN_TEST_DATA_PATH +
                                                input_file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  const auto semantic = std::make_unique<coolang::Semantic>(std::move(parser));
  auto ast_or_err = semantic->CheckProgramSemantics();

  const auto codegen = std::make_unique<coolang::Codegen>(
      std::move(std::get<coolang::ProgramAst>(ast_or_err)));
  codegen->GenerateCode();

  // Windows won't run exe with "patch" in the name...
  if (input_file_name.find("patch") == std::string::npos) {
    codegen->Link();
  } else {
    codegen->Link(ReplacedPatchWithPaatch(input_file_name));
  }

  RunProgram(codegen->GetFilePath());
  return GetProgramOutput(codegen->GetFilePath());
}

void TestCodegen(const std::string& input_file) {
  const std::string parser_output = GetCodgenedProgramOutput(input_file);
  std::string expected_output =
      GetExpectedOutput(CODEGEN_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, parser_output);
}

// TEST(CodegenTest, abort) { TestCodegen("abort.cl"); }
TEST(CodegenTest, assignmentval) { TestCodegen("assignment-val.cl"); }
TEST(CodegenTest, basicinit) { TestCodegen("basic-init.cl"); }
// TEST(CodegenTest, basicequality) { TestCodegen("basicequality.cl"); }
// TEST(CodegenTest, bigexample) { TestCodegen("bigexample.cl"); }
TEST(CodegenTest, bigexpr) { TestCodegen("bigexpr.cl"); }
// TEST(CodegenTest, book_list) { TestCodegen("book_list.cl"); }
// TEST(CodegenTest, boolcl) { TestCodegen("bool.cl"); }
// TEST(CodegenTest, c) { TestCodegen("c.cl"); }
// TEST(CodegenTest, calls) { TestCodegen("calls.cl"); }
// TEST(CodegenTest, casenone) { TestCodegen("case-none.cl"); }
// TEST(CodegenTest, caseorder) { TestCodegen("case-order.cl"); }
// TEST(CodegenTest, casevoid) { TestCodegen("casevoid.cl"); }
// TEST(CodegenTest, cells) { TestCodegen("cells.cl"); }
// TEST(CodegenTest, copyselfdispatch) { TestCodegen("copy-self-dispatch.cl"); }
// TEST(CodegenTest, copyselfinit) { TestCodegen("copy-self-init.cl"); }
TEST(CodegenTest, dispatchoverridedynamic) {
  TestCodegen("dispatch-override-dynamic.cl");
}
// TEST(CodegenTest, dispatchoverridestatic) {
//  TestCodegen("dispatch-override-static.cl");
//}
// TEST(CodegenTest, dispatchvoiddynamic) {
//  TestCodegen("dispatch-void-dynamic.cl");
//}
// TEST(CodegenTest, dispatchvoidstatic) {
//  TestCodegen("dispatch-void-static.cl");
//}
// TEST(CodegenTest, dispatchvoidlet) { TestCodegen("dispatchvoidlet.cl"); }
TEST(CodegenTest, evalorderargs) { TestCodegen("eval-order-args.cl"); }
// TEST(CodegenTest, evalorderarith) { TestCodegen("eval-order-arith.cl"); }
// TEST(CodegenTest, evalorderself) { TestCodegen("eval-order-self.cl"); }
// TEST(CodegenTest, exp) { TestCodegen("exp.cl"); }
// TEST(CodegenTest, fact) { TestCodegen("fact.cl"); }
TEST(CodegenTest, fibo) { TestCodegen("fibo.cl"); }
// TEST(CodegenTest, hairyscary) { TestCodegen("hairyscary.cl"); }
// TEST(CodegenTest, initdefault) { TestCodegen("init-default.cl"); }
TEST(CodegenTest, initorderself) { TestCodegen("init-order-self.cl"); }
// TEST(CodegenTest, initordersuper) { TestCodegen("init-order-super.cl"); }
TEST(CodegenTest, interactionattrinitmethod) {
  TestCodegen("interaction-attrinit-method.cl");
}
// TEST(CodegenTest, lamgc) { TestCodegen("lam-gc.cl"); }
// TEST(CodegenTest, lam) { TestCodegen("lam.cl"); }
// TEST(CodegenTest, letnested) { TestCodegen("let-nested.cl"); }
TEST(CodegenTest, letinit) { TestCodegen("letinit.cl"); }
// TEST(CodegenTest, many_objects_on_heap) {
//  TestCodegen("many_objects_on_heap.cl");
//}
// TEST(CodegenTest, modparam) { TestCodegen("mod-param.cl"); }
TEST(CodegenTest, modparamnointobjcast) {
  TestCodegen("mod-param-no-int-obj-cast.cl");
}
// TEST(CodegenTest, multipledispatch) { TestCodegen("multiple-dispatch.cl"); }
// TEST(CodegenTest, multiplestaticdispatch) {
//  TestCodegen("multiple-static-dispatch.cl");
//}
// TEST(CodegenTest, nestedarith) { TestCodegen("nested-arith.cl"); }
// TEST(CodegenTest, newselfdispatch) { TestCodegen("new-self-dispatch.cl"); }
// TEST(CodegenTest, newselfinit) { TestCodegen("new-self-init.cl"); }
// TEST(CodegenTest, newst) { TestCodegen("new-st.cl"); }
// TEST(CodegenTest, newbasic) { TestCodegen("newbasic.cl"); }
TEST(CodegenTest, nonpolycall) { TestCodegen("non-poly-call.cl"); }
// TEST(CodegenTest, not) { TestCodegen("not.cl"); }
// TEST(CodegenTest, objectequality) { TestCodegen("objectequality.cl"); }
// TEST(CodegenTest, overridebasic) { TestCodegen("override-basic.cl"); }
// TEST(CodegenTest, overridecl) { TestCodegen("override.cl"); }
// TEST(CodegenTest, primes) { TestCodegen("primes.cl"); }
// TEST(CodegenTest, recclass) { TestCodegen("recclass.cl"); }
// TEST(CodegenTest, scoping) { TestCodegen("scoping.cl"); }
// TEST(CodegenTest, selftypeattribute) { TestCodegen("selftypeattribute.cl"); }
TEST(CodegenTest, sequence) { TestCodegen("sequence.cl"); }
// TEST(CodegenTest, shadowattrcase) { TestCodegen("shadow-attr-case.cl"); }
TEST(CodegenTest, shadowattrformal) { TestCodegen("shadow-attr-formal.cl"); }
// TEST(CodegenTest, shadowattrlet) { TestCodegen("shadow-attr-let.cl"); }
// TEST(CodegenTest, shadowcaselet) { TestCodegen("shadow-case-let.cl"); }
// TEST(CodegenTest, shadowformalcase) { TestCodegen("shadow-formal-case.cl"); }
// TEST(CodegenTest, shadowformallet) { TestCodegen("shadow-formal-let.cl"); }
// TEST(CodegenTest, shadowletcase) { TestCodegen("shadow-let-case.cl"); }
TEST(CodegenTest, shadowletlet) { TestCodegen("shadow-let-let.cl"); }
TEST(CodegenTest, simplegc) { TestCodegen("simple-gc.cl"); }
// TEST(CodegenTest, sort_list) { TestCodegen("sort_list.cl"); }
TEST(CodegenTest, stringmethods) { TestCodegen("string-methods.cl"); }
// TEST(CodegenTest, typenamecl) { TestCodegen("typename.cl"); }
// TEST(CodegenTest, whileval) { TestCodegen("while-val.cl"); }

}  // namespace
