#include "treewalk.h"
#include "../front/ast.h"
#include "context.h"
#include "runtime.h"
#include <variant>
#include <vector>

Treewalk::Treewalk(Runtime& runtime)
  : _runtime(runtime)
{
}

Identifier
Treewalk::eval(const ASTNode* n)
{
  if (!n)
    return {};

  ASTNodeLL* list = n->data.nodeList.list;
  Identifier id = {};
  while (list) {
    id = evalNode(list->node);
    list = list->next;
  }

  freeNode(const_cast<ASTNode*>(n));
  return id;
}

Identifier
Treewalk::evalNode(const ASTNode* node)
{
  if (!node)
    return {};

  switch (node->type) {
    case NODE_LIST:
      return evalNodeList(node);
    case NODE_FN_DEF:
      return evalFnDef(node);
    case NODE_FN_CALL:
      return evalFnCall(node);
    case NODE_CLASS_DEF:
      return evalClassDef(node);
    case NODE_ENUM_DEF:
      return evalEnumDef(node);
    case NODE_VAR_DECL:
      return evalVarDecl(node);
    case NODE_VAR_ASSIGN:
      return evalVarAssign(node);
    case NODE_WHL:
      return evalWhl(node);
    case NODE_FOR:
      return evalFor(node);
    case NODE_IF:
      return evalIf(node);
    case NODE_SWITCH:
      return evalSwitch(node);
    case NODE_RET:
      return evalRet(node);
    case NODE_BRK:
      return evalBrk(node);
    case NODE_TOSS:
      return evalToss(node);
    case NODE_BINARYOP:
      return evalBinaryOp(node);
    case NODE_UNARYOP:
      return evalUnaryOp(node);
    case NODE_MEMBER_ACCESS:
      return evalMemberAccess(node);
    case NODE_ARR_ACCESS:
      return evalArrayAccess(node);
    case NODE_BOOL:
      return _runtime.boolObj(node->data.boolValue);
    case NODE_NUMBER:
      return _runtime.numberObj(node->data.numberValue);
    case NODE_STRING:
      return _runtime.stringObj(node->data.stringValue, true);
    case NODE_ARRAY:
      return evalArray(node);
    case NODE_NAME:
      return evalName(node->data.stringValue);
    default:
      _runtime.error("evalNode: Unknown node type %d\n", (int)node->type);
      return {};
  }
}

Identifier
Treewalk::evalNodeList(const ASTNode* nodeList)
{
  ASTNodeLL* it = nodeList->data.nodeList.list;
  Identifier id{};
  while (it) {
    id = evalNode(it->node);
    it = it->next;
  }

  return id;
}

Identifier
Treewalk::evalEnumDef(const ASTNode* enumDef)
{
  int prev = 0;
  ASTNodeLL* it = enumDef->data.enumDef.elements;
  while (it) {
    ASTNode* elem = it->node;
    ASTNode* rhs = elem->data.enumElement.expr;
    Identifier id;
    if (rhs) {
      id = evalNode(rhs);
      auto obj = std::get_if<ObjectHeader>(&id.name);
      if (!obj || obj->valueType != ValueType::Number) {
        _runtime.error("evalEnumDef: Enumerator value must be Number type\n");
        return {};
      }
    } else {
      id = _runtime.numberObj((double)prev);
    }

    // register enum value in global context
    _runtime.defineVariable(elem->data.enumElement.name, id);
    ++prev;
    it = it->next;
  }

  return {};
}

Identifier
Treewalk::evalFnDef(const ASTNode* fnDef)
{
  const char* fnName = fnDef->data.fnDef.name;
  std::vector<const char*> paramNames;
  std::vector<TypeID> paramTypes;

  // fill in parameters
  ASTNodeLL* paramsIt = fnDef->data.fnDef.params;
  while (paramsIt) {
    const char* paramName = paramsIt->node->data.paramInfo.name;
    const char* paramType = _runtime.getInternedString(
      paramsIt->node->data.paramInfo.type->data.stringValue);

    paramNames.push_back(_runtime.getInternedString(paramName));
    Identifier id = _runtime.lookup(paramType);
    auto classID = std::get_if<TypeID>(&id.name);
    if (!classID) {
      _runtime.error("evalFnDef: %s is not a type", paramType);
      return {};
    }

    paramTypes.push_back(*classID);
    paramsIt = paramsIt->next;
  }

  _runtime.defineFunction(
    fnName, false, paramNames, paramTypes, fnDef->data.fnDef.code, nullptr);

  return {};
}

