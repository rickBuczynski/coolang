#include <set>
#include <stack>
#include "coolang/parser/ast.h"
#include "coolang/semantic/type_checker.h"

namespace coolang {

class TypeCheckVisitor : public AstVisitor {
 public:
  explicit TypeCheckVisitor(ProgramAst& program_ast)
      : program_ast_(&program_ast) {}

  const std::vector<SemanticError>& GetErrors() const { return errors_; }

  void Visit(CaseExpr& node) override;
  void Visit(StrExpr& node) override { node.SetExprType("String"); }
  void Visit(WhileExpr& node) override;
  void Visit(LetExpr& node) override;
  void Visit(IntExpr& node) override { node.SetExprType("Int"); }
  void Visit(IsVoidExpr& node) override {
    node.MutableChildExpr()->Accept(*this);
    node.SetExprType("Bool");
  }
  void Visit(MethodCallExpr& node) override;
  void Visit(NotExpr& node) override {
    node.MutableChildExpr()->Accept(*this);

    if (node.GetChildExpr()->GetExprType() != "Bool") {
      errors_.emplace_back(
          node.GetLineRange().end_line_num,
          "Operator not needs a Bool arg but got arg of type: " +
              node.GetChildExpr()->GetExprType() + ".",
          program_ast_->GetFileName());
    }

    node.SetExprType("Bool");
  }
  void Visit(IfExpr& node) override;
  void Visit(NegExpr& node) override {
    node.MutableChildExpr()->Accept(*this);

    if (node.GetChildExpr()->GetExprType() != "Int") {
      errors_.emplace_back(node.GetLineRange().end_line_num,
                           "Operator ~ needs an Int arg but got arg of type: " +
                               node.GetChildExpr()->GetExprType() + ".",
                           program_ast_->GetFileName());
    }

    node.SetExprType("Int");
  }
  void Visit(BlockExpr& node) override {
    for (auto& sub_expr : node.GetExprs()) {
      sub_expr->Accept(*this);
    }
    node.SetExprType(node.GetExprs().back()->GetExprType());
  }
  void Visit(ObjectExpr& node) override;
  void Visit(BinOpExpr& node) override;
  void Visit(MultiplyExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Int");
  }
  void Visit(LessThanEqualCompareExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Bool");
  }
  void Visit(SubtractExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Int");
  }
  void Visit(AddExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Int");
  }
  void Visit(EqCompareExpr& node) override;
  void Visit(DivideExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Int");
  }
  void Visit(LessThanCompareExpr& node) override {
    Visit(static_cast<BinOpExpr&>(node));
    node.SetExprType("Bool");
  }
  void Visit(NewExpr& node) override { node.SetExprType(node.GetType()); }
  void Visit(AssignExpr& node) override;
  void Visit(BoolExpr& node) override { node.SetExprType("Bool"); }
  void Visit(ClassAst& node) override;
  void Visit(CaseBranch& node) override {
    AddToScope(node.GetId(), node.GetType());
    node.MutableExpr()->Accept(*this);
    RemoveFromScope(node.GetId());
  }
  void Visit(MethodFeature& node) override {
  }  // not needed, handled by class visitor
  void Visit(AttributeFeature& node) override {
  }  // not needed, handled by class visitor
  void Visit(ProgramAst& node) override;

 private:
  void CheckMethodOverrideHasSameArgs(const ClassAst* class_ast,
                                      const MethodFeature* method);

  void ClearScope() { in_scope_vars_.clear(); }

  void RemoveFromScope(const std::string& id) {
    in_scope_vars_[id].pop();
    if (in_scope_vars_[id].empty()) {
      in_scope_vars_.erase(id);
    }
  }

  void AddToScope(const std::string& id, const std::string& type) {
    in_scope_vars_[id].push(type);
  }

  bool IsSubtype(std::string subtype, std::string supertype) const {
    if (subtype == "_no_type") {
      return true;
    }

    if (subtype == "SELF_TYPE") {
      subtype = current_class_->GetName();
    }
    if (supertype == "SELF_TYPE") {
      supertype = current_class_->GetName();
    }
    for (const ClassAst* c = program_ast_->GetClassByName(subtype);
         c != nullptr; c = c->GetSuperClass()) {
      if (c->GetName() == supertype) {
        return true;
      }
    }
    return false;
  }

  const ClassAst* GetClassByName(std::string name) const {
    if (name == "SELF_TYPE") {
      name = current_class_->GetName();
    }
    return program_ast_->GetClassByName(name);
  }

