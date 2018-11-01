#ifndef COOLANG_PARSER_AST_H
#define COOLANG_PARSER_AST_H

#include <algorithm>
#include <filesystem>
#include <optional>
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
class ConstAstVisitor;

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

  virtual void Accept(AstVisitor& vis) = 0;
  virtual void Accept(ConstAstVisitor& vis) const = 0;

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
  virtual void Visit(MethodFeature& node) = 0;
  virtual void Visit(AttributeFeature& node) = 0;
  virtual void Visit(ProgramAst& node) = 0;
};

class ConstAstVisitor {
 public:
  ConstAstVisitor() = default;
  virtual ~ConstAstVisitor() = default;
  ConstAstVisitor(const ConstAstVisitor& other) = default;
  ConstAstVisitor(ConstAstVisitor&& other) noexcept = default;
  ConstAstVisitor& operator=(const ConstAstVisitor& other) = default;
  ConstAstVisitor& operator=(ConstAstVisitor&& other) noexcept = default;

  virtual void Visit(const CaseExpr& node) = 0;
  virtual void Visit(const StrExpr& node) = 0;
  virtual void Visit(const WhileExpr& node) = 0;
  virtual void Visit(const LetExpr& node) = 0;
  virtual void Visit(const IntExpr& node) = 0;
  virtual void Visit(const IsVoidExpr& node) = 0;
  virtual void Visit(const MethodCallExpr& node) = 0;
  virtual void Visit(const NotExpr& node) = 0;
  virtual void Visit(const IfExpr& node) = 0;
  virtual void Visit(const NegExpr& node) = 0;
  virtual void Visit(const BlockExpr& node) = 0;
  virtual void Visit(const ObjectExpr& node) = 0;
  virtual void Visit(const BinOpExpr& node) = 0;
  virtual void Visit(const MultiplyExpr& node) = 0;
  virtual void Visit(const LessThanEqualCompareExpr& node) = 0;
  virtual void Visit(const SubtractExpr& node) = 0;
  virtual void Visit(const AddExpr& node) = 0;
  virtual void Visit(const EqCompareExpr& node) = 0;
  virtual void Visit(const DivideExpr& node) = 0;
  virtual void Visit(const LessThanCompareExpr& node) = 0;
  virtual void Visit(const NewExpr& node) = 0;
  virtual void Visit(const AssignExpr& node) = 0;
  virtual void Visit(const BoolExpr& node) = 0;
  virtual void Visit(const ClassAst& node) = 0;
  virtual void Visit(const CaseBranch& node) = 0;
  virtual void Visit(const MethodFeature& node) = 0;
  virtual void Visit(const AttributeFeature& node) = 0;
  virtual void Visit(const ProgramAst& node) = 0;
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

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::string id_;
  std::unique_ptr<Expr> rhs_expr_;
};

class IntExpr : public Expr {
 public:
  IntExpr(std::string val, LineRange line_range)
      : Expr(line_range), val_(std::move(val)) {}

  int GetValAsInt() const { return std::stoi(val_); }
  std::string GetValAsStr() const { return val_; }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::string val_;
};

class BoolExpr : public Expr {
 public:
  BoolExpr(LineRange line_range, bool val) : Expr(line_range), val_(val) {}

  bool GetVal() const { return val_; }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  bool val_;
};

class StrExpr : public Expr {
 public:
  StrExpr(LineRange line_range, std::string val)
      : Expr(line_range), val_(std::move(val)) {}

  const std::string& GetVal() const { return val_; }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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
  
  const Expr* GetInitializationExpr() const {
    return initialization_expr_.get();
  }
  Expr* MutableInitializationExpr() { return initialization_expr_.get(); }

  const Expr* GetInExpr() const { return in_expr_.get(); }
  Expr* MutableInExpr() { return in_expr_.get(); }

  const LetExpr* GetChainedLet() const { return chained_let_.get(); }
  LetExpr* MutableChainedLet()  { return chained_let_.get(); }

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetChildExpr() const { return child_expr_.get(); }
  Expr* MutableChildExpr() { return child_expr_.get(); }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetLhsExpr() const { return lhs_expr_.get(); }
  const Expr* GetRhsExpr() const { return rhs_expr_.get(); }
  Expr* MutableLhsExpr() { return lhs_expr_.get(); }
  Expr* MutableRhsExpr() { return rhs_expr_.get(); }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  virtual std::string OpName() const = 0;