Identifier
Treewalk::evalFnCall(const ASTNode* fnCall)
{
  ASTNode* callee = fnCall->data.fnCall.callee;
  std::vector<ObjectHeader> args;
  ASTNodeLL* it = fnCall->data.fnCall.arguments;
  while (it) {
    Identifier id = evalNode(it->node);
    auto obj = std::get_if<ObjectHeader>(&id.name);
    if (!obj) {
      _runtime.error("evalFnCall: argument is not an object\n");
      return {};
    }

    args.push_back(*obj);
    it = it->next;
  }

  // if node is just a name, then try if its a class ctor
  if (callee->type == NODE_NAME) {
    const char* className = callee->data.stringValue;
    Identifier id = _runtime.lookup(className);
    auto classID = std::get_if<TypeID>(&id.name);
    if (classID) {
      Identifier instance = _runtime.classInstance(*classID, args);
      return instance;
    }
  }

  // if its a member access, resolve left part first, and then call method of
  // the right on result instance
  if (callee->type == NODE_MEMBER_ACCESS) {
    Identifier id = evalMemberAccess(callee->data.memberAccess.left);
    if (!std::holds_alternative<ObjectHeader>(id.name)) {
      _runtime.error("evalFnCall: lhs is not an object");
      return {};
    }

    const char* mthdName = callee->data.memberAccess.right->data.stringValue;
    Identifier mthdNameID_ = _runtime.lookupInClass(mthdName, id);
    auto mthdNameID = std::get_if<FnNameID>(&mthdNameID_.name);
    if (!mthdNameID) {
      _runtime.error("evalFnCall: no method with name %s on lhs", mthdName);
      return {};
    }

    return _runtime.call(*mthdNameID, args, id);
  }

  else {
    // find callee object cause its lambda or global function
    Identifier id = evalNode(callee);
    auto fnNameID = std::get_if<FnNameID>(&id.name);
    if (!fnNameID) {
      _runtime.error("evalFnCall: Callee is not a callable\n");
      return {};
    }

    return _runtime.call(*fnNameID, args);
  }
}

Identifier
Treewalk::evalClassDef(const ASTNode* classDef)
{
  const char* className = classDef->data.classDef.name;
  std::vector<ClassMethod> methods;
  std::vector<ClassField> fields;

  ASTNodeLL* it = classDef->data.classDef.members;
  size_t fieldIdx = 0;
  while (it) {
    ASTNode* node = it->node;
    Modifier mod = {};
    if (node->type == NODE_CLASSFIELD_DEF) {
      mod = node->data.classMethodDef.modifier;
      fields.push_back({ node->data.classFieldDef.name, mod, fieldIdx++ });
    } else if (node->type == NODE_CLASSMETHOD_DEF) {
      mod = node->data.classMethodDef.modifier;
      ASTNode* fnDef = node->data.classMethodDef.function;
      const char* methodName = fnDef->data.fnDef.name;

      ASTNodeLL* paramsIt = fnDef->data.fnDef.params;
      std::vector<const char*> paramNames;
      std::vector<TypeID> paramTypes;
      while (paramsIt) {
        const char* paramName = paramsIt->node->data.paramInfo.name;
        const char* paramType =
          paramsIt->node->data.paramInfo.type->data.stringValue;

        paramNames.push_back(paramName);
        auto paramTypeID_ = _runtime.lookup(paramType);
        auto paramTypeID = std::get_if<TypeID>(&paramTypeID_.name);
        if (!paramTypeID) {
          _runtime.error("evalClassDef: unknown parameter type %s", paramType);
          return {};
        }

        paramTypes.push_back(*paramTypeID);
        paramsIt = paramsIt->next;
      }

      // using local variables into pointers might be dangerous here
      methods.push_back({ mod,
                          methodName,
                          false,
                          &paramNames,
                          &paramTypes,
                          fnDef->data.fnDef.code,
                          nullptr });
    }

    it = it->next;
  }

  // TODO: implement static fields
  _runtime.defineClass(className, false, false, methods, fields, {});

  return {};
}

