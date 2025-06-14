#include "eval_ast.h"
#include "ast.h"
#include "runtime.h"
#include "types.h"
#include <csetjmp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// fwd decls
void
evalNodeList(ASTNode* nodeList);
void
evalEnumDef(ASTNode* enumDef);
void
evalEnumElement(ASTNode* enumElement, int prev);
void
evalFnDef(ASTNode* fnDef);
void
evalFnCall(ASTNode* fnCall);
void
evalClassDef(ASTNode* classDef);
void
evalVarDecl(ASTNode* batchDecl);
void
evalVarAssign(ASTNode* varAssign);
void
evalDel(ASTNode* evalDel);
void
evalWhl(ASTNode* loopWhl);
void
evalFor(ASTNode* loopFor);
void
evalIf(ASTNode* ifStmt);
void
evalSwitch(ASTNode* switchStmt);
void
evalRet(ASTNode* retStmt);
void
evalBrk(ASTNode* brkStmt);
void
evalToss(ASTNode* tossStmt);
Object*
evalName(const char* name);
Object*
evalArray(ASTNode* array);
Object*
evalMemberAccess(ASTNode* membAccess);
Object*
evalArrayAccess(ASTNode* array);
Object*
evalBinaryOp(ASTNode* node);
Object*
evalUnaryOp(ASTNode* node);
Object*
evalLambda(ASTNode* node);

static bool
compareObjects(Object* A, Object* B)
{
  if (!A || !B)
    return false;

  return A == B;
}

void
evalProgram(ASTNode* stmts)
{
  if (!stmts)
    return;

  ASTNodeLL* list = stmts->data.nodeList.list;
  while (list) {
    if (setGlobalCheckpoint() == 0) {
      contextPush({}, NONE, nullptr);
      evalNode(list->node);
      // if not in global, then pop
      if (!(contextGetFlags() & IN_GLOBAL)) {
        Object* lastEval = contextGetLastEval();
        retainObject(lastEval);
        contextPop();
        contextSetLastEval(lastEval);
      }
    }

    list = list->next;
  }
  freeNode(stmts);
}

void
evalNode(ASTNode* node)
{
  if (!node)
    return;
  switch (node->type) {
    case NODE_LIST:
      evalNodeList(node);
      break;
    case NODE_FN_DEF:
      evalFnDef(node);
      break;
    case NODE_FN_CALL:
      evalFnCall(node);
      break;
    case NODE_CLASS_DEF:
      evalClassDef(node);
      break;
    case NODE_ENUM_DEF:
      evalEnumDef(node);
      break;
    case NODE_VAR_DECL:
      evalVarDecl(node);
      break;
    case NODE_VAR_ASSIGN:
      evalVarAssign(node);
      break;
    case NODE_DEL:
      evalDel(node);
      break;
    case NODE_WHL:
      evalWhl(node);
      break;
    case NODE_FOR:
      evalFor(node);
      break;
    case NODE_IF:
      evalIf(node);
      break;
    case NODE_SWITCH:
      evalSwitch(node);
      break;
    case NODE_RET:
      evalRet(node);
      break;
    case NODE_BRK:
      evalBrk(node);
      break;
    case NODE_TOSS:
      evalToss(node);
      break;
    case NODE_BINARYOP:
      contextSetLastEval(evalBinaryOp(node));
      break;
    case NODE_UNARYOP:
      contextSetLastEval(evalUnaryOp(node));
      break;
    case NODE_MEMBER_ACCESS:
      contextSetLastEval(evalMemberAccess(node));
      break;
    case NODE_ARR_ACCESS:
      contextSetLastEval(evalArrayAccess(node));
      break;
    case NODE_BOOL:
      contextSetLastEval(boolObj(node->data.boolValue));
      break;
    case NODE_NUMBER:
      contextSetLastEval(numberObj(node->data.numberValue));
      break;
    case NODE_STRING:
      contextSetLastEval(stringObj(node->data.stringValue, true));
      break;
    case NODE_ARRAY:
      contextSetLastEval(evalArray(node));
      break;
    case NODE_NAME:
      contextSetLastEval(evalName(node->data.stringValue));
      break;
    case NODE_LAMBDA:
      contextSetLastEval(evalLambda(node));
      break;
    default:
      setError(false, "evalNode: Unknown node type %d\n", (int)node->type);
  }
}