  std::string FirstCommonSupertype(
      const std::vector<std::string>& types) const {
    std::vector<std::vector<std::string>> all_super_types_for_each_type;
    all_super_types_for_each_type.reserve(types.size());

    for (const auto& type : types) {
      all_super_types_for_each_type.emplace_back();
      all_super_types_for_each_type.back().push_back(type);
      for (const auto* super_class :
           GetClassByName(type)->GetAllSuperClasses()) {
        all_super_types_for_each_type.back().push_back(super_class->GetName());
      }
    }

    std::string prev = all_super_types_for_each_type.front().back();
    while (!all_super_types_for_each_type.front().empty()) {
      std::string cur = all_super_types_for_each_type.front().back();

      for (auto& super_classes : all_super_types_for_each_type) {
        if (super_classes.empty() || super_classes.back() != cur) {
          return prev;
        }
        super_classes.pop_back();
      }

      prev = cur;
    }

    return prev;
  }

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

  if (lhs_type != "Int" || rhs_type != "Int") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "non-Int arguments: " + lhs_type + " + " + rhs_type,
                         program_ast_->GetFileName());
  }
}

void TypeCheckVisitor::Visit(CaseExpr& node) {
  node.MutableCaseExpr()->Accept(*this);

  std::set<std::string> branch_types;
  for (auto& branch : node.MutableBranches()) {
    branch.Accept(*this);
    if (branch_types.find(branch.GetType()) != branch_types.end()) {
      errors_.emplace_back(
          node.GetLineRange().end_line_num,
          "Duplicate branch " + branch.GetType() + " in case statement.",
          program_ast_->GetFileName());
    }

    branch_types.insert(branch.GetType());
  }

  std::vector<std::string> branch_return_types;
  for (const auto& branch : node.GetBranches()) {
    branch_return_types.push_back(branch.GetExpr()->GetExprType());
  }

  node.SetExprType(FirstCommonSupertype(branch_return_types));
}

void TypeCheckVisitor::Visit(WhileExpr& node) {
  node.MutableConditionExpr()->Accept(*this);
  if (node.GetConditionExpr()->GetExprType() != "Bool") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Loop condition does not have type Bool.",
                         program_ast_->GetFileName());
  }

  node.MutableLoopExpr()->Accept(*this);

  node.SetExprType("Object");
}

void TypeCheckVisitor::Visit(LetExpr& node) {
  std::vector<Formal> bindings;
  LetExpr* cur_let = &node;
  Expr* in_expr = nullptr;

  while (in_expr == nullptr) {
    if (cur_let->GetInitializationExpr()) {
      cur_let->GetInitializationExpr()->Accept(*this);

      if (!IsSubtype(cur_let->GetInitializationExpr()->GetExprType(),
                     cur_let->GetType())) {
        errors_.emplace_back(
            cur_let->GetLineRange().end_line_num,
            "Inferred type " + cur_let->GetInitializationExpr()->GetExprType() +
                " of initialization of " + cur_let->GetId() +
                " does not conform to identifier's declared type " +
                cur_let->GetType() + ".",
            program_ast_->GetFileName());
      }
    }

    if (cur_let->GetId() == "self") {
      errors_.emplace_back(cur_let->GetLineRange().end_line_num,
                           "'self' cannot be bound in a 'let' expression.",
                           program_ast_->GetFileName());
    } else {
      bindings.emplace_back(cur_let->GetId(), cur_let->GetType(),
                            LineRange(0, 0));
    }

    in_expr = cur_let->GetInExpr().get();
    cur_let = cur_let->GetChainedLet().get();
  }

  for (const Formal& f : bindings) {
    AddToScope(f.GetId(), f.GetType());
  }

  in_expr->Accept(*this);

  for (const Formal& f : bindings) {
    RemoveFromScope(f.GetId());
  }

  for (LetExpr* let_expr = &node; let_expr != nullptr;
       let_expr = let_expr->GetChainedLet().get()) {
    let_expr->SetExprType(in_expr->GetExprType());
  }
}