Identifier
Treewalk::evalVarDecl(const ASTNode* varDecl)
{
  const char* name = varDecl->data.varDecl.name;
  Identifier value = _runtime.nullObj();
  // if assignment declaration, try to eval value
  if (varDecl->data.varDecl.expr)
    value = evalNode(varDecl->data.varDecl.expr);

  _runtime.defineVariable(name, value);
  return value;
}

Identifier
Treewalk::evalVarAssign(const ASTNode* varAssign)
{
  // get location of lhs by evaluating it
  Identifier lhs = evalNode(varAssign->data.varAssign.lhs);
  auto lhsObj = std::get_if<ObjectHeader>(&lhs.name);
  if (!lhsObj) {
    _runtime.error("evalVarAssign: lhs is not an object");
    return {};
  } else if (lhs.locationHandle == 0) {
    _runtime.error("evalVarAssign: Cannot resolve location of lhs");
    return {};
  }

  size_t locationHandle = lhs.locationHandle;
  if (lhsObj->isConst) {
    _runtime.error("evalVarAssign: Trying to assign to constant object");
    return {};
  }

  // get value of rhs by evaluating it
  Identifier rhs = evalNode(varAssign->data.varAssign.rhs);
  auto rhsObj = std::get_if<ObjectHeader>(&rhs.name);
  if (!rhsObj) {
    _runtime.error("evalVarAssign: Trying to assign a non-object");
    return {};
  }

  OpType op = varAssign->data.varAssign.op;
  switch (op) {
    case OP_ASSIGN: {
      break;
    }
    case OP_ASSIGN_ADD:
    case OP_ASSIGN_SUB:
    case OP_ASSIGN_DIV:
    case OP_ASSIGN_MUL:
    case OP_ASSIGN_MOD:
    case OP_ASSIGN_XOR:
    case OP_ASSIGN_SHIFTL:
    case OP_ASSIGN_SHIFTR:
    case OP_ASSIGN_AND:
    case OP_ASSIGN_OR: {
      OpType binaryOp =
        static_cast<OpType>(to_underlying(op) - to_underlying(OP_ASSIGN));

      // TODO: make this work with builtins
      const char* mthdName = op2String(binaryOp);
      if (_runtime.isPrimitive(lhsObj->objClass))
        mthdName = "binary";

      Identifier opNameID_ =
        _runtime.lookupInClass(mthdName, { 0, lhsObj->objClass });
      auto opNameID = std::get_if<FnNameID>(&opNameID_.name);
      if (!opNameID) {
        _runtime.error("evalVarAssign: No defined operator overload lhs\n");
        return {};
      }

      std::vector<ObjectHeader> args = { *rhsObj };

      // TODO: remove this and fix builtins calling
      if (_runtime.isPrimitive(lhsObj->objClass)) {
        args.insert(args.begin(), *lhsObj);
        ObjectHeader opObj = std::get<ObjectHeader>(
          _runtime.numberObj(static_cast<double>(binaryOp)).name);
        args.insert(args.begin(), opObj);
      }

      rhs = _runtime.call(*opNameID, args, lhs);
      break;
    }
    default: {
      _runtime.error("evalVarAssign: Wrong assignment operator\n");
      return {};
    }
  }

  _runtime.tryAssign(lhs.locationHandle, rhs);
  return rhs;
}

Identifier
Treewalk::evalWhl(const ASTNode* loopWhl)
{
  ASTNode* cond = loopWhl->data.loopWhl.condition;
  ASTNode* code = loopWhl->data.loopWhl.code;

  // enable breakable flag
  _runtime.setContextFlag(ContextFlags::Breakable);
  while (true) {
    // evaluate condition
    Identifier id = evalNode(cond);
    bool condition = false;
    auto condObj = std::get_if<ObjectHeader>(&id.name);
    if (!condObj) {
      _runtime.error("evalWhl: Non-evaluable loop condition\n");
      return {};
    }

    ObjectHeader val = *condObj;
    if (val.valueType == ValueType::Bool) {
      condition = val.data.boolValue;
    } else if (val.valueType == ValueType::Number) {
      condition = val.data.numValue != 0;
    } else {
      _runtime.error("evalWhl: Condition cannot be evaluated to Bool type\n");
      return {};
    }

    if (!condition)
      break;

    evalNode(code);

    if (to_underlying(_runtime.getContextFlags()) &
        to_underlying(ContextFlags::Broken)) {
      _runtime.resetContextFlag(ContextFlags::Broken);
      break;
    }
  }

  _runtime.resetContextFlag(ContextFlags::Breakable);
  return {};
}

