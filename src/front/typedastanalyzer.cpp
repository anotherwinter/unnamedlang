#include "front/typedastanalyzer.h"
#include "errorcode_bases.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include <variant>

using namespace HIR;

TypedASTAnalyzer::TypedASTAnalyzer(Diagnostics& diag,
                                   SymbolRegistry& reg,
                                   NodeAllocator& alloc)
  : _diag(diag)
  , _reg(reg)
  , _alloc(alloc)
  , _ignoreSelfInjecting(false)
{
  initPrebuilts();
}

void
TypedASTAnalyzer::initPrebuilts()
{
  _boolType = _reg.resolveClass("Bool");
  _numberType = _reg.resolveClass("Number");
  _stringType = _reg.resolveClass("String");
  _arrayType = _reg.resolveClass("Array");
}

void
TypedASTAnalyzer::analyzeTypedNode(TypedNode* node)
{
  if (!node)
    return;

  std::visit(
    Overloaded{
      [&](const std::monostate&) {},
      [&](const NodeList& list) { analyzeNodeList(list); },
      [&](const FnDef& fnDef) { analyzeFnDef(node); },
      [&](const CallExpr& callExpr) { analyzeCallExpr(node); },
      [&](const ClassDef& classDef) { analyzeClassDef(node); },
      [&](const EnumDef& enumDef) { analyzeEnumDef(node); },
      [&](const VarDecl& varDecl) { analyzeVarDecl(node); },
      [&](const VarAssign& varAssign) { analyzeVarAssign(node); },
      [&](const LoopWhl& loopWhl) { analyzeWhl(node); },
      [&](const LoopFor& loopFor) { analyzeFor(node); },
      [&](const StmtIf& stmtIf) { analyzeIf(node); },
      [&](const StmtSwitch& stmtSwitch) { analyzeSwitch(node); },
      [&](const StmtRet& stmtRet) { analyzeRet(node); },
      [&](const StmtBrk& stmtBrk) { analyzeBrk(node); },
      [&](const BinaryExpr& binaryExpr) { analyzeBinaryExpr(node); },
      [&](const UnaryExpr& unaryExpr) { analyzeUnaryExpr(node); },
      [&](const MemberAccess& membAccess) { analyzeMemberAccess(node); },
      [&](const ArrayAccess& arrAccess) { analyzeArrayAccess(node); },
      [&](const BoolVal& boolVal) { analyzeBool(node); },
      [&](const NumVal& numVal) { analyzeNumber(node); },
      [&](const StringVal& stringVal) { analyzeString(node); },
      [&](const ArrayVal& arrVal) { analyzeArray(node); },
      [&](const NameExpr& nameExpr) { analyzeName(node); },
      [&](const SelfExpr& selfExpr) {},
    },
    node->node);
}

void
TypedASTAnalyzer::analyzeNodeList(const NodeList& list)
{
  for (auto& n : list.nodes)
    analyzeTypedNode(n);
}

void
TypedASTAnalyzer::analyzeFnDef(TypedNode* node)
{
  auto& fnDef = std::get<FnDef>(node->node);
  FunctionID fnID = fnDef.fnID;
  if (!isValid(fnID)) {
    _diag.putMsg(STUB_ERR, node->line, node->col);
    return;
  }

  FunctionInfo* fnInfo = _reg.resolveFunction(fnID);
  std::vector<FnResolution>* overloads = _reg.resolveFunctionOverloads(
    fnDef.nameID, fnInfo->paramTypes, fnDef.ownerID, true);
  // overloads should be never nullptr here since at predeclare pass fninfo
  // was inserted for given definition
  if (overloads == nullptr) {
    _diag.putMsg(STUB_ERR, node->line, node->col);
    return;
  }

  // if found more than one exact signature
  if (overloads->size() != 1) {
    _diag.putMsg(STUB_ERR, node->line, node->col);
    return;
  }

  _reg.pushScope(fnInfo->ownerID, fnInfo->id);
  analyzeTypedNode(fnDef.body);
  _reg.popScope();
}

