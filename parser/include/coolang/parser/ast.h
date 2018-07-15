#ifndef COOLANG_PARSER_AST_H
#define COOLANG_PARSER_AST_H

#include <algorithm>
#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace coolang {

class LineRange {
 public:
  LineRange(int start_line_num, int end_line_num)
      : start_line_num(start_line_num), end_line_num(end_line_num) {}

  std::string ToString() const { return "#" + std::to_string(end_line_num); }
  std::string ToStringStartLine() const {
    return "#" + std::to_string(start_line_num);
  }

  int start_line_num;
  int end_line_num;
};

class AstVisitor;

class AstNode {
 public:
  explicit AstNode(LineRange line_range) : line_range_(line_range) {}
  virtual ~AstNode() = default;

  AstNode(const AstNode& other) = delete;
  AstNode& operator=(const AstNode& other) = delete;

  AstNode(AstNode&& other) noexcept = default;
  AstNode& operator=(AstNode&& other) noexcept = default;

  LineRange GetLineRange() const { return line_range_; }
  void SetLineRange(const LineRange line_range) { line_range_ = line_range; }

  virtual std::string ToString(int indent_depth) const = 0;

  virtual void Accept(AstVisitor& ast_visitor) = 0;

 private:
  LineRange line_range_;
};

class Expr;
class CaseExpr;
class StrExpr;
class WhileExpr;
class LetExpr;
class IntExpr;
class IsVoidExpr;
class MethodCallExpr;
class NotExpr;
class IfExpr;
class NegExpr;
class BlockExpr;
class ObjectExpr;
class BinOpExpr;
class MultiplyExpr;
class LessThanEqualCompareExpr;
class SubtractExpr;
class AddExpr;
class EqCompareExpr;
class DivideExpr;
class LessThanCompareExpr;
class NewExpr;
class AssignExpr;
class BoolExpr;
class ClassAst;
class CaseBranch;
class Feature;
class MethodFeature;
class AttributeFeature;
class ProgramAst;

class AstVisitor {
 public:
  AstVisitor() = default;
  virtual ~AstVisitor() = default;
  AstVisitor(const AstVisitor& other) = default;
  AstVisitor(AstVisitor&& other) noexcept = default;
  AstVisitor& operator=(const AstVisitor& other) = default;
  AstVisitor& operator=(AstVisitor&& other) noexcept = default;

  virtual void Visit(CaseExpr& node) = 0;
  virtual void Visit(StrExpr& node) = 0;
  virtual void Visit(WhileExpr& node) = 0;
  virtual void Visit(LetExpr& node) = 0;
  virtual void Visit(IntExpr& node) = 0;
  virtual void Visit(IsVoidExpr& node) = 0;
  virtual void Visit(MethodCallExpr& node) = 0;
  virtual void Visit(NotExpr& node) = 0;
  virtual void Visit(IfExpr& node) = 0;
  virtual void Visit(NegExpr& node) = 0;
  virtual void Visit(BlockExpr& node) = 0;
  virtual void Visit(ObjectExpr& node) = 0;
  virtual void Visit(BinOpExpr& node) = 0;
  virtual void Visit(MultiplyExpr& node) = 0;
  virtual void Visit(LessThanEqualCompareExpr& node) = 0;
  virtual void Visit(SubtractExpr& node) = 0;
  virtual void Visit(AddExpr& node) = 0;
  virtual void Visit(EqCompareExpr& node) = 0;
  virtual void Visit(DivideExpr& node) = 0;
  virtual void Visit(LessThanCompareExpr& node) = 0;
  virtual void Visit(NewExpr& node) = 0;
  virtual void Visit(AssignExpr& node) = 0;
  virtual void Visit(BoolExpr& node) = 0;
  virtual void Visit(ClassAst& node) = 0;
  virtual void Visit(CaseBranch& node) = 0;
  virtual void Visit(Feature& node) = 0;
  virtual void Visit(MethodFeature& node) = 0;
  virtual void Visit(AttributeFeature& node) = 0;
  virtual void Visit(ProgramAst& node) = 0;
};

class Expr : public AstNode {
 public:
  explicit Expr(LineRange line_range) : AstNode(line_range) {}

  const std::string& GetExprType() const { return expr_type_; }
  void SetExprType(const std::string& expr_type) { expr_type_ = expr_type; }

 private:
  std::string expr_type_ = "_no_type";
};

class AssignExpr : public Expr {
 public:
  AssignExpr(std::string id, std::unique_ptr<Expr> rhs_expr,
             LineRange line_range)
      : Expr(line_range), id_(std::move(id)), rhs_expr_(std::move(rhs_expr)) {}

  std::string ToString(int indent_depth) const override;

  const std::string& GetId() const { return id_; }
  Expr* GetRhsExpr() const { return rhs_expr_.get(); }

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string id_;
  std::unique_ptr<Expr> rhs_expr_;
};

