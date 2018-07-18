#include <stack>
#include "coolang/parser/ast.h"
#include "coolang/semantic/type_checker.h"

namespace coolang {

void PopAndEraseIfEmpty(
    std::unordered_map<std::string, std::stack<std::string>>& in_scope_vars,
    const std::string& id) {
  in_scope_vars[id].pop();
  if (in_scope_vars[id].empty()) {
    in_scope_vars.erase(id);
  }
}

class TypeCheckVisitor : public AstVisitor {
 public:
  TypeCheckVisitor(std::string file_name) : file_name_(std::move(file_name)) {}

  const std::vector<SemanticError>& GetErrors() const { return errors_; }

  void Visit(CaseExpr& node) override { node.SetExprType("TODO"); }
  void Visit(StrExpr& node) override { node.SetExprType("String"); }
  void Visit(WhileExpr& node) override { node.SetExprType("TODO"); }
  void Visit(LetExpr& node) override;
  void Visit(IntExpr& node) override { node.SetExprType("Int"); }
  void Visit(IsVoidExpr& node) override { node.SetExprType("TODO"); }
  void Visit(MethodCallExpr& node) override { node.SetExprType("TODO"); }
  void Visit(NotExpr& node) override { node.SetExprType("TODO"); }
  void Visit(IfExpr& node) override { node.SetExprType("TODO"); }
  void Visit(NegExpr& node) override { node.SetExprType("TODO"); }
  void Visit(BlockExpr& node) override {
    for (auto& sub_expr : node.GetExprs()) {
      sub_expr->Accept(*this);
    }
    node.SetExprType(node.GetExprs().back()->GetExprType());
  }
  void Visit(ObjectExpr& node) override;
  void Visit(BinOpExpr& node) override { node.SetExprType("TODO"); }
  void Visit(MultiplyExpr& node) override { node.SetExprType("TODO"); }
  void Visit(LessThanEqualCompareExpr& node) override {
    node.SetExprType("TODO");
  }
  void Visit(SubtractExpr& node) override { node.SetExprType("TODO"); }
  void Visit(AddExpr& node) override { node.SetExprType("TODO"); }
  void Visit(EqCompareExpr& node) override { node.SetExprType("TODO"); }
  void Visit(DivideExpr& node) override { node.SetExprType("TODO"); }
  void Visit(LessThanCompareExpr& node) override { node.SetExprType("TODO"); }
  void Visit(NewExpr& node) override { node.SetExprType(node.GetType()); }
  void Visit(AssignExpr& node) override;
  void Visit(BoolExpr& node) override { node.SetExprType("BoolTODO"); }
  void Visit(ClassAst& node) override;
  void Visit(CaseBranch& node) override {}
  void Visit(Feature& node) override {}
  void Visit(MethodFeature& node) override {}
  void Visit(AttributeFeature& node) override {}
  void Visit(ProgramAst& node) override {}

 private:
  std::vector<SemanticError> errors_;
  std::unordered_map<std::string, std::stack<std::string>> in_scope_vars_;
  std::string file_name_;
};

void TypeCheckVisitor::Visit(ObjectExpr& node) {
  if (in_scope_vars_.find(node.GetId()) == in_scope_vars_.end()) {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Undeclared identifier " + node.GetId() + ".",
                         file_name_);
  }
  node.SetExprType("TODOObjectExpr");
}

void TypeCheckVisitor::Visit(LetExpr& node) {
  if (node.GetInitializationExpr()) {
    node.GetInitializationExpr()->Accept(*this);
  }

  in_scope_vars_[node.GetId()].push(node.GetType());

  if (node.GetInExpr()) {
    node.GetInExpr()->Accept(*this);
  } else if (node.GetChainedLet()) {
    // TODO can you use a variable from earlier in the chain in an init
    // expression within the chain?
    node.GetChainedLet()->Accept(*this);
  }

  PopAndEraseIfEmpty(in_scope_vars_, node.GetId());

  node.SetExprType("TODOLetExpr");
}

void TypeCheckVisitor::Visit(AssignExpr& node) {
  node.GetRhsExpr()->Accept(*this);

  const std::string rhs_type = node.GetRhsExpr()->GetExprType();
  const std::string lhs_type = in_scope_vars_[node.GetId()].top();

  if (rhs_type != lhs_type) {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Type " + rhs_type +
                             " of assigned expression does not conform to "
                             "declared type " +
                             lhs_type + " of identifier " + node.GetId() + ".",
                         file_name_);
  }

  node.SetExprType(rhs_type);
}

void TypeCheckVisitor::Visit(ClassAst& node) {
  // add all attributes to scope before diving into expressions since these
  // attributes are visible throughout the class
  for (const auto* attr : node.GetAttributeFeatures()) {
    if (attr->GetId() == "self") {
      errors_.emplace_back(attr->GetLineRange().end_line_num,
                           "'self' cannot be the name of an attribute.",
                           node.GetContainingFileName());
      return;
    }
    const std::vector<const ClassAst*>& super_classes =
        node.GetAllSuperClasses();
    for (const auto* super_class : super_classes) {
      // TODO this is slow, could be faster
      for (const auto super_attr : super_class->GetAttributeFeatures()) {
        if (super_attr->GetId() == attr->GetId()) {
          errors_.emplace_back(
              attr->GetLineRange().end_line_num,
              "Attribute " + node.GetType() + "." + attr->GetId() +
                  " is already defined in class " + super_class->GetType() +
                  " at " + super_class->GetContainingFileName() + ":" +
                  std::to_string(super_attr->GetLineRange().end_line_num) + ".",
              node.GetContainingFileName());
          return;
        } 
      }
    }

    in_scope_vars_[attr->GetId()].push(attr->GetType());
  }

  for (const auto& feature : node.GetFeatures()) {
    // TODO handle scope for method parameters
    if (feature->GetRootExpr()) {
      feature->GetRootExpr()->Accept(*this);
    }
  }

  // remove all attributes frome scope
  for (const auto& feature : node.GetFeatures()) {
    if (auto* attr = dynamic_cast<AttributeFeature*>(feature.get())) {
      PopAndEraseIfEmpty(in_scope_vars_, attr->GetId());
    }
  }
}

std::vector<SemanticError> TypeChecker::CheckTypes(ProgramAst& program_ast) {
  TypeCheckVisitor type_check_visitor(program_ast.GetFileName());
  for (auto& cool_class : program_ast.MutableClasses()) {
    cool_class.Accept(type_check_visitor);
  }
  return type_check_visitor.GetErrors();
}

}  // namespace coolang