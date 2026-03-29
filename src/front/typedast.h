#pragma once
#include "shared.h"
#include "symbolregistry.h"
#include <array>
#include <memory>
#include <variant>

struct TypedNode;

enum class BinaryExprOp
{
  Add = 0,
  Sub,
  Mul,
  Div,
  Mod,
  Shiftl,
  Shiftr,
  And,
  Or,
  Xor,
  Lt,
  Gt,
  Leq,
  Geq,
  Eq,
  Neq,
  LogicAnd,
  LogicOr,
  _Count,
};

enum class UnaryExprOp
{
  Neg = to_underlying(BinaryExprOp::_Count),
  Inc,
  Dec,
  _Count,
};

enum class AssignOp
{
  None = to_underlying(UnaryExprOp::_Count),
  Add,
  Sub,
  Mul,
  Div,
  Mod,
  Shiftl,
  Shiftr,
  And,
  Or,
  Xor,
};

struct NodeList
{
  std::vector<TypedNode*> nodes;
};

struct FnDef
{
  FunctionID fnID = fnInvalidID;
  TypedNode* body;
};

struct FnResolutionKey
{
  TypeID ownerID;
  FnNameID nameID;
};

struct CallExpr
{
  enum class ResolutionType
  {
    Unresolved,
    Static,
    Dynamic,
    Error,
  };

  ResolutionType resType = ResolutionType::Unresolved;

  const TypedNode* callee = nullptr; // valid always, except for error
  FnResolutionKey fnKey = { classInvalidID,
                            fnNameInvalidID }; // valid if static resolving

  // after pass1 every call is unresolved, so this node contains only other
  // nodes after pass1
  // after pass2, resolving is done where it is possible, and node may contain
  // resolved function id

  std::vector<TypedNode*> args;
};

struct ClassDef
{
  TypeID classID = classInvalidID;
  std::vector<FnDef> methods;
  std::vector<const char*> fields;
};

struct EnumElement
{
  const char* name;
  TypedNode* val;
};

struct EnumDef
{
  std::vector<EnumElement> vals;
};

struct VarDecl
{
  const char* name;
  Modifier mod;
  TypedNode* val;
};

struct VarAssign
{
  AssignOp op;
  TypedNode* lhs;
  TypedNode* rhs;
};

struct LoopWhl
{
  TypedNode* cond;
  TypedNode* body;
};

struct LoopFor
{
  TypedNode* assigns;
  TypedNode* cond;
  TypedNode* postIter;
  TypedNode* body;
};

struct StmtIf
{
  TypedNode* cond;
  TypedNode* body;
  TypedNode* elseBody;
};

struct StmtSwitch
{
  struct StmtSwitchCase
  {
    TypedNode* expr;
    TypedNode* body;
  };

  TypedNode* expr;
  std::vector<StmtSwitchCase> cases;
};

struct StmtRet
{
  TypedNode* retValue;
};

struct StmtBrk
{
  TypedNode* cond;
};

struct BinaryExpr
{
  BinaryExprOp op;
  TypedNode* lhs;
  TypedNode* rhs;
};

enum class UnaryExprPrec
{
  Prefix,
  Postfix
};

struct UnaryExpr
{
  UnaryExprOp op;
  UnaryExprPrec prec;
  TypedNode* expr;
};

struct MemberAccess
{
  TypedNode* base;
  std::variant<TypedNode*, // unresolved
               VarID,      // field
               FunctionID  // method
               >
    membID;
};

struct ArrayAccess
{
  TypedNode* base;
  TypedNode* index;
};

struct BoolVal
{
  bool val;
};

struct NumVal
{
  double val;
};

struct StringVal
{
  const char* val;
};

struct ArrayVal
{
  std::vector<TypedNode*> list;
};

struct NameExpr
{
  const char* name;
  VarID id = varInvalidID;
};

struct TypedNode
{
  std::variant<std::monostate
#define X(Name, Cat) , Name
#include "typednodes_cats.def"
#undef X
               >
    node;
};

template<typename T>
struct isDecl : std::false_type
{};
template<typename T>
struct isExpr : std::false_type
{};
template<typename T>
struct isStmt : std::false_type
{};

enum class NodeCat
{
  None,
  Decl,
  Expr,
  Stmt,
};

template<typename T>
struct NodeCategory
{
  static constexpr NodeCat val = NodeCat::None;
};

#define X(Name, Cat)                                                           \
  template<>                                                                   \
  struct NodeCategory<Name>                                                    \
  {                                                                            \
    static constexpr NodeCat val = NodeCat::Cat;                               \
  };
#include "typednodes_cats.def"
#undef X

template<typename T>
constexpr NodeCat nodeCatVal = NodeCategory<T>::val;

inline NodeCat
catOf(const TypedNode& n)
{
  return std::visit([](const auto& x) { return nodeCatVal<decltype(x)>; },
                    n.node);
}

class Diagnostics;
struct ASTNode;
struct ASTNodeLL;

struct AnalyzedTag
{};
struct UnanalyzedTag
{};

template<typename Tag>
struct TypedTree
{
  const TypedNode* root;
};

class TypedASTBuilder;
class TypedASTAnalyzer;

class TypedAST
{
public:
  TypedAST(Diagnostics& diag);
  TypedTree<UnanalyzedTag> build(const ASTNode* root);
  TypedTree<AnalyzedTag> analyze(TypedTree<UnanalyzedTag> root);
  void print(const TypedNode* node, int indent = 0);

private:
  TypedAST(const TypedAST& other) = delete;
  TypedAST(TypedAST&& other) = delete;

  Diagnostics& _diag;
  std::unique_ptr<SymbolRegistry> _reg;
  std::unique_ptr<TypedASTBuilder> _builder;
  std::unique_ptr<TypedASTAnalyzer> _analyzer;

  static constexpr std::array _typedNodesStr = {
#define X(Name, Cat) #Name,
#include "typednodes_cats.def"
#undef X
  };

  inline void printIndent(int indent)
  {
    for (int i = 0; i < indent; ++i)
      putchar(' ');
  }
};