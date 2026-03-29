#pragma once
#include "typedast.h"

class SymbolRegistry;
class Diagnostics;

class TypedASTAnalyzer
{
public:
  TypedASTAnalyzer(SymbolRegistry& reg, Diagnostics& diag);
  inline TypedTree<AnalyzedTag> analyze(TypedTree<UnanalyzedTag> root)
  {
    analyzeTypedNode(root.root);
    return { root.root };
  }

private:
  TypedASTAnalyzer(const TypedASTAnalyzer& other) = delete;
  TypedASTAnalyzer(TypedASTAnalyzer&& other) = delete;

  // pass 2 - semantic checks, resolution
  void analyzeTypedNode(const TypedNode* node) {}

  // pass2 methods
  void analyzeNodeList(const TypedNode* list);
  void analyzeFnDef(const TypedNode* fnDef);
  void analyzeCallExpr(const TypedNode* callExpr);
  void analyzeClassDef(const TypedNode* classDef);
  void analyzeEnumDef(const TypedNode* enumDef);
  void analyzeVarDecl(const TypedNode* varDecl);
  void analyzeVarAssign(const TypedNode* varAssign);
  void analyzeWhl(const TypedNode* loopWhl);
  void analyzeFor(const TypedNode* loopFor);
  void analyzeIf(const TypedNode* stmtIf);
  void analyzeSwitch(const TypedNode* stmtSwitch);
  void analyzeRet(const TypedNode* stmtRet);
  void analyzeBrk(const TypedNode* stmtBrk);
  void analyzeBinaryExpr(const TypedNode* binaryExpr);
  void analyzeUnaryExpr(const TypedNode* unaryExpr);
  void analyzeMemberAccess(const TypedNode* membAccess);
  void analyzeArrayAccess(const TypedNode* arrAccess);
  void analyzeBool(const TypedNode* boolVal);
  void analyzeNumber(const TypedNode* numVal);
  void analyzeString(const TypedNode* stringVal);
  void analyzeArray(const TypedNode* arrVal);
  void analyzeName(const TypedNode* nameExpr);
};