void
evalNodeList(ASTNode* nodeList)
{
  ASTNodeLL* it = nodeList->data.nodeList.list;
  while (it) {
    evalNode(it->node);
    it = it->next;
  }
}

void
evalEnumDef(ASTNode* enumDef)
{
  int prev = 0;
  ASTNodeLL* it = enumDef->data.enumDef.elements;
  while (it) {
    ASTNode* elem = it->node;
    ASTNode* rhs = elem->data.enumElement.expr;
    Object* val;
    if (rhs) {
      evalNode(rhs);
      val = contextGetLastEval();
      if (strcmp(val->type, "Number") == 0)
        setError(false, "evalEnumDef: Enumerator value must be Number type\n");
    } else {
      val = numberObj((double)prev);
    }

    // register enum value in global context
    registerObject(elem->data.enumElement.name, val);
    prev++;
    it = it->next;
  }
}

void
evalFnDef(ASTNode* fnDef)
{
  const char* fnName = fnDef->data.fnDef.name;
  std::vector<ParameterInfo> params;
  params.clear();

  // fill in parameters
  ASTNodeLL* paramsIt = fnDef->data.fnDef.params;
  while (paramsIt) {
    const char* paramName = paramsIt->node->data.paramInfo.name;
    const char* paramType =
      getInternedString(paramsIt->node->data.paramInfo.type->data.stringValue);

    params.push_back({ duplicateString(paramName, true), paramType });
    paramsIt = paramsIt->next;
  }

  Object* fnObj = functionObj(fnDef->data.fnDef.code, params);
  registerObject(duplicateString(fnName, true), fnObj);
}

void
evalFnCall(ASTNode* fnCall)
{
  ASTNode* callee = fnCall->data.fnCall.callee;
  std::vector<Object*> args;
  ASTNodeLL* it = fnCall->data.fnCall.arguments;
  while (it) {
    evalNode(it->node);
    args.push_back(contextGetLastEval());
    it = it->next;
  }

  // if node is just a name, then try if its a class ctor
  if (callee->type == NODE_NAME) {
    const char* className = callee->data.stringValue;
    ClassInfo* info = contextLookupClass(className);
    if (info) {
      Object* instance = classInstance(className, args);
      if (instance) {
        contextSetLastEval(instance);
        return;
      }
    }
  }

  // if its a member access chain, treat it like class member call
  if (callee->type == NODE_MEMBER_ACCESS) {
    evalNode(callee->data.memberAccess.left);
    Object* classInstance = contextGetLastEval();

    const char* methodName = callee->data.memberAccess.right->data.stringValue;
    callClassMember(classInstance->type, methodName, classInstance, args);
  }

  else {
    // find callee object cause its lambda or global function
    evalNode(callee);
    Object* calleeObj = contextGetLastEval();

    if (!calleeObj) {
      setError(false, "evalFnCall: Failed\n");
      return;
    }

    bool isFunction = strcmp(calleeObj->type, "Function") == 0;
    bool isLambda = strcmp(calleeObj->type, "Lambda") == 0;
    if (!isFunction && !isLambda) {
      setError(false, "evalFnCall: Callee is not a callable\n");
      return;
    }

    if (isFunction) {
      Function* fn = (Function*)calleeObj->data.objectPtr;
      callFunction(nullptr, fn, args, NONE, nullptr);
    } else {
      Lambda* lambda = (Lambda*)calleeObj->data.objectPtr;
      callLambda(lambda, args);
    }
  }
}

void
evalClassDef(ASTNode* classDef)
{
  const char* className = classDef->data.classDef.name;
  std::vector<ClassMember> members;

  ASTNodeLL* it = classDef->data.classDef.members;
  while (it) {
    ASTNode* node = it->node;
    if (node->type == NODE_CLASSFIELD_DEF) {
      Modifier mod = node->data.classMethodDef.modifier;
      members.push_back({ node->data.classFieldDef.name, nullptr, mod });
    } else if (node->type == NODE_CLASSMETHOD_DEF) {
      Modifier mod = node->data.classMethodDef.modifier;
      ASTNode* fnDef = node->data.classMethodDef.function;
      const char* methodName = duplicateString(fnDef->data.fnDef.name, true);
      std::vector<ParameterInfo> params;
      params.clear();

      ASTNodeLL* paramsIt = fnDef->data.fnDef.params;
      while (paramsIt) {
        const char* paramName = paramsIt->node->data.paramInfo.name;
        const char* paramType = getInternedString(
          paramsIt->node->data.paramInfo.type->data.stringValue);

        params.push_back({ paramName, paramType });
        paramsIt = paramsIt->next;
      }

      Object* fnObj = functionObj(fnDef->data.fnDef.code, params);
      members.push_back({ node->data.classFieldDef.name, fnObj, mod });
    }

    it = it->next;
  }

  registerClass(className, false, members);
}

