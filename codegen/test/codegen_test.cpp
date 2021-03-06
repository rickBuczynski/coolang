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

#include "coolang/codegen/codegen.h"
#include <filesystem>
#include "coolang/codegen/platform.h"
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

void RunProgram(const std::filesystem::path& in_path,
                const std::filesystem::path& out_path,
                const std::filesystem::path& exe_path,
                bool use_stdin_redirection) {
  std::filesystem::path output_file_path = out_path;
  output_file_path.replace_extension(".out");

  std::string command = exe_path.string() + " > " + output_file_path.string();
  if (use_stdin_redirection) {
    command += " < ";
    command += in_path.string();
    command += ".in";
  }
  std::system(command.c_str());
}

std::string GetProgramOutput(const std::filesystem::path& out_path) {
  std::filesystem::path output_file_path = out_path;
  output_file_path.replace_extension(".out");

  std::ifstream t(output_file_path.string());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::string GetCodgenedProgramOutput(const std::string& input_file_name,
                                     bool use_stdin_redirection,
                                     bool gc_is_verbose) {
  auto lexer = std::make_unique<coolang::Lexer>(CODEGEN_TEST_DATA_PATH +
                                                input_file_name);
  auto parser = std::make_unique<coolang::Parser>(std::move(lexer));
  auto ast = std::get<coolang::ProgramAst>(parser->ParseProgram());

  const coolang::Semantic semantic;
  auto semantic_errors = semantic.CheckProgramSemantics(ast);
  if (!semantic_errors.empty()) {
    return coolang::SemanticError::ToString(semantic_errors);
  }

  std::filesystem::path in_path = ast.GetFilePath();
  const std::filesystem::path out_path =
      in_path.parent_path() / "test_output" / in_path.stem();

  if (!std::filesystem::exists(out_path.parent_path())) {
    std::filesystem::create_directories(out_path.parent_path());
  }

  std::filesystem::path exe_path = out_path;
  exe_path.replace_extension(coolang::platform::GetExeFileExtension());

  std::filesystem::path obj_path = out_path;
  obj_path.replace_extension(coolang::platform::GetObjectFileExtension());

  // Windows won't run an exe with "patch" in the name...
  if (exe_path.filename().string().find("patch") != std::string::npos) {
    const std::string replaced_patch_filename =
        ReplacedPatchWithPaatch(exe_path.filename().string());
    exe_path.replace_filename(replaced_patch_filename);
  }

  const auto codegen =
      std::make_unique<coolang::Codegen>(ast, obj_path, exe_path);
  codegen->GenerateCode(gc_is_verbose);
  codegen->Link();

  RunProgram(in_path, out_path, exe_path, use_stdin_redirection);
  return GetProgramOutput(out_path);
}

void TestCodegen(const std::string& input_file,
                 bool use_stdin_redirection = false,
                 bool gc_is_verbose = false) {
  const std::string program_output = GetCodgenedProgramOutput(
      input_file, use_stdin_redirection, gc_is_verbose);
  std::string expected_output =
      GetExpectedOutput(CODEGEN_TEST_DATA_PATH + input_file + ".out");
  EXPECT_EQ(expected_output, program_output);
}

TEST(CodegenTest, gcnewcopy) { TestCodegen("gc-new-copy.cl", false, true); }

TEST(CodegenTest, gcrootslet) { TestCodegen("gc-roots-let.cl", false, true); }
TEST(CodegenTest, gcrootssupertype) {
  TestCodegen("gc-roots-supertype.cl", false, true);
}
TEST(CodegenTest, gcrootseqcmp) {
  TestCodegen("gc-roots-eqcmp.cl", false, true);
}
TEST(CodegenTest, gcrootsargs) { TestCodegen("gc-roots-args.cl", false, true); }
TEST(CodegenTest, gcrootsreassign) {
  TestCodegen("gc-roots-reassign.cl", false, true);
}
TEST(CodegenTest, gcrootsletstr) {
  TestCodegen("gc-roots-let-str.cl", false, true);
}
TEST(CodegenTest, gccopies) { TestCodegen("gc-copies.cl", false, true); }

TEST(CodegenTest, abort) { TestCodegen("abort.cl"); }
TEST(CodegenTest, abort2) { TestCodegen("abort-2.cl"); }
TEST(CodegenTest, assignmentval) { TestCodegen("assignment-val.cl"); }
TEST(CodegenTest, basicinit) { TestCodegen("basic-init.cl"); }
TEST(CodegenTest, basicequality) { TestCodegen("basicequality.cl"); }
TEST(CodegenTest, bigexample) { TestCodegen("bigexample.cl"); }
TEST(CodegenTest, bigexpr) { TestCodegen("bigexpr.cl"); }
TEST(CodegenTest, book_list) { TestCodegen("book_list.cl"); }
TEST(CodegenTest, boolcl) { TestCodegen("bool.cl"); }
TEST(CodegenTest, boolliteral) { TestCodegen("bool-literal.cl"); }
TEST(CodegenTest, booltypename) { TestCodegen("bool-typename.cl"); }
TEST(CodegenTest, boxedcasting) { TestCodegen("boxed-casting.cl"); }
TEST(CodegenTest, boxedtypename) { TestCodegen("boxed-typename.cl"); }
TEST(CodegenTest, c) { TestCodegen("c.cl"); }
TEST(CodegenTest, calls) { TestCodegen("calls.cl"); }
TEST(CodegenTest, casenone) { TestCodegen("case-none.cl"); }
TEST(CodegenTest, caseorder) { TestCodegen("case-order.cl"); }
TEST(CodegenTest, casevoid) { TestCodegen("casevoid.cl"); }
TEST(CodegenTest, cells) { TestCodegen("cells.cl"); }
TEST(CodegenTest, copyobject) { TestCodegen("copy-object.cl"); }
TEST(CodegenTest, copyselfdispatch) { TestCodegen("copy-self-dispatch.cl"); }
TEST(CodegenTest, copyselfinit) { TestCodegen("copy-self-init.cl"); }
TEST(CodegenTest, dispatchoverridedynamic) {
  TestCodegen("dispatch-override-dynamic.cl");
}
TEST(CodegenTest, dispatchoverridedynamiccomplex) {
  TestCodegen("dispatch-override-dynamic-complex.cl");
}
TEST(CodegenTest, dispatchoverridestatic) {
  TestCodegen("dispatch-override-static.cl");
}
TEST(CodegenTest, dispatchvoiddynamic) {
  TestCodegen("dispatch-void-dynamic.cl");
}
TEST(CodegenTest, dispatchvoidstatic) {
  TestCodegen("dispatch-void-static.cl");
}
TEST(CodegenTest, dispatchvoidlet) { TestCodegen("dispatchvoidlet.cl"); }
TEST(CodegenTest, evalorderargs) { TestCodegen("eval-order-args.cl"); }
TEST(CodegenTest, evalorderarith) { TestCodegen("eval-order-arith.cl"); }
TEST(CodegenTest, evalorderself) { TestCodegen("eval-order-self.cl"); }
TEST(CodegenTest, exp) { TestCodegen("exp.cl"); }
TEST(CodegenTest, fact) { TestCodegen("fact.cl"); }
TEST(CodegenTest, fibo) { TestCodegen("fibo.cl"); }
TEST(CodegenTest, hairyscary) { TestCodegen("hairyscary.cl"); }
TEST(CodegenTest, initdefault) { TestCodegen("init-default.cl"); }
TEST(CodegenTest, inint) { TestCodegen("in-int.cl", true); }
TEST(CodegenTest, initorderself) { TestCodegen("init-order-self.cl"); }
TEST(CodegenTest, initordersuper) { TestCodegen("init-order-super.cl"); }
TEST(CodegenTest, instring) { TestCodegen("in-string.cl", true); }
TEST(CodegenTest, instringeof) { TestCodegen("in-string-eof.cl", true); }
TEST(CodegenTest, intcopytypenameabort) {
  TestCodegen("int-copy-typename-abort.cl");
}
TEST(CodegenTest, interactionattrinitmethod) {
  TestCodegen("interaction-attrinit-method.cl");
}
TEST(CodegenTest, lamgc) { TestCodegen("lam-gc.cl"); }
TEST(CodegenTest, lam) { TestCodegen("lam.cl"); }
TEST(CodegenTest, lesseqcmp) { TestCodegen("less-eq-cmp.cl"); }
TEST(CodegenTest, letnested) { TestCodegen("let-nested.cl"); }
TEST(CodegenTest, letuseprev) { TestCodegen("let-use-prev.cl"); }
TEST(CodegenTest, letinit) { TestCodegen("letinit.cl"); }
TEST(CodegenTest, many_objects_on_heap) {
  TestCodegen("many_objects_on_heap.cl");
}
TEST(CodegenTest, modparam) { TestCodegen("mod-param.cl"); }
TEST(CodegenTest, modparamnointobjcast) {
  TestCodegen("mod-param-no-int-obj-cast.cl");
}
TEST(CodegenTest, multipledispatch) { TestCodegen("multiple-dispatch.cl"); }
TEST(CodegenTest, multiplestaticdispatch) {
  TestCodegen("multiple-static-dispatch.cl");
}
TEST(CodegenTest, nestedarith) { TestCodegen("nested-arith.cl"); }
TEST(CodegenTest, newselfdispatch) { TestCodegen("new-self-dispatch.cl"); }
TEST(CodegenTest, newselfinit) { TestCodegen("new-self-init.cl"); }
TEST(CodegenTest, newst) { TestCodegen("new-st.cl"); }
TEST(CodegenTest, newbasic) { TestCodegen("newbasic.cl"); }
TEST(CodegenTest, nonpolycall) { TestCodegen("non-poly-call.cl"); }
TEST(CodegenTest, not) { TestCodegen("not.cl"); }
TEST(CodegenTest, not2) { TestCodegen("not-2.cl"); }
TEST(CodegenTest, objectequality) { TestCodegen("objectequality.cl"); }
TEST(CodegenTest, overridebasic) { TestCodegen("override-basic.cl"); }
TEST(CodegenTest, overridecl) { TestCodegen("override.cl"); }
TEST(CodegenTest, primes) { TestCodegen("primes.cl"); }
TEST(CodegenTest, recclass) { TestCodegen("recclass.cl"); }
TEST(CodegenTest, scoping) { TestCodegen("scoping.cl"); }
TEST(CodegenTest, selftypeattribute) { TestCodegen("selftypeattribute.cl"); }
TEST(CodegenTest, sequence) { TestCodegen("sequence.cl"); }
TEST(CodegenTest, shadowattrcase) { TestCodegen("shadow-attr-case.cl"); }
TEST(CodegenTest, shadowattrformal) { TestCodegen("shadow-attr-formal.cl"); }
TEST(CodegenTest, shadowattrlet) { TestCodegen("shadow-attr-let.cl"); }
TEST(CodegenTest, shadowcaselet) { TestCodegen("shadow-case-let.cl"); }
TEST(CodegenTest, shadowformalcase) { TestCodegen("shadow-formal-case.cl"); }
TEST(CodegenTest, shadowformallet) { TestCodegen("shadow-formal-let.cl"); }
TEST(CodegenTest, shadowletcase) { TestCodegen("shadow-let-case.cl"); }
TEST(CodegenTest, shadowletlet) { TestCodegen("shadow-let-let.cl"); }
TEST(CodegenTest, simplegc) { TestCodegen("simple-gc.cl"); }
TEST(CodegenTest, sort_list) { TestCodegen("sort_list.cl"); }
TEST(CodegenTest, stringmethods) { TestCodegen("string-methods.cl"); }
TEST(CodegenTest, typenamecl) { TestCodegen("typename.cl"); }
TEST(CodegenTest, typenameobject) { TestCodegen("typename-object-io.cl"); }
TEST(CodegenTest, whileloop) { TestCodegen("whileloop.cl"); }
TEST(CodegenTest, whileval) { TestCodegen("while-val.cl"); }

}  // namespace