void TypeCheckVisitor::Visit(MethodCallExpr& node) {
  node.MutableLhsExpr()->Accept(*this);
  std::string caller_type = node.GetLhsExpr()->GetExprType();

  if (node.GetStaticDispatchType().has_value()) {
    if (!IsSubtype(caller_type, node.GetStaticDispatchType().value())) {
      errors_.emplace_back(
          node.GetLineRange().end_line_num,
          "Expression type " + caller_type +
              " does not conform to declared static dispatch type " +
              node.GetStaticDispatchType().value() + ".",
          program_ast_->GetFileName());
    }
    caller_type = node.GetStaticDispatchType().value();
  }

  if (caller_type == "SELF_TYPE") {
    caller_type = current_class_->GetName();
  }

  const MethodFeature* method_feature =
      program_ast_->GetClassByName(caller_type)
          ->GetMethodFeatureByName(node.GetMethodName());
  if (method_feature == nullptr) {
    errors_.emplace_back(
        node.GetLineRange().end_line_num,
        "Dispatch to undefined method " + node.GetMethodName() + ".",
        program_ast_->GetFileName());
    return;
  }

  auto expected_args = method_feature->GetArgs();
  auto& args = node.MutableArgs();

  if (expected_args.size() != args.size()) {
    errors_.emplace_back(
        node.GetLineRange().end_line_num,
        "In call of method " + node.GetMethodName() + " expected " +
            std::to_string(expected_args.size()) + " args but found " +
            std::to_string(args.size()) + " args.",
        program_ast_->GetFileName());
  }

  for (size_t i = 0; i < args.size(); i++) {
    args[i]->Accept(*this);
    if (!IsSubtype(args[i]->GetExprType(), expected_args[i].GetType())) {
      errors_.emplace_back(node.GetLineRange().end_line_num,
                           "In call of method " + node.GetMethodName() +
                               ", type " + args[i]->GetExprType() +
                               " of parameter " + expected_args[i].GetId() +
                               " does not conform to declared type " +
                               expected_args[i].GetType() + ".",
                           program_ast_->GetFileName());
    }
  }

  std::string return_type = method_feature->GetReturnType();

  bool lhs_is_self = false;
  if (const auto* obj_expr =
          dynamic_cast<const ObjectExpr*>(node.GetLhsExpr())) {
    if (obj_expr->GetId() == "self") {
      lhs_is_self = true;
    }
  }

  if (return_type == "SELF_TYPE" && !lhs_is_self) {
    return_type = caller_type;
  }

  node.SetExprType(return_type);
}

void TypeCheckVisitor::Visit(IfExpr& node) {
  node.MutableIfConditionExpr()->Accept(*this);
  if (node.GetIfConditionExpr()->GetExprType() != "Bool") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Loop condition does not have type Bool.",
                         program_ast_->GetFileName());
  }

  node.MutableThenExpr()->Accept(*this);
  node.MutableElseExpr()->Accept(*this);

  node.SetExprType(FirstCommonSupertype(
      {node.GetThenExpr()->GetExprType(), node.GetElseExpr()->GetExprType()}));
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

  if (node.GetId() == "self") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Cannot assign to 'self'.",
                         program_ast_->GetFileName());
    return;
  }

  std::string rhs_type = node.GetRhsExpr()->GetExprType();
  const std::string lhs_type = in_scope_vars_[node.GetId()].top();

  if (rhs_type == "SELF_TYPE") {
    rhs_type = current_class_->GetName();
  }

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

  if (node.GetName() == "Int" || node.GetName() == "Bool" ||
      node.GetName() == "String" || node.GetName() == "Object" ||
      node.GetName() == "IO" || node.GetName() == "SELF_TYPE") {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Redefinition of basic class " + node.GetName() + ".",
                         node.GetContainingFilePath());
  }

  // add all attributes to scope before diving into expressions since these
  // attributes are visible throughout the class
  std::vector<const ClassAst*> class_and_supers = node.GetAllSuperClasses();
  class_and_supers.push_back(&node);

  std::unordered_map<std::string,
                     std::pair<const AttributeFeature*, const ClassAst*>>
      attributes_by_id;
  for (const auto* cool_class : class_and_supers) {
    for (const auto* attr : cool_class->GetAttributeFeatures()) {
      if (attr->GetId() == "self") {
        errors_.emplace_back(attr->GetLineRange().end_line_num,
                             "'self' cannot be the name of an attribute.",
                             node.GetContainingFilePath());
        return;
      }

      const auto already_defined_in_class =
          attributes_by_id.find(attr->GetId());
      if (already_defined_in_class != attributes_by_id.end()) {
        errors_.emplace_back(
            attr->GetLineRange().end_line_num,
            "Attribute " + node.GetName() + "." +
                already_defined_in_class->first +
                " is already defined in class " +
                already_defined_in_class->second.second->GetName() + " at " +
                already_defined_in_class->second.second->GetContainingFilePath()
                    .filename()
                    .string() +
                ":" +
                std::to_string(
                    already_defined_in_class->second.first->GetLineRange()
                        .end_line_num) +
                ".",
            node.GetContainingFilePath());
        return;
      }

      attributes_by_id[attr->GetId()] = std::make_pair(attr, cool_class);
      AddToScope(attr->GetId(), attr->GetType());
    }
  }

  bool has_main_method = false;
  for (const auto& feature : node.GetFeatures()) {
    if (auto* method_feature = dynamic_cast<MethodFeature*>(feature.get())) {
      auto& args = method_feature->GetArgs();
      std::set<std::string> arg_ids;
      for (const auto& arg : args) {
        if (arg_ids.find(arg.GetId()) != arg_ids.end()) {
          errors_.emplace_back(
              method_feature->GetLineRange().end_line_num,
              "Formal parameter " + arg.GetId() + " is multiply defined.",
              node.GetContainingFilePath());
        }
        arg_ids.insert(arg.GetId());
        in_scope_vars_[arg.GetId()].push(arg.GetType());

        if (arg.GetType() == "SELF_TYPE") {
          errors_.emplace_back(method_feature->GetLineRange().end_line_num,
                               "Formal parameter " + arg.GetId() +
                                   " cannot have type SELF_TYPE.",
                               node.GetContainingFilePath());
        }
      }

      if (arg_ids.find("self") != arg_ids.end()) {
        errors_.emplace_back(method_feature->GetLineRange().end_line_num,
                             "'self' cannot be the name of a formal parameter.",
                             node.GetContainingFilePath());
      }

      CheckMethodOverrideHasSameArgs(&node, method_feature);
    }

    if (feature->GetRootExpr()) {
      feature->GetRootExpr()->Accept(*this);
    }

    if (auto* method_feature = dynamic_cast<MethodFeature*>(feature.get())) {
      auto& args = method_feature->GetArgs();
      for (const auto& arg : args) {
        RemoveFromScope(arg.GetId());
      }

      const std::string& declared_return_type = method_feature->GetReturnType();
      const std::string& inferred_return_type =
          method_feature->GetRootExpr()->GetExprType();

      if (declared_return_type != "SELF_TYPE" &&
          program_ast_->GetClassByName(declared_return_type) == nullptr) {
        errors_.emplace_back(
            method_feature->GetLineRange().end_line_num,
            "Method " + node.GetName() + "." + method_feature->GetId() +
                " has undefined type " + declared_return_type + ".",
            node.GetContainingFilePath());
      } else if (!IsSubtype(inferred_return_type, declared_return_type) ||
                 declared_return_type == "SELF_TYPE" &&
                     inferred_return_type != "SELF_TYPE") {
        errors_.emplace_back(method_feature->GetLineRange().end_line_num,
                             "Inferred return type " + inferred_return_type +
                                 " of method " + method_feature->GetId() +
                                 " does not conform to declared return type " +
                                 declared_return_type + ".",
                             node.GetContainingFilePath());
      }

      if (method_feature->GetId() == "main") {
        has_main_method = true;
        if (node.GetName() == "Main" && !method_feature->GetArgs().empty()) {
          errors_.emplace_back(node.GetLineRange().end_line_num,
                               "Method Main.main shouldn't take arguments.",
                               node.GetContainingFilePath());
        }
      }
    }
  }

  if (node.GetName() == "Main" && !has_main_method) {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Method Main.main is not defined.",
                         node.GetContainingFilePath());
  }

  ClearScope();
}

