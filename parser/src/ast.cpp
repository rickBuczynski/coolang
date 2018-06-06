#include <string>
#include "coolang/parser/ast.h"

std::string Indentation(int indentation_level)
{
  // each indent is 2 spaces
  return std::string(indentation_level*2, ' ');
}

std::string coolang::ast::Formal::ToString() const {
  return "";
}

std::string coolang::ast::AttributeFeature::ToString() const {
  return "";
}

std::string coolang::ast::CoolClass::ToString() const {
  std::string str = Indentation(1) + line_range_.ToString() + '\n';
  str += Indentation(1) + "_class" + '\n';
  str += Indentation(2) + type_ + '\n';
  str += Indentation(2) + InheritsTypeAsString() + '\n';
  str += Indentation(2) + "TODO program name goes here" + '\n';

  return str;
}

std::string coolang::ast::Program::ToString() const {
  using namespace std::string_literals;

  std::string str = line_range_.ToString() + '\n';
  str += "_program"s  + '\n';

  for (const auto& cool_class : classes_) {
    str += cool_class.ToString();
  }

  return str;
}
