#include <string>
#include "coolang/parser/ast.h"

std::string coolang::ast::Formal::ToString() const {
  return "";
}

std::string coolang::ast::AttributeFeature::ToString() const {
  return "";
}

std::string coolang::ast::CoolClass::ToString() const {
  return "" + '\n';
}

std::string coolang::ast::Program::ToString() const {
  using namespace std::string_literals;

  std::string str;
  str += line_range_.ToString() + '\n';
  str += "_program"s  + '\n';

  for (const auto& cool_class : classes_) {
    str += cool_class.ToString();
  }

  return str;
}