 private:
  std::unique_ptr<Expr> lhs_expr_;
  std::unique_ptr<Expr> rhs_expr_;
};

class DivideExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_divide"; }
};

class MultiplyExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_mul"; }
};

class AddExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_plus"; }
};

class SubtractExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_sub"; }
};

class EqCompareExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_eq"; }
};

class LessThanCompareExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_lt"; }
};

class LessThanEqualCompareExpr : public BinOpExpr {
 public:
  using BinOpExpr::BinOpExpr;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 protected:
  std::string OpName() const override { return "_leq"; }
};

class ObjectExpr : public Expr {
 public:
  ObjectExpr(LineRange line_range, std::string id)
      : Expr(line_range), id_(std::move(id)) {}

  std::string ToString(int indent_depth) const override;

  const std::string& GetId() const { return id_; }

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::string id_;
};

class BlockExpr : public Expr {
 public:
  BlockExpr(LineRange line_range, std::vector<std::unique_ptr<Expr>> exprs)
      : Expr(line_range), exprs_(std::move(exprs)) {}

  const std::vector<std::unique_ptr<Expr>>& GetExprs() const { return exprs_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const std::string& GetMethodName() const { return method_name_; }
  const Expr* GetLhsExpr() const { return lhs_expr_.get(); }
  Expr* MutableLhsExpr() { return lhs_expr_.get(); }
  const std::vector<std::unique_ptr<Expr>>& GetArgs() const { return args_; }
  std::vector<std::unique_ptr<Expr>> const& MutableArgs() { return args_; }
  const std::optional<std::string>& GetStaticDispatchType() const {
    return static_dispatch_type_;
  }

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetIfConditionExpr() const { return if_condition_expr_.get(); }
  const Expr* GetThenExpr() const { return then_expr_.get(); }
  const Expr* GetElseExpr() const { return else_expr_.get(); }

  Expr* MutableIfConditionExpr() const { return if_condition_expr_.get(); }
  Expr* MutableThenExpr() const { return then_expr_.get(); }
  Expr* MutableElseExpr() const { return else_expr_.get(); }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetExpr() const { return expr_.get(); }
  Expr* MutableExpr() { return expr_.get(); }
  const std::string& GetId() const { return id_; }
  const std::string& GetType() const { return type_; }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetCaseExpr() const { return case_expr_.get(); }
  const std::vector<CaseBranch>& GetBranches() const { return branches_; }

  Expr* MutableCaseExpr() { return case_expr_.get(); }
  std::vector<CaseBranch>& MutableBranches() { return branches_; }

  std::unordered_map<std::string, const CaseBranch*> BranchesByType() const {
    std::unordered_map<std::string, const CaseBranch*> branches_by_type;
    for (const CaseBranch& branch : branches_) {
      branches_by_type[branch.GetType()] = &branch;
    }
    return branches_by_type;
  }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetConditionExpr() const { return condition_expr_.get(); }
  const Expr* GetLoopExpr() const { return loop_expr_.get(); }
  Expr* MutableConditionExpr() { return condition_expr_.get(); }
  Expr* MutableLoopExpr() { return loop_expr_.get(); }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::unique_ptr<Expr> condition_expr_;
  std::unique_ptr<Expr> loop_expr_;
};

class NotExpr : public Expr {
 public:
  NotExpr(LineRange line_range, std::unique_ptr<Expr> child_expr)
      : Expr(line_range), child_expr_(std::move(child_expr)) {}

  const Expr* GetChildExpr() const { return child_expr_.get(); }
  Expr* MutableChildExpr() { return child_expr_.get(); }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::unique_ptr<Expr> child_expr_;
};

class IsVoidExpr : public Expr {
 public:
  IsVoidExpr(LineRange line_range, std::unique_ptr<Expr> child_expr)
      : Expr(line_range), child_expr_(std::move(child_expr)) {}

  const Expr* GetChildExpr() const { return child_expr_.get(); }
  Expr* MutableChildExpr() { return child_expr_.get(); }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::unique_ptr<Expr> child_expr_;
};

class NewExpr : public Expr {
 public:
  NewExpr(LineRange line_range, std::string type)
      : Expr(line_range), type_(std::move(type)) {}

