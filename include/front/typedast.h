#pragma once
#include "ast.h"
#include "symbolregistry.h"
#include <array>
#include <cstddef>
#include <memory>
#include <variant>

namespace HIR {
struct TypedNode;

enum class ExprOp : uint32_t
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
  UnaryNeg,
  UnaryInc,
  UnaryDec,
  Assign,
  AssignAdd,
  AssignSub,
  AssignMul,
  AssignDiv,
  AssignMod,
  AssignShiftl,
  AssignShiftr,
  AssignAnd,
  AssignOr,
  AssignXor,
};

struct NodeList
{
  std::vector<TypedNode*> nodes;
};

struct FnDef
{
  FunctionID fnID = {};
  TypeID ownerID = {};
  FnNameID nameID = {};
  std::vector<TypeID> paramTypes;
  TypedNode* body;
};

struct FnResolutionKey
{
  TypeID ownerID = {};
  FnNameID nameID = {};
};

struct CallExpr
{
  enum class ResolutionType
  {
    Static,
    Dynamic,
    Error,
  };

  ResolutionType resType = ResolutionType::Dynamic;

  TypedNode* callee = nullptr; // valid always, except for error
  FnResolution fnRes = {};     // valid if static resolving

  // after pass1 every call is unresolved, so callee node contains only other
  // nodes after pass1
  // after pass2, resolving is done where it is possible, and callee node may
  // contain resolved function id

  std::vector<TypedNode*> args;
};

struct ClassDef
{
  TypeID classID = {};
  std::vector<TypedNode*> methods;
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
  Modifier mod = {};
  TypeID type = {};
  VarID id = {};
  TypedNode* val;
};

struct VarAssign
{
  ExprOp op;
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
  ExprOp op;
  TypedNode* lhs;
  TypedNode* rhs;
  TypeID type = {};
};

enum class UnaryExprPrec
{
  Prefix,
  Postfix
};

struct UnaryExpr
{
  ExprOp op;
  UnaryExprPrec prec;
  TypedNode* expr;
  TypeID type = {};
};

struct ArrayAccess
{
  TypedNode* base;
  TypedNode* index;
};

struct MemberAccess
{
  TypedNode* base;
  std::vector<TypedNode*> memb;
  bool resolved = false;
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
  VarResolution varRes;
  FnNameResolution fnNameRes;
};

struct TypedNode
{
  std::variant<
#define X(Name, Cat) Name,
#include "typednodes_cats.def"
#undef X
    std::monostate>
    node;

  size_t line;
  size_t col;
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

struct Analyzed
{};
struct Unanalyzed
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
  ~TypedAST();
  TypedTree<Unanalyzed> build(const ASTNode* root);
  TypedTree<Analyzed> analyze(TypedTree<Unanalyzed> root);
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

}