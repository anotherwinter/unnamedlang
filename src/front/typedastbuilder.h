#pragma once
#include "../alloc/arena.h"
#include "typedast.h"

struct ASTNode;

class SymbolRegistry;
class Diagnostics;

namespace HIR {

// pass 0/1 - Typed AST builder
class TypedASTBuilder
{
public:
  TypedASTBuilder(Diagnostics& diag, SymbolRegistry& reg);

  // pass0/1 - build typed ast
  inline TypedTree<Unanalyzed> build(const ASTNode* root)
  {
    _arena.reset();
    predeclare(root);
    TypedNode* typedRoot = buildFromAST(root);

    return { typedRoot };
  }

private:
  TypedASTBuilder(const TypedASTBuilder& other) = delete;
  TypedASTBuilder(TypedASTBuilder&& other) = delete;

  Diagnostics& _diag;
  SymbolRegistry& _reg;
  ArenaAlloc _arena;

  template<typename T>
  TypedNode* allocTypedNode(const ASTNode* node);

  inline TypedNode* allocTypedNode()
  {
    return allocTypedNode<std::monostate>(nullptr);
  }

  // pass0 - predeclare all class occurences
  void predeclare(const ASTNode* node);

  // pass1 - build typed ast tree (might contain erroneous nodes) without
  // semantics check
  TypedNode* buildFromAST(const ASTNode* node);

  // pass1 methods
  TypedNode* buildNodeList(const ASTNode* node);
  TypedNode* buildFnDef(const ASTNode* node);
  TypedNode* buildCallExpr(const ASTNode* node);
  TypedNode* buildClassDef(const ASTNode* node);
  TypedNode* buildEnumDef(const ASTNode* node);
  TypedNode* buildVarDecl(const ASTNode* node);
  TypedNode* buildVarAssign(const ASTNode* node);
  TypedNode* buildWhl(const ASTNode* node);
  TypedNode* buildFor(const ASTNode* node);
  TypedNode* buildIf(const ASTNode* node);
  TypedNode* buildSwitch(const ASTNode* node);
  TypedNode* buildRet(const ASTNode* node);
  TypedNode* buildBrk(const ASTNode* node);
  TypedNode* buildBinaryExpr(const ASTNode* node);
  TypedNode* buildUnaryExpr(const ASTNode* node, UnaryExprPrec prec);
  TypedNode* buildMemberAccess(const ASTNode* node);
  TypedNode* buildArrayAccess(const ASTNode* node);
  TypedNode* buildBool(const ASTNode* node);
  TypedNode* buildNumber(const ASTNode* node);
  TypedNode* buildString(const ASTNode* node);
  TypedNode* buildArray(const ASTNode* node);
  TypedNode* buildName(const ASTNode* node);

  [[nodiscard]] FnDeclKey evalFnDecl(const ASTNode* node,
                                     std::vector<std::string>& paramNames,
                                     std::vector<TypeID>& paramTypes,
                                     TypeID ownerID = dynamicTypeID);
};

}