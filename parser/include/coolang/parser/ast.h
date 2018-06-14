#ifndef COOLANG_PARSER_AST_H
#define COOLANG_PARSER_AST_H

#include <optional>
#include <string>
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

class AstNode {
 public:
  AstNode(LineRange line_range) : line_range_(line_range) {}
  virtual ~AstNode() = default;

  AstNode(const AstNode& other) = delete;
  AstNode& operator=(const AstNode& other) = delete;

  AstNode(AstNode&& other) noexcept = default;
  AstNode& operator=(AstNode&& other) noexcept = default;

  LineRange GetLineRange() const { return line_range_; }

  virtual std::string ToString(int indent_depth) const = 0;

 private:
  LineRange line_range_;
};

class Expr : public AstNode {
 public:
  Expr(LineRange line_range) : AstNode(line_range) {}
};

class AssignExpr : public Expr {
 public:
  AssignExpr(std::string id, std::unique_ptr<Expr> rhs_expr,
             LineRange line_range)
      : Expr(line_range), id_(std::move(id)), rhs_expr_(std::move(rhs_expr)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::string id_;
  std::unique_ptr<Expr> rhs_expr_;
};

class IntExpr : public Expr {
 public:
  IntExpr(std::string val, LineRange line_range)
      : Expr(line_range), val_(std::move(val)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::string val_;
};

class LetExpr : public Expr {
 public:
  LetExpr(LineRange line_range, std::string id, std::string type,
          std::unique_ptr<Expr> initialization_expr,
          std::unique_ptr<Expr> in_expr)
      : Expr(line_range),
        id_(std::move(id)),
        type_(std::move(type)),
        initialization_expr_(std::move(initialization_expr)),
        in_expr_(std::move(in_expr)) {}

  std::string ToString(int indent_depth) const override;

 private:
  // TODO make these vectors to allow multiple vars in one let
  std::string id_;
  std::string type_;
  std::unique_ptr<Expr> initialization_expr_;

  std::unique_ptr<Expr> in_expr_;
};

class BinOpExpr : public Expr {
 public:
  BinOpExpr(LineRange line_range, std::unique_ptr<Expr> lhs_expr,
            std::unique_ptr<Expr> rhs_expr)
      : Expr(line_range),
        lhs_expr_(std::move(lhs_expr)),
        rhs_expr_(std::move(rhs_expr)) {}

  std::string ToString(int indent_depth) const override;

 protected:
  virtual std::string OpName() const = 0;

 private:
  std::unique_ptr<Expr> lhs_expr_;
  std::unique_ptr<Expr> rhs_expr_;
};

class AddExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

 protected:
  std::string OpName() const override { return "_plus"; }
};

class MultiplyExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

 protected:
  std::string OpName() const override { return "_mul"; }
};

class ObjectExpr : public Expr {
 public:
  ObjectExpr(LineRange line_range, std::string id)
      : Expr(line_range), id_(std::move(id)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::string id_;
};

class BlockExpr : public Expr {
 public:
  BlockExpr(LineRange line_range, std::vector<std::unique_ptr<Expr>> exprs)
      : Expr(line_range), exprs_(std::move(exprs)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::vector<std::unique_ptr<Expr>> exprs_;
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

class Feature : public AstNode {
 public:
  Feature(LineRange line_range) : AstNode(line_range) {}
};

class MethodFeature : public Feature {
 public:
  MethodFeature(std::string id, std::string return_type,
                std::unique_ptr<Expr> body, LineRange line_range)
      : Feature(line_range),
        id_(std::move(id)),
        return_type_(std::move(return_type)),
        body_(std::move(body)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::string id_;
  // TODO arg list
  std::string return_type_;
  std::unique_ptr<Expr> body_;
};

class AttributeFeature : public Feature {
 public:
  AttributeFeature(std::string id, std::string type, LineRange line_range)
      : Feature(line_range), id_(std::move(id)), type_(std::move(type)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::string id_;
  std::string type_;
  std::unique_ptr<Expr> initialization_expr_;
};

class CoolClass : public AstNode {
 public:
  CoolClass(std::string type, std::optional<std::string> inherits_type,
            std::vector<std::unique_ptr<Feature>>&& features,
            LineRange line_range, std::string containing_file_name)
      : AstNode(line_range),
        type_(std::move(type)),
        inherits_type_(std::move(inherits_type)),
        features_(std::move(features)),
        containing_file_name_(std::move(containing_file_name)) {}

  std::string ToString(int indent_depth) const override;

  std::string InheritsTypeAsString() const {
    return inherits_type_.value_or("Object");
  }

 private:
  std::string type_;
  std::optional<std::string> inherits_type_;
  std::vector<std::unique_ptr<Feature>> features_;
  std::string containing_file_name_;
};

class Program : public AstNode {
 public:
  Program(std::vector<CoolClass>&& cool_classes, LineRange line_range)
      : AstNode(line_range), classes_(std::move(cool_classes)) {}

  std::string ToString(int indent_depth) const override;

 private:
  std::vector<CoolClass> classes_;
};

}  // namespace ast
}  // namespace coolang

#endif  // COOLANG_PARSER_AST_H
