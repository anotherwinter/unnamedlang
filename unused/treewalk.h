#pragma once
#include "runtime.h"

struct ASTNode;

class Treewalk
{
public:
  Treewalk(Runtime& runtime);
  Identifier eval(const ASTNode* n);

private:
  Runtime& _runtime;

  Identifier evalNode(const ASTNode* node);
  Identifier evalNodeList(const ASTNode* nodeList);
  Identifier evalEnumDef(const ASTNode* enumDef);
  Identifier evalEnumElement(const ASTNode* enumElement, int prev);
  Identifier evalFnDef(const ASTNode* fnDef);
  Identifier evalFnCall(const ASTNode* fnCall);
  Identifier evalClassDef(const ASTNode* classDef);
  Identifier evalVarDecl(const ASTNode* batchDecl);
  Identifier evalVarAssign(const ASTNode* varAssign);
  Identifier evalWhl(const ASTNode* loopWhl);
  Identifier evalFor(const ASTNode* loopFor);
  Identifier evalIf(const ASTNode* ifSWtmt);
  Identifier evalSwitch(const ASTNode* switchStmt);
  Identifier evalRet(const ASTNode* retStmt);
  Identifier evalBrk(const ASTNode* brkStmt);
  Identifier evalToss(const ASTNode* tossStmt);

  Identifier evalName(const char* name);
  Identifier evalArray(const ASTNode* array);
  Identifier evalMemberAccess(const ASTNode* membAccess);
  Identifier evalArrayAccess(const ASTNode* arrayAccess);
  Identifier evalBinaryOp(const ASTNode* node);
  Identifier evalUnaryOp(const ASTNode* node);
};