void TypeCheckVisitor::CheckMethodOverrideHasSameArgs(
    const ClassAst* class_ast, const MethodFeature* method) {
  const ClassAst* super_class = class_ast->GetSuperClass();
  const MethodFeature* overriden_method =
      super_class->GetMethodFeatureByName(method->GetId());
  if (overriden_method != nullptr) {
    if (method->GetArgs().size() != overriden_method->GetArgs().size()) {
      errors_.emplace_back(
          method->GetLineRange().end_line_num,
          "Incompatible number of formal parameters in redefined method " +
              overriden_method->GetId() + ".",
          class_ast->GetContainingFilePath());
      return;
    }

    for (size_t i = 0; i < method->GetArgs().size(); i++) {
      if (overriden_method->GetArgs()[i].GetType() !=
          method->GetArgs()[i].GetType()) {
        errors_.emplace_back(method->GetLineRange().end_line_num,
                             "In redefined method " + method->GetId() +
                                 ", parameter type " +
                                 method->GetArgs()[i].GetType() +
                                 " is different from original type " +
                                 overriden_method->GetArgs()[i].GetType(),
                             class_ast->GetContainingFilePath());
      }
    }
  }
}

void TypeCheckVisitor::Visit(ProgramAst& node) {
  std::set<std::string> class_names;
  for (auto& cool_class : node.MutableClasses()) {
    if (class_names.find(cool_class.GetName()) != class_names.end()) {
      errors_.emplace_back(
          cool_class.GetLineRange().end_line_num,
          "Class " + cool_class.GetName() + " was previously defined.",
          node.GetFileName());
    }
    class_names.insert(cool_class.GetName());
    cool_class.Accept(*this);
  }

  if (node.GetClassByName("Main") == nullptr) {
    errors_.emplace_back(node.GetLineRange().end_line_num,
                         "Class Main is not defined.", node.GetFileName());
  }
}

std::vector<SemanticError> TypeChecker::CheckTypes(ProgramAst& program_ast) {
  TypeCheckVisitor type_check_visitor(program_ast);
  program_ast.Accept(type_check_visitor);
  return type_check_visitor.GetErrors();
}

}  // namespace coolang