class IntExpr : public Expr {
 public:
  IntExpr(std::string val, LineRange line_range)
      : Expr(line_range), val_(std::move(val)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string val_;
};

class BoolExpr : public Expr {
 public:
  BoolExpr(LineRange line_range, bool val) : Expr(line_range), val_(val) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  bool val_;
};

class StrExpr : public Expr {
 public:
  StrExpr(LineRange line_range, std::string val)
      : Expr(line_range), val_(std::move(val)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string val_;
};

class LetExpr : public Expr {
 public:
  LetExpr(LineRange line_range, std::string id, std::string type,
          std::unique_ptr<Expr> initialization_expr,
          std::unique_ptr<Expr> in_expr, std::unique_ptr<LetExpr> chained_let)
      : Expr(line_range),
        id_(std::move(id)),
        type_(std::move(type)),
        initialization_expr_(std::move(initialization_expr)),
        in_expr_(std::move(in_expr)),
        chained_let_(std::move(chained_let)) {}

  std::string ToString(int indent_depth) const override;

  const std::string& GetId() const { return id_; }
  const std::string& GetType() const { return type_; }
  const std::unique_ptr<Expr>& GetInitializationExpr() const {
    return initialization_expr_;
  }
  const std::unique_ptr<Expr>& GetInExpr() const { return in_expr_; }
  const std::unique_ptr<LetExpr>& GetChainedLet() const { return chained_let_; }

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string id_;
  std::string type_;
  // null if no initialization_expr_
  std::unique_ptr<Expr> initialization_expr_;
  // null if this is not the last let in chain e.g.
  // let x:Int <- 5, y:String <- "biddle", z:Int in x
  // only z:Int in x has non-null in_expr_
  std::unique_ptr<Expr> in_expr_;

  std::unique_ptr<LetExpr> chained_let_;
};

class NegExpr : public Expr {
 public:
  NegExpr(LineRange line_range, std::unique_ptr<Expr> child_expr)
      : Expr(line_range), child_expr_(std::move(child_expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> child_expr_;
};

class BinOpExpr : public Expr {
 public:
  BinOpExpr(LineRange line_range, std::unique_ptr<Expr> lhs_expr,
            std::unique_ptr<Expr> rhs_expr)
      : Expr(line_range),
        lhs_expr_(std::move(lhs_expr)),
        rhs_expr_(std::move(rhs_expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  virtual std::string OpName() const = 0;

 private:
  std::unique_ptr<Expr> lhs_expr_;
  std::unique_ptr<Expr> rhs_expr_;
};

class DivideExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_divide"; }
};

class MultiplyExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_mul"; }
};

class AddExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_plus"; }
};

class SubtractExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_sub"; }
};

class EqCompareExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_eq"; }
};

class LessThanCompareExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_lt"; }
};

class LessThanEqualCompareExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 protected:
  std::string OpName() const override { return "_le"; }
};

class ObjectExpr : public Expr {
 public:
  ObjectExpr(LineRange line_range, std::string id)
      : Expr(line_range), id_(std::move(id)) {}

  std::string ToString(int indent_depth) const override;

  const std::string& GetId() const { return id_; }

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string id_;
};

class BlockExpr : public Expr {
 public:
  BlockExpr(LineRange line_range, std::vector<std::unique_ptr<Expr>> exprs)
      : Expr(line_range), exprs_(std::move(exprs)) {}