void
TypedASTAnalyzer::analyzeCallExpr(TypedNode* node)
{
  auto& callExpr = std::get<CallExpr>(node->node);
  analyzeTypedNode(callExpr.callee);
  for (auto& a : callExpr.args)
    analyzeTypedNode(a);

  auto callRes = resolveCallExpr(callExpr);
  if (callRes.resolved && !std::holds_alternative<FnResolution>(callRes.res)) {
    _diag.putMsg(STUB_ERR, node->line, node->col);

  }

  else if (callRes.resolved && !_ignoreSelfInjecting && !_reg.isGlobalScope()) {
    MemberAccess membAccess = {};

    auto callNode = _alloc.allocTypedNode<CallExpr>(node->line, node->col);
    callNode->node = callExpr;

    membAccess.base = _alloc.allocTypedNode<SelfExpr>(node->line, node->col);
    membAccess.memb.push_back(callNode);

    node->node = membAccess;
  }
}

void
TypedASTAnalyzer::analyzeClassDef(TypedNode* node)
{
  auto& classDef = std::get<ClassDef>(node->node);
  std::set<const char*> duplicates;
  for (auto& f : classDef.fields) {
    if (duplicates.find(f) == duplicates.end())
      duplicates.emplace(f);
    else
      _diag.putMsg(STUB_ERR, node->line, node->col);
  }

  for (auto& m : classDef.methods)
    analyzeFnDef(m);
}

