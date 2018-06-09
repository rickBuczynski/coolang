#ifndef COOLANG_PARSER_AST_H
#define COOLANG_PARSER_AST_H

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace coolang {
namespace ast {

class LineRange {
 public:
  LineRange(int start_line_num, int end_line_num)
      : start_line_num(start_line_num), end_line_num(end_line_num) {}

  std::string ToString() const { return "#" + std::to_string(end_line_num); }

  int start_line_num;
  int end_line_num;
};

class Expr {};

class AssignExpr : public Expr {
 public:
  AssignExpr(std::string id, std::unique_ptr<Expr> rhs_expr,
             LineRange line_range)
      : id_(std::move(id)),
        rhs_expr_(std::move(rhs_expr)),
        line_range_(line_range) {}

 private:
  std::string id_;
  std::unique_ptr<Expr> rhs_expr_;
  LineRange line_range_;
};

class IntExpr : public Expr {
 public:
  IntExpr(std::string val, LineRange line_range)
      : val_(std::move(val)), line_range_(line_range) {}

 private:
  std::string val_;
  LineRange line_range_;
};

class Formal {
 public:
  Formal(std::string id, std::string type, LineRange line_range)
      : id_(std::move(id)), type_(std::move(type)), line_range_(line_range) {}

  std::string GetId() const { return id_; }
  std::string GetType() const { return type_; }
  LineRange GetLineRange() const { return line_range_; }

  std::string ToString() const;

 private:
  std::string id_;
  std::string type_;
  LineRange line_range_;
};

class MethodFeature {
 public:
  MethodFeature(std::string id, std::string return_type, LineRange line_range)
      : id_(std::move(id)),
        return_type_(std::move(return_type)),
        line_range_(line_range) {}

  std::string ToString() const;

 private:
  std::string id_;
  // TODO arg list
  std::string return_type_;
  std::unique_ptr<Expr> body_;
  LineRange line_range_;
};

class AttributeFeature {
 public:
  AttributeFeature(std::string id, std::string type, LineRange line_range)
      : id_(std::move(id)), type_(std::move(type)), line_range_(line_range) {}

  std::string ToString() const;

 private:
  std::string id_;
  std::string type_;
  std::optional<Expr> initialization_expr_;
  LineRange line_range_;
};

using Feature = std::variant<MethodFeature, AttributeFeature>;

inline std::string FeatureToString(const Feature& feature) {
  return std::visit([](auto&& arg) -> std::string { return arg.ToString(); },
                    feature);
}

class CoolClass {
 public:
  CoolClass(std::string type, std::optional<std::string> inherits_type,
            std::vector<Feature>&& features,
            LineRange line_range, std::string containing_file_name)
      : type_(std::move(type)),
        inherits_type_(std::move(inherits_type)),
        features_(std::move(features)),
        line_range_(line_range),
        containing_file_name_(std::move(containing_file_name)) {}

  LineRange GetLineRange() const { return line_range_; }

  std::string ToString() const;

  std::string InheritsTypeAsString() const {
    return inherits_type_.value_or("Object");
  }

 private:
  std::string type_;
  std::optional<std::string> inherits_type_;
  std::vector<Feature> features_;
  LineRange line_range_;

  std::string containing_file_name_;
};

class Program {
 public:
  Program(std::vector<CoolClass>&& cool_classes, LineRange line_range)
      : classes_(std::move(cool_classes)), line_range_(line_range) {}

  std::string ToString() const;

 private:
  std::vector<CoolClass> classes_;
  LineRange line_range_;
};

}  // namespace ast
}  // namespace coolang

#endif  // COOLANG_PARSER_AST_H