Identifier
Treewalk::evalFor(const ASTNode* loopFor)
{
  // enable breakable flag
  _runtime.setContextFlag(ContextFlags::Breakable);

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
      Identifier id = evalNode(it->node);
      bool condition = false;
      auto condObj = std::get_if<ObjectHeader>(&id.name);
      if (!condObj) {
        _runtime.error("evalFor: Non-evaluable loop condition\n");
        return {};
      }

      ObjectHeader val = *condObj;
      if (val.valueType == ValueType::Bool) {
        condition = val.data.boolValue;
      } else if (val.valueType == ValueType::Bool) {
        condition = val.data.numValue != 0;
      } else {
        _runtime.error("evalFor: Condition cannot be evaluated to Bool type\n");
        return {};
      }

      if (!condition) {
        runningLoop = false;
        break;
      }
    }

    if (!runningLoop)
      break;

    evalNode(code);
    if (to_underlying(_runtime.getContextFlags()) &
        to_underlying(ContextFlags::Broken)) {
      _runtime.resetContextFlag(ContextFlags::Broken);
      break;
    }

    // evaluate post-iteration assignments
    ASTNodeLL* postIt = post;
    while (postIt) {
      evalNode(postIt->node);
      postIt = postIt->next;
    }
  }

  _runtime.resetContextFlag(ContextFlags::Breakable);
  return {};
}

Identifier
Treewalk::evalIf(const ASTNode* ifStmt)
{
  ASTNode* cond = ifStmt->data.ifBlock.condition;
  ASTNode* thenBlock = ifStmt->data.ifBlock.code;
  ASTNode* elseBlock = ifStmt->data.ifBlock.elseBlock;
  Identifier id = evalNode(cond);
  bool condition = false;

  auto condObj = std::get_if<ObjectHeader>(&id.name);
  if (!condObj) {
    _runtime.error("evalIf: Non-evaluable loop condition\n");
    return {};
  }

  ObjectHeader val = *condObj;
  if (val.valueType == ValueType::Bool) {
    condition = val.data.boolValue;
  } else if (val.valueType == ValueType::Bool) {
    condition = val.data.numValue != 0;
  } else {
    _runtime.error("evalIf: Condition cannot be evaluated to Bool type\n");
    return {};
  }

  if (condition)
    evalNode(thenBlock);
  else if (elseBlock)
    evalNode(elseBlock);

  return {};
}

Identifier
Treewalk::evalSwitch(const ASTNode* switchStmt)
{
  ASTNode* expr = switchStmt->data.switchStmt.expr;
  ASTNodeLL* cases = switchStmt->data.switchStmt.cases;
  Identifier id = evalNode(expr);
  auto exprObj = std::get_if<ObjectHeader>(&id.name);
  if (!exprObj) {
    _runtime.error("evalSwitch: Non-evaluable switch expression\n");
    return {};
  }

  ObjectHeader exprValue = *exprObj;
  while (cases) {
    ASTNode* caseNode = cases->node;
    if (caseNode->data.switchCase.isDefault) {
      return evalNode(caseNode->data.switchCase.code);
    }

    ASTNode* caseExpr = caseNode->data.switchCase.expr;
    Identifier caseId = evalNode(caseExpr);
    auto caseObj = std::get_if<ObjectHeader>(&id.name);
    if (!caseObj) {
      _runtime.error("evalSwitch: Non-evaluable case expression\n");
      return {};
    }

    ObjectHeader caseVal = *caseObj;
    if (_runtime.compareObjects(exprValue, caseVal)) {
      return evalNode(caseNode->data.switchCase.code);
    }

    cases = cases->next;
  }

  return {};
}

Identifier
Treewalk::evalRet(const ASTNode* retStmt)
{
  if (!(to_underlying(_runtime.getContextFlags()) &
        to_underlying(ContextFlags::Function))) {
    _runtime.error("evalRet: ret outside of a function\n");
    return {};
  }

  if (retStmt->data.ret.retValue) {
    _runtime.setLastEval(evalNode(retStmt->data.ret.retValue));
  } else {
    _runtime.setLastEval(_runtime.nullObj());
  }

  _runtime.returnFromFunction();

  return _runtime.getLastEval();
}