void
TypedASTAnalyzer::analyzeEnumDef(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeVarDecl(TypedNode* node)
{
  auto& varDecl = std::get<VarDecl>(node->node);
  auto curScopeRes = _reg.resolveVarCurScope(varDecl.name);
  // if variable, not a field is already declared in current scope
  if (std::holds_alternative<VarInfo>(curScopeRes.nameID) &&
      !isValid(curScopeRes.ownerID))
    _diag.putMsg(STUB_ERR, node->line, node->col);
  else {
    VarID id = _reg.declareVariable(varDecl.name, varDecl.type, varDecl.mod);
    varDecl.id = id;
  }

  analyzeTypedNode(varDecl.val);
  auto valType = inferType(varDecl.val);
  // if declaration has static type but expression value is not the same type
  // TODO: add type inheritance/type compatibility
  if (isValid(varDecl.type) && varDecl.type != valType)
    _diag.putMsg(STUB_ERR, varDecl.val->line, varDecl.val->col);
}

void
TypedASTAnalyzer::analyzeVarAssign(TypedNode* node)
{
  auto& varAssign = std::get<VarAssign>(node->node);
  analyzeTypedNode(varAssign.lhs);
  analyzeTypedNode(varAssign.rhs);

  auto lhs = resolve(varAssign.lhs);
  // dont try to analyze further if lhs cannot be resolved
  if (!lhs.resolved)
    return;

  TypeID lhsType = inferType(varAssign.lhs);
  TypeID rhsType = inferType(varAssign.rhs);

  // if assignment with binary operation
  if (varAssign.op > ExprOp::Assign) {
    auto op = static_cast<OperatorID>(to_underlying(varAssign.op) -
                                      to_underlying(ExprOp::AssignAdd));
    auto opRes =
      _reg.resolveFunction(FnNameID{ to_underlying(op) }, { rhsType }, lhsType);
    if (!isValid(opRes.id)) {
      opRes = _reg.resolveFunction(
        FnNameID{ to_underlying(op) }, { lhsType }, rhsType);
    }

    if (isValid(opRes.id) && lhsType != opRes.returnType)
      _diag.putMsg(STUB_ERR, node->line, node->col);
  }
}

void
TypedASTAnalyzer::analyzeWhl(TypedNode* node)
{
  auto& loopWhl = std::get<LoopWhl>(node->node);
  analyzeTypedNode(loopWhl.cond);
  analyzeTypedNode(loopWhl.body);
}

void
TypedASTAnalyzer::analyzeFor(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeIf(TypedNode* node)
{
  auto& stmtIf = std::get<StmtIf>(node->node);
  analyzeTypedNode(stmtIf.cond);
  analyzeTypedNode(stmtIf.body);
  analyzeTypedNode(stmtIf.elseBody);
}

void
TypedASTAnalyzer::analyzeSwitch(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeRet(TypedNode* node)
{
  auto& stmtRet = std::get<StmtRet>(node->node);
  analyzeTypedNode(stmtRet.retValue);
}

void
TypedASTAnalyzer::analyzeBrk(TypedNode* node)
{
  auto& stmtBrk = std::get<StmtBrk>(node->node);
  analyzeTypedNode(stmtBrk.cond);
}

void
TypedASTAnalyzer::analyzeBinaryExpr(TypedNode* node)
{
  auto& binary = std::get<BinaryExpr>(node->node);
  analyzeTypedNode(binary.lhs);
  analyzeTypedNode(binary.rhs);

  auto lhsType = inferType(binary.lhs);
  auto rhsType = inferType(binary.rhs);

  // if unexpected op
  if (binary.op >= ExprOp::Assign)
    _diag.putMsg(STUB_ERR, node->line, node->col);

  // try to resolve operator statically only if both operands can be resolved
  // statically
  if (isValid(lhsType) && isValid(rhsType)) {
    // try to find op in lhs
    auto fnRes =
      _reg.resolveFunction({ to_underlying(binary.op) }, { rhsType }, lhsType);

    // if failed, try to find in rhs
    if (!isValid(fnRes.id))
      fnRes = _reg.resolveFunction(
        { to_underlying(binary.op) }, { lhsType }, rhsType);

    if (!isValid(fnRes.id)) {
      _diag.putMsg(STUB_ERR, binary.lhs->line, binary.lhs->col);
      return;
    }

    binary.type = fnRes.returnType;
  }
}

void
TypedASTAnalyzer::analyzeUnaryExpr(TypedNode* node)
{
  auto& unary = std::get<UnaryExpr>(node->node);
  analyzeTypedNode(unary.expr);
  auto exprType = inferType(unary.expr);

  // if unexpected op
  if (unary.op < ExprOp::UnaryNeg || unary.op > ExprOp::UnaryDec)
    _diag.putMsg(STUB_ERR, node->line, node->col);

  // try to resolve operator statically
  if (isValid(exprType)) {
    auto fnRes =
      _reg.resolveFunction({ to_underlying(unary.op) }, {}, exprType);

    if (!isValid(fnRes.id)) {
      _diag.putMsg(STUB_ERR, unary.expr->line, unary.expr->col);
      return;
    }

    unary.type = fnRes.returnType;
  }
}

void
TypedASTAnalyzer::analyzeMemberAccess(TypedNode* node)
{
  auto& membAccess = std::get<MemberAccess>(node->node);
  auto res = resolveMemberAccess(membAccess);

  // if not in global scope and injecting of "self" enabled
  if (membAccess.resolvedBase && !_ignoreSelfInjecting &&
      !_reg.isGlobalScope()) {
    membAccess.memb.insert(membAccess.memb.begin(), membAccess.base);
    membAccess.base = _alloc.allocTypedNode<SelfExpr>(node->line, node->col);
  }

  // error if ownerid is valid (means base is resolved statically), but member
  // cannot be resolved statically
  if (membAccess.resolvedBase && !membAccess.resolvedChain)
    _diag.putMsg(STUB_ERR, node->line, node->col);
}

void
TypedASTAnalyzer::analyzeArrayAccess(TypedNode* node)
{
  // STUB: add bounds check (?) and check if index is integer
  // add "self" injecting
}

void
TypedASTAnalyzer::analyzeBool(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeNumber(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeString(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeArray(TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeName(TypedNode* node)
{
  auto& nameExpr = std::get<NameExpr>(node->node);
  auto nameRes = resolveNameExpr(nameExpr);
  if (!nameRes.resolved) {
    _diag.putMsg(STUB_ERR, node->line, node->col);
    return;
  }

  if (!_ignoreSelfInjecting && isValid(nameRes.ownerID)) {
    MemberAccess membAccess = {};

    TypedNode* nameNode =
      _alloc.allocTypedNode<NameExpr>(node->line, node->col);
    nameNode->node = nameExpr;

    membAccess.base = _alloc.allocTypedNode<SelfExpr>(node->line, node->col);
    membAccess.memb.push_back(nameNode);

    node->node = membAccess;
  }
}

NameResolutionResult
TypedASTAnalyzer::resolve(TypedNode* node)
{
  if (!node)
    return {};

  auto res = std::visit(
    Overloaded{
      [&](auto&&) { return NameResolutionResult{}; },
      [&](CallExpr& callExpr) { return resolveCallExpr(callExpr); },
      [&](MemberAccess& membAccess) { return resolveMemberAccess(membAccess); },
      [&](ArrayAccess& arrAccess) { return resolveArrayAccess(arrAccess); },
      [&](NameExpr& nameExpr) { return resolveNameExpr(nameExpr); },
      [&](SelfExpr& selfExpr) { return resolveSelfExpr(); },
    },
    node->node);

  return res;
}

NameResolutionResult
TypedASTAnalyzer::resolveCallExpr(CallExpr& callExpr)
{
  // if already resolved
  if (callExpr.resType == CallExpr::ResolutionType::Static)
    return { {}, callExpr.fnRes.returnType, callExpr.fnRes, true };

  auto calleeRes = resolve(callExpr.callee);
  auto fnNameID = std::get_if<FnNameID>(&calleeRes.res);

  // this will be true if failed to resolve statically
  if (!fnNameID)
    return calleeRes;

  std::vector<TypeID> types;
  for (auto& a : callExpr.args) {
    auto argType = resolve(a);
    if (!argType.resolved)
      return {};
  }

  auto fnRes = _reg.resolveFunction(*fnNameID, types, calleeRes.ownerID);
  if (!isValid(fnRes.id))
    return {};

  callExpr.resType = CallExpr::ResolutionType::Static;
  callExpr.fnRes = fnRes;

  return { calleeRes.ownerID, fnRes.returnType, fnRes, true };
}

NameResolutionResult
TypedASTAnalyzer::resolveMemberAccess(MemberAccess& membAccess)
{
  auto left = resolve(membAccess.base);
  if (!left.resolved || !isValid(left.type))
    return {};

  membAccess.resolvedBase = true;
  bool selfTemp = _ignoreSelfInjecting;
  _ignoreSelfInjecting = true;
  auto membOwnerType = left.type;
  NameResolutionResult right;
  bool resolvedChain = true;
  for (auto& m : membAccess.memb) {
    _reg.pushScope(membOwnerType);
    right = resolve(m);
    if (!right.resolved)
      resolvedChain = false;

    _reg.popScope();

    membOwnerType = right.type;
  }

  _ignoreSelfInjecting = selfTemp;
  membAccess.resolvedChain = resolvedChain;

  return right;
}

NameResolutionResult
TypedASTAnalyzer::resolveArrayAccess(ArrayAccess& arrAccess)
{
  TypedNode node = { arrAccess };
  resolve(arrAccess.base);
  resolve(arrAccess.index);

  return {};
}

NameResolutionResult
TypedASTAnalyzer::resolveNameExpr(NameExpr& nameExpr)
{
  auto nameRes = _reg.resolveName(nameExpr.name);
  if (auto fnNameID = std::get_if<FnNameID>(&nameRes.nameID)) {
    return { nameRes.ownerID, {}, *fnNameID, true };
  } else if (auto varInfo = std::get_if<VarInfo>(&nameRes.nameID)) {
    return { nameRes.ownerID, varInfo->type, *varInfo, true };
  }

  return {};
}

NameResolutionResult
TypedASTAnalyzer::resolveSelfExpr()
{
  TypeID type = _reg.getCurScopeOwnerID();

  return { {}, type, type, true };
}

TypeID
TypedASTAnalyzer::inferType(TypedNode* node)
{
  if (node == nullptr)
    return {};

  auto type = std::visit(
    Overloaded{
      [&](auto&&) { return TypeID{}; },
      [&](CallExpr& callExpr) { return typeOfCallExpr(callExpr); },
      [&](BinaryExpr& binary) { return typeOfBinaryExpr(binary); },
      [&](UnaryExpr& unary) { return typeOfUnaryExpr(unary); },
      [&](MemberAccess& membAccess) { return typeOfMemberAccess(membAccess); },
      [&](ArrayAccess& arrAccess) { return typeOfArrayAccess(arrAccess); },
      [&](BoolVal& val) { return typeOfBool(val); },
      [&](NumVal& val) { return typeOfNumber(val); },
      [&](StringVal& val) { return typeOfString(val); },
      [&](ArrayVal& val) { return typeOfArray(val); },
      [&](NameExpr& nameExpr) { return typeOfName(nameExpr); },
    },
    node->node);

  return type;
}

TypeID
TypedASTAnalyzer::typeOfCallExpr(CallExpr& callExpr)
{
  // if analyzer didnt resolve call expression, then type is unknown
  if (!isValid(callExpr.fnRes.id))
    return {};

  return callExpr.fnRes.returnType;
}

TypeID
TypedASTAnalyzer::typeOfBinaryExpr(BinaryExpr& binary)
{
  if (isValid(binary.type))
    return binary.type;

  auto op = static_cast<OperatorID>(binary.op);
  auto lhs = resolve(binary.lhs);
  auto rhs = resolve(binary.rhs);
  TypeID lhsType = getType(lhs);
  TypeID rhsType = getType(rhs);
  FunctionID opRes = {};
  if (lhs.resolved) {
    auto opRes =
      _reg.resolveFunction(FnNameID{ to_underlying(op) }, { rhsType }, lhsType);
  } else if (rhs.resolved) {
    auto opRes =
      _reg.resolveFunction(FnNameID{ to_underlying(op) }, { lhsType }, rhsType);
  }

  if (isValid(opRes.val)) {
    auto fnInfo = _reg.resolveFunction(opRes);
    return fnInfo->returnType;
  }

  return {};
}

TypeID
TypedASTAnalyzer::typeOfUnaryExpr(UnaryExpr& unary)
{
  if (isValid(unary.type))
    return unary.type;

  auto op = static_cast<OperatorID>(unary.op);
  auto lhs = resolve(unary.expr);
  TypeID lhsType = getType(lhs);
  lhsType = isValid(lhsType) ? lhsType : dynamicTypeID;
  if (lhs.resolved) {
    auto opRes =
      _reg.resolveFunction(FnNameID{ to_underlying(op) }, {}, lhsType);

    if (isValid(opRes.id))
      return opRes.returnType;
  }

  return {};
}

TypeID
TypedASTAnalyzer::typeOfMemberAccess(MemberAccess& membAccess)
{
  auto baseRes = resolve(membAccess.base);
  auto ownerID = baseRes.type;

  // if member access was already statically resolved
  if (membAccess.resolvedChain)
    return inferType(membAccess.memb.back());

  return {};
}

TypeID
TypedASTAnalyzer::typeOfArrayAccess(ArrayAccess& arrAccess)
{
  // STUB
  return {};
}

TypeID
TypedASTAnalyzer::typeOfBool(BoolVal& val)
{
  return _boolType;
}

TypeID
TypedASTAnalyzer::typeOfNumber(NumVal& val)
{
  return _numberType;
}

TypeID
TypedASTAnalyzer::typeOfString(StringVal& val)
{
  return _stringType;
}

TypeID
TypedASTAnalyzer::typeOfArray(ArrayVal& val)
{
  return _arrayType;
}

TypeID
TypedASTAnalyzer::typeOfName(NameExpr& nameExpr)
{
  if (!isValid(nameExpr.fnNameRes.id) && !isValid(nameExpr.varInfo.id)) {
    auto res = resolveNameExpr(nameExpr);
    if (!res.resolved)
      return {};

    if (auto varInfo = std::get_if<VarInfo>(&res.res))
      return varInfo->type;
    else if (auto fnRes = std::get_if<FnResolution>(&res.res))
      return fnRes->returnType;
  }

  // only resolve type of variable since we cant resolve function knowing only
  // its name
  return nameExpr.varInfo.type;
}