  const std::string& GetType() const { return type_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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
  virtual const Expr* GetRootExpr() const = 0;
  virtual Expr* MutableRootExpr() = 0;
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

  const Expr* GetRootExpr() const override { return body_.get(); }
  Expr* MutableRootExpr() override { return body_.get(); }

  const std::string& GetId() const { return id_; }
  const std::vector<Formal>& GetArgs() const { return args_; }
  const std::string& GetReturnType() const { return return_type_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

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

  const Expr* GetRootExpr() const override {
    return initialization_expr_.get();
  }
  Expr* MutableRootExpr() override { return initialization_expr_.get(); }

  const std::string& GetId() const { return id_; }
  const std::string& GetType() const { return type_; }

  std::string ToString(int indent_depth) const override;

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::string id_;
  std::string type_;
  std::unique_ptr<Expr> initialization_expr_;
};

class ClassAst : public AstNode {
 public:
  ClassAst(std::string name, ClassAst* super_class,
           std::vector<std::unique_ptr<Feature>>&& features,
           LineRange line_range, std::filesystem::path containing_file_path)
      : AstNode(line_range),
        name_(std::move(name)),
        super_class_(super_class),
        features_(std::move(features)),
        containing_file_path_(std::move(containing_file_path)) {}

  std::string ToString(int indent_depth) const override;

  std::filesystem::path GetContainingFilePath() const {
    return containing_file_path_.filename().string();
  }

  const ClassAst* GetSuperClass() const { return super_class_; }

  std::vector<const ClassAst*> GetAllSuperClasses() const {
    if (GetSuperClass() == nullptr) {
      return {};
    }

    std::vector<const ClassAst*> super_classes;
    super_classes.push_back(GetSuperClass());
    while (super_classes.back()->GetSuperClass() != nullptr) {
      super_classes.push_back(super_classes.back()->GetSuperClass());
    }
    return super_classes;
  }

  std::vector<const ClassAst*> SupersThenThis() const {
    std::vector<const ClassAst*> supers_and_class = GetAllSuperClasses();
    std::reverse(supers_and_class.begin(), supers_and_class.end());
    supers_and_class.push_back(this);

    return supers_and_class;
  }

  void AddSubClass(const ClassAst* sub_class) {
    sub_classes_.push_back(sub_class);
  }

  const std::vector<const ClassAst*>& GetSubClasses() const {
    return sub_classes_;
  }

  std::vector<const ClassAst*> AllDescendantClasses() const {
    if (sub_classes_.empty()) {
      return {};
    }

    std::vector<const ClassAst*> descendants;
    for (const ClassAst* sub_class : sub_classes_) {
      descendants.push_back(sub_class);
      for (const ClassAst* sub_class_descendant :
           sub_class->AllDescendantClasses()) {
        descendants.push_back(sub_class_descendant);
      }
    }
    return descendants;
  }

  void SetSuperClass(const ClassAst* super_class) {
    super_class_ = super_class;
  }
  const std::string& GetName() const { return name_; }
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
  std::vector<const MethodFeature*> GetMethodFeatures() const {
    std::vector<const MethodFeature*> method_features;
    for (const auto& feature : GetFeatures()) {
      if (auto* method_feature = dynamic_cast<MethodFeature*>(feature.get())) {
        method_features.push_back(method_feature);
      }
    }
    return method_features;
  }

  const MethodFeature* GetMethodFeatureByName(
      const std::string& method_name) const {
    for (const auto* method_feature : GetMethodFeatures()) {
      if (method_feature->GetId() == method_name) {
        return method_feature;
      }
    }
    if (super_class_ == nullptr) {
      return nullptr;
    }
    return super_class_->GetMethodFeatureByName(method_name);
  }

  const ClassAst* GetClassOrSuperClassThatDefinesMethod(
      const MethodFeature* method) const {
    for (const auto* method_feature : GetMethodFeatures()) {
      if (method_feature == method) {
        return this;
      }
    }
    if (super_class_ == nullptr) {
      return nullptr;
    }
    return super_class_->GetClassOrSuperClassThatDefinesMethod(method);
  }

  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  std::string name_;
  const ClassAst* super_class_;
  std::vector<const ClassAst*> sub_classes_;
  std::vector<std::unique_ptr<Feature>> features_;
  std::filesystem::path containing_file_path_;
};

class ProgramAst : public AstNode {
 public:
  ProgramAst(const std::filesystem::path& file_path,
             std::vector<ClassAst>&& cool_classes, LineRange line_range)
      : AstNode(line_range),
        file_path_(file_path),
        classes_(std::move(cool_classes)),
        object_class_(std::make_unique<ClassAst>(
            "Object", nullptr, ObjectClassFeatures(), LineRange(0, 0),
            file_path.filename().string())),
        io_class_(std::make_unique<ClassAst>("IO", object_class_.get(),
                                             IoClassFeatures(), LineRange(0, 0),
                                             file_path.filename().string())),
        int_class_(std::make_unique<ClassAst>(
            "Int", object_class_.get(), IntClassFeatures(), LineRange(0, 0),
            file_path.filename().string())),
        string_class_(std::make_unique<ClassAst>(
            "String", object_class_.get(), StringClassFeatures(),
            LineRange(0, 0), file_path.filename().string())),
        bool_class_(std::make_unique<ClassAst>(
            "Bool", object_class_.get(), BoolClassFeatures(), LineRange(0, 0),
            file_path.filename().string())) {
    for (auto& cool_class : classes_) {
      classes_by_name_[cool_class.GetName()] = &cool_class;
    }
    classes_by_name_[object_class_->GetName()] = object_class_.get();
    classes_by_name_[io_class_->GetName()] = io_class_.get();

    classes_by_name_[int_class_->GetName()] = int_class_.get();
    classes_by_name_[string_class_->GetName()] = string_class_.get();
    classes_by_name_[bool_class_->GetName()] = bool_class_.get();

    object_class_->AddSubClass(int_class_.get());
    object_class_->AddSubClass(string_class_.get());
    object_class_->AddSubClass(bool_class_.get());
    object_class_->AddSubClass(io_class_.get());
  }

  const std::vector<ClassAst>& GetClasses() const { return classes_; }
  std::vector<ClassAst>& MutableClasses() { return classes_; }
  const ClassAst* GetClassByName(const std::string& name) const {
    const auto class_ast = classes_by_name_.find(name);
    if (class_ast == classes_by_name_.end()) {
      return nullptr;
    } else {
      return class_ast->second;
    }
  }
  ClassAst* MutableClassByName(const std::string& name) {
    const auto class_ast = classes_by_name_.find(name);
    if (class_ast == classes_by_name_.end()) {
      return nullptr;
    } else {
      return class_ast->second;
    }
  }

  // Only includes user defined classes
  std::vector<const ClassAst*> SortedClassesWithSupersBeforeSubs() const {
    std::vector<const ClassAst*> sorted;

    for (auto* obj_sub_class : object_class_->GetSubClasses()) {
      if (obj_sub_class != io_class_.get() &&
          obj_sub_class != int_class_.get() &&
          obj_sub_class != string_class_.get() &&
          obj_sub_class != bool_class_.get()) {
        sorted.push_back(obj_sub_class);
      }
    }
    for (auto* io_sub_class : io_class_->GetSubClasses()) {
      sorted.push_back(io_sub_class);
    }
    for (size_t i = 0; i < sorted.size(); i++) {
      for (auto* sub_class : sorted.at(i)->GetSubClasses()) {
        sorted.push_back(sub_class);
      }
    }

    return sorted;
  }

  std::string GetFileName() const { return file_path_.filename().string(); }
  const std::filesystem::path& GetFilePath() const { return file_path_; }
  const ClassAst* GetObjectClass() const { return object_class_.get(); }
  const ClassAst* GetIoClass() const { return io_class_.get(); }
  const ClassAst* GetStringClass() const { return string_class_.get(); }
  const ClassAst* GetIntClass() const { return int_class_.get(); }
  const ClassAst* GetBoolClass() const { return bool_class_.get(); }

  std::string ToString(int indent_depth) const override;
  void Accept(AstVisitor& vis) override { vis.Visit(*this); }
  void Accept(ConstAstVisitor& vis) const override { vis.Visit(*this); }

 private:
  static std::vector<std::unique_ptr<Feature>> ObjectClassFeatures() {
    std::unique_ptr<Feature> abort_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "abort",
                                        std::vector<Formal>{}, "Object",
                                        std::unique_ptr<Expr>{});
    std::unique_ptr<Feature> type_name_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "type_name",
                                        std::vector<Formal>{}, "String",
                                        std::unique_ptr<Expr>{});
    std::unique_ptr<Feature> copy_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "copy",
                                        std::vector<Formal>{}, "SELF_TYPE",
                                        std::unique_ptr<Expr>{});

