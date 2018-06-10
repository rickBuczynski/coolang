#include <string>
#include "coolang/parser/ast.h"

std::string Indentation(int indentation_level) {
  // each indent is 2 spaces
  return std::string(indentation_level * 2, ' ');
}

std::string coolang::ast::AssignExpr::ToString() const {
  std::string str;

  str += Indentation(3) + GetLineRange().ToString() + '\n';
  str += Indentation(3) + "_assign" + '\n';

  str += Indentation(4) + id_ + '\n';

  str += rhs_expr_->ToString();

  str += Indentation(3) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::IntExpr::ToString() const {
  std::string str;

  str += Indentation(4) + GetLineRange().ToString() + '\n';
  str += Indentation(4) + "_int" + '\n';

  str += Indentation(5) + val_ + '\n';

  str += Indentation(4) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::Formal::ToString() const { return ""; }

std::string coolang::ast::MethodFeature::ToString() const {
  std::string str;

  str += Indentation(2) + GetLineRange().ToString() + '\n';
  str += Indentation(2) + "_method" + '\n';

  str += Indentation(3) + id_ + '\n';
  str += Indentation(3) + return_type_ + '\n';

  str += body_->ToString();

  return str;
}

std::string coolang::ast::AttributeFeature::ToString() const {
  std::string str;

  str += Indentation(2) + GetLineRange().ToString() + '\n';
  str += Indentation(2) + "_attr" + '\n';

  str += Indentation(3) + id_ + '\n';
  str += Indentation(3) + type_ + '\n';

  if (initialization_expr_) {
    // TODO
  } else {
    str += Indentation(3) + GetLineRange().ToString() + '\n';
    str += Indentation(3) + "_no_expr" + '\n';
    str += Indentation(3) + ": _no_type" + '\n';
  }
  return str;
}

std::string coolang::ast::CoolClass::ToString() const {
  std::string str;

  str += Indentation(1) + line_range_.ToString() + '\n';
  str += Indentation(1) + "_class" + '\n';

  str += Indentation(2) + type_ + '\n';
  str += Indentation(2) + InheritsTypeAsString() + '\n';
  str += Indentation(2) + '"' + containing_file_name_ + '"' + '\n';
  str += Indentation(2) + '(' + '\n';

  for (const auto& feature : features_) {
    str += feature->ToString();
  }

  str += Indentation(2) + ')' + '\n';

  return str;
}

std::string coolang::ast::Program::ToString() const {
  using namespace std::string_literals;

  std::string str;

  str += line_range_.ToString() + '\n';
  str += "_program"s + '\n';

  for (const auto& cool_class : classes_) {
    str += cool_class.ToString();
  }

  return str;
}
