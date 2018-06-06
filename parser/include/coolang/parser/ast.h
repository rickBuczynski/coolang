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

  const int start_line_num;
  const int end_line_num;
};

class Expr {
  // TODO
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
  const std::string id_;
  const std::string type_;
  const LineRange line_range_;
};

class MethodFeature {
  // TODO
 public:
  std::string ToString() const;
};

class AttributeFeature {
 public:
  AttributeFeature(std::string id, std::string type,
                   std::optional<Expr> initialization_expr,
                   LineRange line_range)
      : id_(std::move(id)),
        type_(std::move(type)),
        initialization_expr_(initialization_expr),
        line_range_(line_range) {}

  std::string ToString() const;

 private:
  const std::string id_;
  const std::string type_;
  const std::optional<Expr> initialization_expr_;
  const LineRange line_range_;
};

using Feature = std::variant<MethodFeature, AttributeFeature>;

inline std::string FeatureToString(Feature feature) {
  return std::visit([](auto&& arg) -> std::string { return arg.ToString(); },
                    feature);
}

class CoolClass {
 public:
  CoolClass(std::string type, std::optional<std::string> inherits_type,
            std::vector<Feature> features, LineRange line_range)
      : type_(std::move(type)),
        inherits_type_(std::move(inherits_type)),
        features_(std::move(features)),
        line_range_(line_range) {}

  LineRange GetLineRange() const { return line_range_; }

  std::string ToString() const;

  std::string InheritsTypeAsString() const {
    return inherits_type_.value_or("Object");
  }

 private:
  const std::string type_;
  const std::optional<std::string> inherits_type_;
  const std::vector<Feature> features_;
  const LineRange line_range_;
};

class Program {
 public:
  Program(std::vector<CoolClass> cool_classes, LineRange line_range)
      : classes_(std::move(cool_classes)), line_range_(line_range) {}

  std::string ToString() const;

 private:
  const std::vector<CoolClass> classes_;
  const LineRange line_range_;
};

}  // namespace ast
}  // namespace coolang

#endif  // COOLANG_PARSER_AST_H
