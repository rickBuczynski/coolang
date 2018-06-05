#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace coolang {
namespace ast {

// TODO maybe instead of strings for type and object names we use the lexer
// tokens for type safety?

class Expr {
  // TODO
};

class Formal {
 public:
  Formal(std::string id, std::string type) : id_(id), type_(type){};

 private:
  std::string id_;
  std::string type_;
};

class MethodFeature {
  // TODO
};

class AttributeFeature {
 public:
  AttributeFeature(Formal formal, std::optional<Expr> initialization_expr)
      : formal_(formal), initialization_expr_(initialization_expr){};
  AttributeFeature(Formal formal)
      : formal_(formal), initialization_expr_(std::nullopt){};

 private:
  Formal formal_;
  std::optional<Expr> initialization_expr_;
};

using Feature = std::variant<MethodFeature, AttributeFeature>;

class CoolClass {
 public:
  CoolClass(std::string type, std::optional<std::string> inherits_type,
            std::vector<Feature> features)
      : type_(type), inherits_type_(inherits_type), features_(features){};
  CoolClass(std::string type, std::vector<Feature> features)
      : type_(type), inherits_type_(std::nullopt), features_(features){};

 private:
  std::string type_;
  std::optional<std::string> inherits_type_;
  std::vector<Feature> features_;
};

class Program {
 public:
  Program(const std::vector<CoolClass>& classes) : classes_(classes){};

 private:
  std::vector<CoolClass> classes_;
};

}  // namespace ast
}  // namespace coolang