void
evalVarDecl(ASTNode* varDecl)
{
  const char* name = varDecl->data.varDecl.name;
  if (varDecl->data.varDecl.expr)
    evalNode(varDecl->data.varDecl.expr);
  else
    contextSetLastEval(nullObj());

  Object* value = contextGetLastEval();

  registerObject(name, value);
  contextSetLastEval(value);
}

void
evalVarAssign(ASTNode* varAssign)
{
  Object** location =
    resolveLHS(varAssign->data.varAssign.lhs, false);
  evalNode(varAssign->data.varAssign.rhs);
  Object* rhs = contextGetLastEval();
  if (location && (*location)->isConst && (*location)->refcount) {
    setError(false, "evalVarAssign: Trying to assign to constant object\n");
    runtimeRecover();
  }

  Object* lhs = *location;

  switch (varAssign->data.varAssign.op) {
    case OperatorType::EQUALS: {
      break;
    }
    case OperatorType::ASSIGN_ADD: {
      callBinaryOp(OperatorType::BINARY_ADD, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_SUB: {
      callBinaryOp(OperatorType::BINARY_SUB, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_DIV: {
      callBinaryOp(OperatorType::BINARY_DIV, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_MUL: {
      callBinaryOp(OperatorType::BINARY_MUL, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_MOD: {
      callBinaryOp(OperatorType::BINARY_MOD, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_XOR: {
      callBinaryOp(OperatorType::BINARY_XOR, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_SHIFTL: {
      callBinaryOp(OperatorType::BINARY_SHIFTL, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_SHIFTR: {
      callBinaryOp(OperatorType::BINARY_SHIFTR, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_AND: {
      callBinaryOp(OperatorType::BINARY_AND, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    case OperatorType::ASSIGN_OR: {
      callBinaryOp(OperatorType::BINARY_OR, lhs, rhs);
      rhs = contextGetLastEval();
      break;
    }
    default: {
      setError(false, "Wrong assignment operator\n");
    }
  }

  releaseObject(lhs);
  *location = rhs;
  retainObject(rhs);
}

void
evalDel(ASTNode* evalDel)
{
  evalNode(evalDel->data.del.expr);
  Object* obj = contextGetLastEval();

  // deleting global null object is not allowed
  if (obj == nullObj()) {
    setError(false, "releaseObject: Trying to release null object\n");
    runtimeRecover();
  }

  if (obj) {
    releaseObject(obj);
  } else {
    setError(false, "evalDel: Can't delete nullptr\n");
    runtimeRecover();
  }
}

void
evalWhl(ASTNode* loopWhl)
{
  ASTNode* cond = loopWhl->data.loopWhl.condition;
  ASTNode* code = loopWhl->data.loopWhl.code;

  // enable breakable flag
  contextSetFlag(IN_BREAKABLE);
  while (true) {
    // evaluate condition
    evalNode(cond);
    Object* val = contextGetLastEval();

    // if condition is not Number or Bool type, then report error
    if (strcmp(val->type, "Bool") && strcmp(val->type, "Number")) {
      setError(false, "evalWhl: Condition cannot be evaluated to Bool type\n");
      break;
    }

    if (!(((Bool*)val->data.objectPtr)->value))
      break;

    evalNode(code);

    if (contextGetFlags() & BROKEN) {
      contextResetFlag(BROKEN);
      break;
    }
  }

  contextResetFlag(IN_BREAKABLE);
}

void
evalFor(ASTNode* loopFor)
{
  // enable breakable flag
  contextSetFlag(IN_BREAKABLE);

  // evaluate pre-iterations assignments
  ASTNodeLL* it = loopFor->data.loopFor.assigns;
  while (it) {
    evalNode(it->node);
    it = it->next;
  }

  ASTNodeLL* cond = loopFor->data.loopFor.condition;
  ASTNode* code = loopFor->data.loopFor.code;
  ASTNodeLL* post = loopFor->data.loopFor.postIterationAssigns;
  bool runningLoop = true;

  while (true) {
    auto it = cond;
    // evaluate every condition and break loop if got false at any
    while (it) {
      evalNode(it->node);
      Object* val = contextGetLastEval();

      // if condition is not Number or Bool type, then report error
      if (strcmp(val->type, "Bool") && strcmp(val->type, "Number")) {
        setError(false,
                 "evalFor: Condition cannot be evaluated to Bool type\n");
        break;
      }

      if (!((Bool*)val->data.objectPtr)->value) {
        runningLoop = false;
        break;
      }
    }

    if (!runningLoop)
      break;

    evalNode(code);
    if (contextGetFlags() & BROKEN) {
      contextResetFlag(BROKEN);
      break;
    }

    // evaluate post-iteration assignments
    ASTNodeLL* postIt = post;
    while (postIt) {
      evalNode(postIt->node);
      postIt = postIt->next;
    }
  }

  contextResetFlag(IN_BREAKABLE);
}

void
evalIf(ASTNode* ifStmt)
{
  ASTNode* cond = ifStmt->data.ifBlock.condition;
  ASTNode* thenBlock = ifStmt->data.ifBlock.code;
  ASTNode* elseBlock = ifStmt->data.ifBlock.elseBlock;
  evalNode(cond);
  Object* val = contextGetLastEval();

  // if condition is not Number or Bool type, then report error
  if (strcmp(val->type, "Bool") && strcmp(val->type, "Number")) {
    setError(false, "evalFor: Condition cannot be evaluated to Bool type\n");
    return;
  }

  if (((Bool*)val->data.objectPtr)->value) {
    evalNode(thenBlock);
  } else if (elseBlock) {
    evalNode(elseBlock);
  }
}

void
evalSwitch(ASTNode* switchStmt)
{
  ASTNode* expr = switchStmt->data.switchStmt.expr;
  ASTNodeLL* cases = switchStmt->data.switchStmt.cases;
  evalNode(expr);
  Object* scrut = contextGetLastEval();
  while (cases) {
    ASTNode* caseNode = cases->node;
    if (caseNode->data.switchCase.isDefault) {
      evalNode(caseNode->data.switchCase.code);
      return;
    }

    ASTNode* caseExpr = caseNode->data.switchCase.expr;
    evalNode(caseExpr);
    Object* caseVal = contextGetLastEval();
    if (compareObjects(scrut, caseVal)) {
      evalNode(caseNode->data.switchCase.code);
      return;
    }
    cases = cases->next;
  }
}

void
evalRet(ASTNode* retStmt)
{
  if (!(contextGetFlags() & IN_FUNCTION)) {
    setError(false, "evalRet: ret outside of a function\n");
    return;
  }

  if (retStmt->data.ret.retValue) {
    evalNode(retStmt->data.ret.retValue);
    Object* val = contextGetLastEval();
    contextSetLastEval(val);
  } else {
    contextSetLastEval(nullObj());
  }

  returnFromFunction();
}

void
evalBrk(ASTNode* brkStmt)
{
  if (!(contextGetFlags() & IN_BREAKABLE)) {
    setError(false, "evalBrk: Not inside of a breakable construct\n");
    return;
  }

  // check condition if specified
  if (brkStmt->data.brk.condition) {
    evalNode(brkStmt->data.brk.condition);
    Object* cond = contextGetLastEval();

    // if condition is not Number or Bool type, then report error
    if (strcmp(cond->type, "Bool") && strcmp(cond->type, "Number")) {
      setError(false, "evalFor: Condition cannot be evaluated to Bool type\n");
      return;
    }

    if (!((Bool*)cond->data.objectPtr)->value) {
      return;
    }
  }

  // set broken flag so breakables will stop evaluating
  contextSetFlag(BROKEN);
}

void
evalToss(ASTNode* tossStmt)
{
  evalNode(tossStmt->data.toss.msg);
  Object* exObj = contextGetLastEval();
  if (strcmp(exObj->type, "String"))
    setError(
      false,
      "evalToss: Tossing anything other than strings is not implemented\n");

  releaseObject(exObj);
  runtimeRecover();
}

Object*
evalName(const char* name)
{
  Object** obj = contextLookup(name);
  if (!obj)
    setError(false,
             "evalName: %s is not defined in current or global context\n",
             name);

  return *obj;
}

Object*
evalArray(ASTNode* array)
{
  std::vector<Object*> elems;
  ASTNodeLL* it = array->data.array.elements;
  while (it) {
    evalNode(it->node);
    elems.push_back(contextGetLastEval());
    it = it->next;
  }

  return arrayObj(elems);
}

Object*
evalMemberAccess(ASTNode* membAccess)
{
  evalNode(membAccess->data.memberAccess.left);
  Object* left = contextGetLastEval();
  if (!left->isCompound) {
    setError(false, "evalMemberAccess: Not a compound type\n");
    return nullptr;
  }

  const char* rightName = membAccess->data.memberAccess.right->data.stringValue;
  const char* className =
    membAccess->data.memberAccess.left->type == NODE_NAME
      ? membAccess->data.memberAccess.left->data.stringValue
      : nullptr;
  Object** right = nullptr;

  if (className && contextLookupClass(className))
    right = contextLookupClassMember(className, rightName, nullptr, true);
  else
    right = contextLookupClassMember(left->type, rightName, nullptr, false);

  if (!right) {
    setError(false, "evalMemberAccess: Can't find %s\n", right);
    return nullptr;
  }

  return *right;
}

Object*
evalArrayAccess(ASTNode* array)
{
  // check if subscript has expression
  ASTNode* subscript = array->data.arrayAccess.subscript;
  if (!subscript) {
    setError(false, "evalArrayAccess: No subcript expression\n");
    return nullptr;
  }

  // evaluate chain to get array object
  evalNode(array->data.arrayAccess.left);
  Object* arrObj = contextGetLastEval();
  if (strcmp(arrObj->type, "Array")) {
    setError(false, "evalArrayAccess: Not an Array\n");
    return nullptr;
  }

  Array* arr = (Array*)arrObj->data.objectPtr;

  Object* result = nullptr;

  // evaluate subcript
  evalNode(subscript);
  Object* indexObj = contextGetLastEval();
  // if subcript is not a number, toss error
  if (strcmp(indexObj->type, "Number")) {
    setError(false, "evalArrayAccess: Index is not a Number\n");
    return nullptr;
  }

  // otherwise check for boundaries
  int index = (int)((Number*)indexObj->data.objectPtr)->value;
  if (index < 0 || index >= (int)arr->objects->size()) {
    setError(false, "evalArrayAccess: Index %d is outside of a range\n", index);
    return nullptr;
  }

  // if everything okay, return object at given index
  result = arr->objects->at(index);

  return result;
}

Object*
evalBinaryOp(ASTNode* node)
{
  // get lhs object
  evalNode(node->data.binaryOp.lhs);
  Object* lhs = contextGetLastEval();

  // get rhs object
  evalNode(node->data.binaryOp.rhs);
  Object* rhs = contextGetLastEval();

  OperatorType opType = (OperatorType)node->data.binaryOp.op;
  callBinaryOp(opType, lhs, rhs);

  return contextGetLastEval();
}

Object*
evalUnaryOp(ASTNode* node)
{
  // get object to apply unary op on
  evalNode(node->data.unaryOp.expr);
  Object* obj = contextGetLastEval();

  OperatorType opType = (OperatorType)node->data.unaryOp.op;
  callUnaryOp(opType, obj);

  return contextGetLastEval();
}

Object*
evalLambda(ASTNode* node)
{
  ASTNodeLL* it = node->data.lambda.captureList;
  std::vector<ParameterInfo> params;
  std::map<std::string, Object*> captureList;
  params.clear();
  captureList.clear();

  // create capture list for lambdas
  while (it) {
    // capture list only allows names
    if (it->node->type != NODE_NAME) {
      setError(false,
               "evalLambda: Only single identifiers allowed in capture list\n");
      return nullptr;
    }

    evalNode(it->node);
    Object* obj = contextGetLastEval();
    std::string name = it->node->data.stringValue;
    captureList[name] = obj;
    it = it->next;
  }

  // fill in parameters
  ASTNodeLL* paramsIt = node->data.lambda.params;
  while (paramsIt) {
    const char* paramName = paramsIt->node->data.paramInfo.name;
    // check if this name is already defined in capture list
    if (captureList.find(paramName) != captureList.end()) {
      setError(false,
               "evalLambda: Parameter %s is already defined in capture list\n",
               paramName);
      return nullptr;
    }

    const char* paramType =
      getInternedString(paramsIt->node->data.paramInfo.type->data.stringValue);

    params.push_back({ duplicateString(paramName, true), paramType });
    paramsIt = paramsIt->next;
  }

  Object* lmbdObj = lambdaObj(node->data.lambda.code, captureList, params);

  return lmbdObj;
}