Identifier
Treewalk::evalBrk(const ASTNode* brkStmt)
{
  if (!(to_underlying(_runtime.getContextFlags()) &
        to_underlying(ContextFlags::Breakable))) {
    _runtime.error("evalBrk: Not inside of a breakable construct\n");
    return {};
  }

  Identifier id = {};
  // check condition if specified
  if (brkStmt->data.brk.condition) {
    id = evalNode(brkStmt->data.brk.condition);
    bool condition = false;
    auto condObj = std::get_if<ObjectHeader>(&id.name);
    if (!condObj) {
      _runtime.error("evalBrk: Non-evaluable break condition\n");
      return {};
    }

    ObjectHeader val = *condObj;
    if (val.valueType == ValueType::Bool) {
      condition = val.data.boolValue;
    } else if (val.valueType == ValueType::Bool) {
      condition = val.data.numValue != 0;
    } else {
      _runtime.error("evalBrk: Condition cannot be evaluated to Bool type\n");
      return {};
    }

    if (!condition)
      return {};
  }

  // set broken flag so breakables will stop evaluating
  _runtime.setContextFlag(ContextFlags::Broken);
  return id;
}

Identifier
Treewalk::evalToss(const ASTNode* tossStmt)
{
  Identifier id = evalNode(tossStmt->data.toss.msg);
  auto tossObj = std::get_if<ObjectHeader>(&id.name);
  if (!tossObj) {
    _runtime.error("evalToss: Not an object\n");
    return {};
  }

  ObjectHeader exObj = *tossObj;
  if (exObj.valueType != ValueType::String) {
    _runtime.error(
      "evalToss: Tossing anything other than strings is not implemented\n");
    return {};
  } else
    // this error doesnt have to return empty Identifier since we returning
    // evaluated expression
    _runtime.error("Exception: %s\n", exObj.data.stringValue);

  return id;
}

Identifier
Treewalk::evalName(const char* name)
{
  Identifier res = _runtime.lookup(name);
  if (std::holds_alternative<std::monostate>(res.name))
    _runtime.error("evalName: %s is undefined\n", name);

  return res;
}

Identifier
Treewalk::evalArray(const ASTNode* array)
{
  std::vector<ObjectHeader> elems;
  ASTNodeLL* it = array->data.array.elements;
  while (it) {
    Identifier el = evalNode(it->node);
    auto elObj = std::get_if<ObjectHeader>(&el.name);
    if (!elObj) {
      _runtime.error("evalArray: Array element is not an object\n");
      return {};
    }

    elems.push_back(*elObj);
    it = it->next;
  }

  return _runtime.arrayObj(elems);
}

Identifier
Treewalk::evalMemberAccess(const ASTNode* membAccess)
{
  const ASTNode* left = membAccess;
  std::vector<const ASTNode*> parts;

  // build member access chain where last node is the most inner
  while (left->type == NODE_MEMBER_ACCESS) {
    ASTNode* right = left->data.memberAccess.right;
    left = left->data.memberAccess.left;

    parts.push_back(right);
    parts.push_back(left);
  }

  Identifier id = evalNode(parts.back());

  // eval member access chain
  for (auto it = std::next(parts.rbegin()); it != parts.rend(); ++it) {
    auto membObj = std::get_if<ObjectHeader>(&id.name);
    if (!membObj) {
      _runtime.error("evalMemberAccess: Member is not an object\n");
      return {};
    } else if (membObj->objClass == dynamicTypeID) {
      _runtime.error("evalMemberAccess: not a class instance");
      return {};
    }

    const ASTNode* right = *it;
    id = _runtime.lookupInClass(right->data.stringValue, id);
  }

  return id;
}

