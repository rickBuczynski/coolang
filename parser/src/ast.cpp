#include <string>
#include "coolang/parser/ast.h"

std::string Indentation(int indentation_level) {
  // each indent is 2 spaces
  return std::string(indentation_level * 2, ' ');
}

std::string coolang::ast::AssignExpr::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_assign" + '\n';

  str += Indentation(indent_depth + 1) + id_ + '\n';

  str += rhs_expr_->ToString(indent_depth + 1);

  str += Indentation(indent_depth) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::IntExpr::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_int" + '\n';

  str += Indentation(indent_depth + 1) + val_ + '\n';

  str += Indentation(indent_depth) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::LetExpr::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_let" + '\n';

  str += Indentation(indent_depth + 1) + id_ + '\n';
  str += Indentation(indent_depth + 1) + type_ + '\n';

  if (initialization_expr_) {
    // TODO
  } else {
    str += Indentation(indent_depth + 1) + GetLineRange().ToString() + '\n';
    str += Indentation(indent_depth + 1) + "_no_expr" + '\n';
    str += Indentation(indent_depth + 1) + ": _no_type" + '\n';
  }

  str += in_expr_->ToString(indent_depth + 1);

  str += Indentation(indent_depth) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::AddExpr::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_plus" + '\n';

  str += lhs_expr_->ToString(indent_depth + 1);
  str += rhs_expr_->ToString(indent_depth + 1);

  str += Indentation(indent_depth) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::ObjectExpr::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_object" + '\n';

  str += Indentation(indent_depth + 1) + id_ + '\n';

  str += Indentation(indent_depth) + ": _no_type" + '\n';

  return str;
}

std::string coolang::ast::Formal::ToString() const { return ""; }

std::string coolang::ast::MethodFeature::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_method" + '\n';

  str += Indentation(indent_depth + 1) + id_ + '\n';
  str += Indentation(indent_depth + 1) + return_type_ + '\n';

  str += body_->ToString(indent_depth + 1);

  return str;
}

std::string coolang::ast::AttributeFeature::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_attr" + '\n';

  str += Indentation(indent_depth + 1) + id_ + '\n';
  str += Indentation(indent_depth + 1) + type_ + '\n';

  if (initialization_expr_) {
    // TODO
  } else {
    str += Indentation(indent_depth + 1) + GetLineRange().ToString() + '\n';
    str += Indentation(indent_depth + 1) + "_no_expr" + '\n';
    str += Indentation(indent_depth + 1) + ": _no_type" + '\n';
  }
  return str;
}

std::string coolang::ast::CoolClass::ToString(int indent_depth) const {
  std::string str;

  str += Indentation(indent_depth) + GetLineRange().ToString() + '\n';
  str += Indentation(indent_depth) + "_class" + '\n';

  str += Indentation(indent_depth + 1) + type_ + '\n';
  str += Indentation(indent_depth + 1) + InheritsTypeAsString() + '\n';
  str +=
      Indentation(indent_depth + 1) + '"' + containing_file_name_ + '"' + '\n';
  str += Indentation(indent_depth + 1) + '(' + '\n';

  for (const auto& feature : features_) {
    str += feature->ToString(indent_depth + 1);
  }

  str += Indentation(indent_depth + 1) + ')' + '\n';

  return str;
}

std::string coolang::ast::Program::ToString(int indent_depth) const {
  using namespace std::string_literals;

  std::string str;

  str += GetLineRange().ToString() + '\n';
  str += "_program"s + '\n';

  for (const auto& cool_class : classes_) {
    str += cool_class.ToString(indent_depth + 1);
  }

  return str;
}