  const std::vector<std::unique_ptr<Expr>>& GetExprs() const { return exprs_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::vector<std::unique_ptr<Expr>> exprs_;
};

class MethodCallExpr : public Expr {
 public:
  MethodCallExpr(LineRange line_range, std::unique_ptr<Expr> lhs_expr,
                 std::optional<std::string> static_dispatch_type,
                 std::string method_name,
                 std::vector<std::unique_ptr<Expr>> unique_ptrs)
      : Expr(line_range),
        lhs_expr_(std::move(lhs_expr)),
        static_dispatch_type_(std::move(static_dispatch_type)),
        method_name_(std::move(method_name)),
        args_(std::move(unique_ptrs)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> lhs_expr_;
  std::optional<std::string> static_dispatch_type_;
  std::string method_name_;
  std::vector<std::unique_ptr<Expr>> args_;
};

class IfExpr : public Expr {
 public:
  IfExpr(LineRange line_range, std::unique_ptr<Expr> if_condition_expr,
         std::unique_ptr<Expr> then_expr, std::unique_ptr<Expr> else_expr)
      : Expr(line_range),
        if_condition_expr_(std::move(if_condition_expr)),
        then_expr_(std::move(then_expr)),
        else_expr_(std::move(else_expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> if_condition_expr_;
  std::unique_ptr<Expr> then_expr_;
  std::unique_ptr<Expr> else_expr_;
};

class CaseBranch : public AstNode {
 public:
  CaseBranch(LineRange line_range, std::string id, std::string type,
             std::unique_ptr<Expr> expr)
      : AstNode(line_range),
        id_(std::move(id)),
        type_(std::move(type)),
        expr_(std::move(expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string id_;
  std::string type_;
  std::unique_ptr<Expr> expr_;
};

class CaseExpr : public Expr {
 public:
  CaseExpr(LineRange line_range, std::unique_ptr<Expr> case_expr,
           std::vector<CaseBranch> case_branches)
      : Expr(line_range),
        case_expr_(std::move(case_expr)),
        branches_(std::move(case_branches)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> case_expr_;

  std::vector<CaseBranch> branches_;
};

class WhileExpr : public Expr {
 public:
  WhileExpr(LineRange line_range, std::unique_ptr<Expr> condition_expr,
            std::unique_ptr<Expr> loop_expr)
      : Expr(line_range),
        condition_expr_(std::move(condition_expr)),
        loop_expr_(std::move(loop_expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> condition_expr_;
  std::unique_ptr<Expr> loop_expr_;
};

class NotExpr : public Expr {
 public:
  NotExpr(LineRange line_range, std::unique_ptr<Expr> child_expr)
      : Expr(line_range), child_expr_(std::move(child_expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> child_expr_;
};

class IsVoidExpr : public Expr {
 public:
  IsVoidExpr(LineRange line_range, std::unique_ptr<Expr> child_expr)
      : Expr(line_range), child_expr_(std::move(child_expr)) {}

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unique_ptr<Expr> child_expr_;
};

class NewExpr : public Expr {
 public:
  NewExpr(LineRange line_range, std::string type)
      : Expr(line_range), type_(std::move(type)) {}

  const std::string& GetType() const { return type_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string type_;
};

class Formal {
 public:
  Formal(std::string id, std::string type, LineRange line_range)
      : id_(std::move(id)), type_(std::move(type)), line_range_(line_range) {}

  std::string GetId() const { return id_; }
  std::string GetType() const { return type_; }
  LineRange GetLineRange() const { return line_range_; }

  std::string ToString(int indent_depth) const;

 private:
  std::string id_;
  std::string type_;
  LineRange line_range_;
};

class Feature : public AstNode {
 public:
  Feature(LineRange line_range) : AstNode(line_range) {}
  virtual const std::unique_ptr<Expr>& GetRootExpr() const = 0;
  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }
};

class MethodFeature : public Feature {
 public:
  MethodFeature(LineRange line_range, std::string id, std::vector<Formal> args,
                std::string return_type, std::unique_ptr<Expr> body)
      : Feature(line_range),
        id_(std::move(id)),
        args_(std::move(args)),
        return_type_(std::move(return_type)),
        body_(std::move(body)) {}

  const std::unique_ptr<Expr>& GetRootExpr() const override { return body_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string id_;
  std::vector<Formal> args_;
  std::string return_type_;
  std::unique_ptr<Expr> body_;
};

class AttributeFeature : public Feature {
 public:
  AttributeFeature(LineRange line_range, std::string id, std::string type,
                   std::unique_ptr<Expr> initialization_expr)
      : Feature(line_range),
        id_(std::move(id)),
        type_(std::move(type)),
        initialization_expr_(std::move(initialization_expr)) {}

  const std::unique_ptr<Expr>& GetRootExpr() const override {
    return initialization_expr_;
  }
  const std::string& GetId() const { return id_; }
  const std::string& GetType() const { return type_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string id_;
  std::string type_;
  std::unique_ptr<Expr> initialization_expr_;
};

class ClassAst : public AstNode {
 public:
  ClassAst(std::string type, ClassAst* super_class,
           std::vector<std::unique_ptr<Feature>>&& features,
           LineRange line_range, std::string containing_file_name)
      : AstNode(line_range),
        type_(std::move(type)),
        super_class_(super_class),
        features_(std::move(features)),
        containing_file_name_(std::move(containing_file_name)) {}

  std::string ToString(int indent_depth) const override;

  const std::string& GetContainingFileName() const {
    return containing_file_name_;
  }

  std::string InheritsTypeAsString() const { return super_class_->GetType(); }

  const std::string& GetType() const { return type_; }
  const std::vector<std::unique_ptr<Feature>>& GetFeatures() const {
    return features_;
  }
  std::vector<const AttributeFeature*> GetAttributeFeatures() const {
    std::vector<const AttributeFeature*> attribute_features;
    for (const auto& feature : GetFeatures()) {
      if (auto* attr = dynamic_cast<AttributeFeature*>(feature.get())) {
        attribute_features.push_back(attr);
      }
    }
    return attribute_features;
  }

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::string type_;
  ClassAst* super_class_;
  std::vector<std::unique_ptr<Feature>> features_;
  std::string containing_file_name_;
};

class ProgramAst : public AstNode {
 public:
  ProgramAst(std::vector<ClassAst>&& cool_classes, LineRange line_range)
      : AstNode(line_range), classes_(std::move(cool_classes)) {
    for (const auto& cool_class : classes_) {
      classes_by_name_[cool_class.GetType()] = &cool_class;
    }
  }

  const std::vector<ClassAst>& GetClasses() const { return classes_; }
  const ClassAst& GetClassByName(const std::string& name) const {
    return *classes_by_name_.at(name);
  }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& ast_visitor) override { ast_visitor.Visit(*this); }

 private:
  std::unordered_map<std::string, const ClassAst*> classes_by_name_;
  std::vector<ClassAst> classes_;
};

}  // namespace coolang

#endif  // COOLANG_PARSER_AST_H
