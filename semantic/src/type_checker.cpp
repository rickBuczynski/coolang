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
  TypeCheckVisitor(ProgramAst& program_ast) : program_ast_(&program_ast) {}

  const std::vector<SemanticError>& GetErrors() const { return errors_; }

  void Visit(CaseExpr& node) override { node.SetExprType("TODO"); }
  void Visit(StrExpr& node) override { node.SetExprType("String"); }
  void Visit(WhileExpr& node) override { node.SetExprType("TODO"); }
  void Visit(LetExpr& node) override;
  void Visit(IntExpr& node) override { node.SetExprType("Int"); }
  void Visit(IsVoidExpr& node) override { node.SetExprType("TODO"); }
  void Visit(MethodCallExpr& node) override;
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
  void Visit(BinOpExpr& node) override;
  void Visit(MultiplyExpr& node) override { node.SetExprType("TODO"); }
  void Visit(LessThanEqualCompareExpr& node) override {
    node.SetExprType("TODO");
  }
  void Visit(SubtractExpr& node) override { node.SetExprType("TODO"); }
  void Visit(AddExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Int");
  }
  void Visit(EqCompareExpr& node) override;
  void Visit(DivideExpr& node) override { node.SetExprType("TODO"); }
  void Visit(LessThanCompareExpr& node) override { node.SetExprType("TODO"); }
  void Visit(NewExpr& node) override { node.SetExprType(node.GetType()); }
  void Visit(AssignExpr& node) override;
  void Visit(BoolExpr& node) override { node.SetExprType("Bool"); }
  void Visit(ClassAst& node) override;
  void Visit(CaseBranch& node) override {}
  void Visit(MethodFeature& node) override {
  }  // not needed, handled by class visitor
  void Visit(AttributeFeature& node) override {
  }  // not needed, handled by class visitor
  void Visit(ProgramAst& node) override;

 private:
  std::vector<SemanticError> errors_;
  std::unordered_map<std::string, std::stack<std::string>> in_scope_vars_;
  ClassAst* current_class_ = nullptr;
  ProgramAst* program_ast_;
};

void TypeCheckVisitor::Visit(ObjectExpr& node) {
  if (node.GetId() == "self") {
    node.SetExprType("SELF_TYPE");
    return;
  }

  const auto vars_find_result = in_scope_vars_.find(node.GetId());
  if (vars_find_result == in_scope_vars_.end()) {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Undeclared identifier " + node.GetId() + ".",
                         program_ast_->GetFileName());
  } else {
    node.SetExprType(vars_find_result->second.top());
  }
}

void TypeCheckVisitor::Visit(BinOpExpr& node) {
  node.MutableLhsExpr()->Accept(*this);
  node.MutableRhsExpr()->Accept(*this);

  const std::string lhs_type = node.GetLhsExpr()->GetExprType();
  const std::string rhs_type = node.GetRhsExpr()->GetExprType();

  // TODO do all BinOps require ints?
  // Even EqCompareExpr?
  if (lhs_type != "Int" || rhs_type != "Int") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "non-Int arguments: " + lhs_type + " + " + rhs_type,
                         program_ast_->GetFileName());
  }
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

void TypeCheckVisitor::Visit(MethodCallExpr& node) {
  std::string caller_type;

  if (node.GetLhsExpr()) {
    node.MutableLhsExpr()->Accept(*this);
    caller_type = node.GetLhsExpr()->GetExprType();

    if (node.GetStaticDispatchType().has_value()) {
      // TODO handle inheritance
      if (caller_type != node.GetStaticDispatchType().value()) {
        errors_.emplace_back(
            node.GetLineRange().end_line_num,
            "Expression type " + caller_type +
                " does not conform to declared static dispatch type " +
                node.GetStaticDispatchType().value() + ".",
            program_ast_->GetFileName());
      }
      caller_type = node.GetStaticDispatchType().value();
    }
  } else {
    caller_type = current_class_->GetType();
  }

  const MethodFeature* method_feature =
      program_ast_->GetClassByName(caller_type)
          ->GetMethodFeatureByName(node.GetMethodName());
  // TODO method_feature could be null if that method is not defined

  auto expected_args = method_feature->GetArgs();
  auto& args = node.MutableArgs();
  // TODO check incorrect number of method call args

  for (auto i = 0; i < args.size(); i++) {
    args[i]->Accept(*this);
    // TODO need to account for inheritance
    if (args[i]->GetExprType() != expected_args[i].GetType()) {
      errors_.emplace_back(node.GetLineRange().end_line_num,
                           "In call of method " + node.GetMethodName() +
                               ", type " + args[i]->GetExprType() +
                               " of parameter " + expected_args[i].GetId() +
                               " does not conform to declared type " +
                               expected_args[i].GetType() + ".",
                           program_ast_->GetFileName());
    }
  }

  node.SetExprType(method_feature->GetReturnType());
}

void TypeCheckVisitor::Visit(EqCompareExpr& node) {
  node.MutableLhsExpr()->Accept(*this);
  node.MutableRhsExpr()->Accept(*this);

  const std::string lhs_type = node.GetLhsExpr()->GetExprType();
  const std::string rhs_type = node.GetRhsExpr()->GetExprType();

  if (lhs_type == "Int" || lhs_type == "Bool" || lhs_type == "String" ||
      rhs_type == "Int" || rhs_type == "Bool" || rhs_type == "String") {
    if (lhs_type != rhs_type) {
      errors_.emplace_back(node.GetLineRange().end_line_num,
                           "Illegal comparison with a basic type.",
                           program_ast_->GetFileName());
    }
  }

  node.SetExprType("Bool");
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
                         program_ast_->GetFileName());
  }

  node.SetExprType(rhs_type);
}

void TypeCheckVisitor::Visit(ClassAst& node) {
  current_class_ = &node;

  if (node.GetType() == "Int" || node.GetType() == "Bool" ||
      node.GetType() == "String") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Redefinition of basic class " + node.GetType() + ".",
                         node.GetContainingFileName());
  }

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
    if (auto* method_feature = dynamic_cast<MethodFeature*>(feature.get())) {
      auto& args = method_feature->GetArgs();
      for (const auto& arg : args) {
        in_scope_vars_[arg.GetId()].push(arg.GetType());
      }
    }

    if (feature->GetRootExpr()) {
      feature->GetRootExpr()->Accept(*this);
    }

    if (auto* method_feature = dynamic_cast<MethodFeature*>(feature.get())) {
      auto& args = method_feature->GetArgs();
      for (const auto& arg : args) {
        PopAndEraseIfEmpty(in_scope_vars_, arg.GetId());
      }
    }
  }

  // remove all attributes frome scope
  for (const auto& feature : node.GetFeatures()) {
    if (auto* attr = dynamic_cast<AttributeFeature*>(feature.get())) {
      PopAndEraseIfEmpty(in_scope_vars_, attr->GetId());
    }
  }
}

void TypeCheckVisitor::Visit(ProgramAst& node) {
  for (auto& cool_class : node.MutableClasses()) {
    cool_class.Accept(*this);
  }
}

std::vector<SemanticError> TypeChecker::CheckTypes(ProgramAst& program_ast) {
  TypeCheckVisitor type_check_visitor(program_ast);
  program_ast.Accept(type_check_visitor);
  return type_check_visitor.GetErrors();
}

}  // namespace coolang