Identifier
Treewalk::evalArrayAccess(const ASTNode* arrayAccess)
{
  const ASTNode* left = arrayAccess;
  std::vector<const ASTNode*> parts;

  // build member access chain where last node is the most inner
  while (left->type == NODE_ARR_ACCESS) {
    // check if subscript has expression
    ASTNode* subscript = arrayAccess->data.arrayAccess.subscript;
    if (!subscript) {
      _runtime.error("evalArrayAccess: No subcript expression\n");
      return {};
    }

    ASTNode* right = left->data.memberAccess.right;
    left = left->data.memberAccess.left;

    parts.push_back(right);
    parts.push_back(left);
  }

  Identifier id = evalNode(parts.back());
  // eval array access chain
  for (auto it = std::next(parts.rbegin()); it != parts.rend(); ++it) {
    auto arrObj = std::get_if<ObjectHeader>(&id.name);
    if (!arrObj) {
      _runtime.error("evalArrayAccess: Member is not an object\n");
      return {};
    } else if (arrObj->objClass == dynamicTypeID) {
      _runtime.error("evalArrayAccess: not a class instance");
      return {};
    }

    const ASTNode* idxExpr = *it;
    Identifier idxVal = evalNode(idxExpr);
    auto idxObj = std::get_if<ObjectHeader>(&idxVal.name);
    if (!idxObj || idxObj->valueType != ValueType::Number) {
      _runtime.error("evalArrayAccess: index specificator is not a number");
      return {};
    }

    double idx = idxObj->data.numValue;
    // check if index is integer
    if (!_runtime.isPositiveInteger(idx)) {
      _runtime.error(
        "evalArrayAccess: index specificator has to be a positive integer");
      return {};
    }

    id = _runtime.accessArray(id, idx);
  }

  return id;
}

Identifier
Treewalk::evalBinaryOp(const ASTNode* node)
{
  // get lhs object
  Identifier lhs = evalNode(node->data.binaryOp.lhs);
  auto lhsObj = std::get_if<ObjectHeader>(&lhs.name);
  if (!lhsObj) {
    _runtime.error("evalBinaryOp: lhs is not an object\n");
    return {};
  }

  // get rhs object
  Identifier rhs = evalNode(node->data.binaryOp.rhs);
  auto rhsObj = std::get_if<ObjectHeader>(&rhs.name);
  if (!lhsObj) {
    _runtime.error("evalBinaryOp: rhs is not an object\n");
    return {};
  }

  OpType op = node->data.binaryOp.op;
  std::vector<ObjectHeader> args = { *lhsObj, *rhsObj };
  const char* mthdName = op2String(op);
  if (_runtime.isPrimitive(lhsObj->objClass))
    mthdName = "binary";

  Identifier opNameID_ =
    _runtime.lookupInClass(mthdName, { 0, lhsObj->objClass });
  auto opNameID = std::get_if<FnNameID>(&opNameID_.name);
  if (!opNameID) {
    _runtime.error("evalBinaryOp: No defined operator overload lhs\n");
    return {};
  }

  // if lhs is primitive then use builtin method
  if (_runtime.isPrimitive(lhsObj->objClass)) {
    Identifier opId = _runtime.numberObj(static_cast<double>(op));
    auto objHdr = std::get<ObjectHeader>(opId.name);
    args.insert(args.begin(), objHdr);
  }

  return _runtime.call(*opNameID, args, lhs);
}

Identifier
Treewalk::evalUnaryOp(const ASTNode* node)
{
  // get object to apply unary op on
  Identifier lhs = evalNode(node->data.unaryOp.expr);
  auto lhsObj = std::get_if<ObjectHeader>(&lhs.name);
  if (!lhsObj) {
    _runtime.error("evalUnaryOp: operand is not an object\n");
    return {};
  }

  OpType op = node->data.unaryOp.op;
  std::vector<ObjectHeader> args;
  const char* mthdName = op2String(op);
  if (_runtime.isPrimitive(lhsObj->objClass))
    mthdName = "unary";

  Identifier opNameID_ =
    _runtime.lookupInClass(mthdName, { 0, lhsObj->objClass });
  auto opNameID = std::get_if<FnNameID>(&opNameID_.name);
  if (!opNameID) {
    _runtime.error("evalUnaryOp: No defined operator overload lhs\n");
    return {};
  }

  // if lhs object type is primitive then use builtin method
  if (_runtime.isPrimitive(lhsObj->objClass)) {
    args.push_back(*lhsObj);
    Identifier opId = _runtime.numberObj(static_cast<double>(op));
    auto objHdr = std::get<ObjectHeader>(opId.name);
    args.insert(args.begin(), objHdr);
  }

  return _runtime.call(*opNameID, args, lhs);
}