    std::vector<std::unique_ptr<Feature>> features;
    features.push_back(std::move(abort_method_feature));
    features.push_back(std::move(type_name_method_feature));
    features.push_back(std::move(copy_method_feature));
    return features;
  }

  static std::vector<std::unique_ptr<Feature>> IoClassFeatures() {
    std::vector<Formal> out_string_args{Formal("x", "String", LineRange(0, 0))};
    std::unique_ptr<Feature> out_string_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "out_string",
                                        out_string_args, "SELF_TYPE",
                                        std::unique_ptr<Expr>{});

    std::vector<Formal> out_int_args{Formal("x", "Int", LineRange(0, 0))};
    std::unique_ptr<Feature> out_int_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "out_int",
                                        out_int_args, "SELF_TYPE",
                                        std::unique_ptr<Expr>{});

    std::unique_ptr<Feature> in_string_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "in_string",
                                        std::vector<Formal>{}, "String",
                                        std::unique_ptr<Expr>{});

    std::unique_ptr<Feature> in_int_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "in_int",
                                        std::vector<Formal>{}, "Int",
                                        std::unique_ptr<Expr>{});

    std::vector<std::unique_ptr<Feature>> features;
    features.push_back(std::move(out_string_method_feature));
    features.push_back(std::move(out_int_method_feature));
    features.push_back(std::move(in_string_method_feature));
    features.push_back(std::move(in_int_method_feature));
    return features;
  }

  static std::vector<std::unique_ptr<Feature>> StringClassFeatures() {
    std::unique_ptr<Feature> length_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "length",
                                        std::vector<Formal>{}, "Int",
                                        std::unique_ptr<Expr>{});

    std::vector<Formal> concat_args{Formal("s", "String", LineRange(0, 0))};
    std::unique_ptr<Feature> concat_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "concat", concat_args,
                                        "String", std::unique_ptr<Expr>{});

    std::vector<Formal> substr_args{Formal("i", "Int", LineRange(0, 0)),
                                    Formal("l", "Int", LineRange(0, 0))};
    std::unique_ptr<Feature> substr_method_feature =
        std::make_unique<MethodFeature>(LineRange(0, 0), "substr", substr_args,
                                        "String", std::unique_ptr<Expr>{});

    std::vector<std::unique_ptr<Feature>> obj_features = ObjectClassFeatures();
    std::vector<std::unique_ptr<Feature>> features;
    features.push_back(std::move(obj_features.at(0)));  // overrides abort
    features.push_back(std::move(obj_features.at(1)));  // overrides typename
    features.push_back(std::move(obj_features.at(2)));  // overrides copy
    features.push_back(std::move(length_method_feature));
    features.push_back(std::move(concat_method_feature));
    features.push_back(std::move(substr_method_feature));
    return features;
  }

  static std::vector<std::unique_ptr<Feature>> IntClassFeatures() {
    std::vector<std::unique_ptr<Feature>> obj_features = ObjectClassFeatures();
    std::vector<std::unique_ptr<Feature>> features;
    features.push_back(std::move(obj_features.at(0)));  // overrides abort
    features.push_back(std::move(obj_features.at(1)));  // overrides typename
    features.push_back(std::move(obj_features.at(2)));  // overrides copy
    return features;
  }

  static std::vector<std::unique_ptr<Feature>> BoolClassFeatures() {
    std::vector<std::unique_ptr<Feature>> obj_features = ObjectClassFeatures();
    std::vector<std::unique_ptr<Feature>> features;
    features.push_back(std::move(obj_features.at(0)));  // overrides abort
    features.push_back(std::move(obj_features.at(1)));  // overrides typename
    features.push_back(std::move(obj_features.at(2)));  // overrides copy
    return features;
  }

  std::filesystem::path file_path_;
  std::vector<ClassAst> classes_;

  // basic classes you can inherit from
  // object is the root of inheritance tree, so it must be first
  std::unique_ptr<ClassAst> object_class_;
  std::unique_ptr<ClassAst> io_class_;

  // basic classes you can't inherit from
  std::unique_ptr<ClassAst> int_class_;
  std::unique_ptr<ClassAst> string_class_;
  std::unique_ptr<ClassAst> bool_class_;

  // all classes this points to need to be dynamically allocated
  // or else all the pointers become invalid when this class is moved
  std::unordered_map<std::string, ClassAst*> classes_by_name_;
};

}  // namespace coolang

#endif  // COOLANG_PARSER_AST_H
