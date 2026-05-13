#include "typedastanalyzer.h"
#include "..//diagnostics.h"
#include "shared.h"
#include "symbolregistry.h"
#include "typedast.h"
#include <variant>

using namespace HIR;

TypedASTAnalyzer::TypedASTAnalyzer(Diagnostics& diag, SymbolRegistry& reg)
  : _diag(diag)
  , _reg(reg)
{
  initPrebuilts();
}

void
TypedASTAnalyzer::initPrebuilts()
{
  _boolType = { _reg.beginDeclareClass("dynamic") };
  _boolType = { _reg.beginDeclareClass("Bool") };
  _numberType = { _reg.beginDeclareClass("Number") };
  _stringType = { _reg.beginDeclareClass("String") };
  _arrayType = { _reg.beginDeclareClass("Array") };
}

void
TypedASTAnalyzer::analyzeTypedNode(const TypedNode* node)
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
TypedASTAnalyzer::analyzeFnDef(const TypedNode* node)
{
  auto fnDef = std::get<FnDef>(node->node);
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

  analyzeTypedNode(fnDef.body);
}

void
TypedASTAnalyzer::analyzeCallExpr(const TypedNode* node)
{
  auto callExpr = std::get<CallExpr>(node->node);
  analyzeTypedNode(callExpr.callee);
  for (auto& a : callExpr.args)
    analyzeTypedNode(a);

  auto callRes = resolveCallExpr(callExpr);
}

void
TypedASTAnalyzer::analyzeClassDef(const TypedNode* node)
{
  auto classDef = std::get<ClassDef>(node->node);
  std::set<const char*> duplicates;
  for (auto& f : classDef.fields) {
    if (duplicates.find(f) == duplicates.end())
      duplicates.emplace(f);
    else
      _diag.putMsg(STUB_ERR, node->line, node->col);
  }

  for (auto& m : classDef.methods) {
    analyzeFnDef(m);
  }
}

void
TypedASTAnalyzer::analyzeEnumDef(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeVarDecl(const TypedNode* node)
{
  auto varDecl = std::get<VarDecl>(node->node);
  auto curScopeRes = _reg.resolveVariableCurScope(varDecl.name);
  // if variable is already declared in current scope
  if (isValid(curScopeRes))
    _diag.putMsg(STUB_ERR, node->line, node->col);
  else
    VarID var = _reg.declareVariable(varDecl.name, varDecl.type, varDecl.mod);

  analyzeTypedNode(varDecl.val);
  auto valType = inferType(varDecl.val);
  // if declaration has static type but expression value is not the same type
  // TODO: add type inheritance/type compatibility
  if (isValid(varDecl.type) && varDecl.type != valType)
    _diag.putMsg(STUB_ERR, varDecl.val->line, varDecl.val->col);
}

void
TypedASTAnalyzer::analyzeVarAssign(const TypedNode* node)
{
  auto varAssign = std::get<VarAssign>(node->node);
  analyzeTypedNode(varAssign.lhs);
  analyzeTypedNode(varAssign.rhs);

  auto lhs = resolve(varAssign.lhs);

  // dont try to analyze further if lhs cannot be resolved
  if (!lhs.resolved) {
    _diag.putMsg(STUB_ERR, varAssign.lhs->line, varAssign.lhs->col);
    return;
  } else if (!std::holds_alternative<VarResolution>(lhs.res)) {
    _diag.putMsg(STUB_ERR, varAssign.lhs->line, varAssign.lhs->col);
    return;
  }

  auto varRes = std::get<VarResolution>(lhs.res);
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
TypedASTAnalyzer::analyzeWhl(const TypedNode* node)
{
  auto loopWhl = std::get<LoopWhl>(node->node);
  analyzeTypedNode(loopWhl.cond);
  analyzeTypedNode(loopWhl.body);
}

void
TypedASTAnalyzer::analyzeFor(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeIf(const TypedNode* node)
{
  auto stmtIf = std::get<StmtIf>(node->node);
  analyzeTypedNode(stmtIf.cond);
  analyzeTypedNode(stmtIf.body);
  analyzeTypedNode(stmtIf.elseBody);
}

void
TypedASTAnalyzer::analyzeSwitch(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeRet(const TypedNode* node)
{
  auto stmtRet = std::get<StmtRet>(node->node);
  analyzeTypedNode(stmtRet.retValue);
}

void
TypedASTAnalyzer::analyzeBrk(const TypedNode* node)
{
  auto stmtBrk = std::get<StmtBrk>(node->node);
  analyzeTypedNode(stmtBrk.cond);
}

void
TypedASTAnalyzer::analyzeBinaryExpr(const TypedNode* node)
{
  auto binary = std::get<BinaryExpr>(node->node);
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
      _diag.putMsg(STUB_ERR, node->line, node->col);
      return;
    }

    binary.type = fnRes.returnType;
  }
}

void
TypedASTAnalyzer::analyzeUnaryExpr(const TypedNode* node)
{
  auto unary = std::get<UnaryExpr>(node->node);
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
      _diag.putMsg(STUB_ERR, node->line, node->col);
      return;
    }

    unary.type = fnRes.returnType;
  }
}

void
TypedASTAnalyzer::analyzeMemberAccess(const TypedNode* node)
{
  auto membAccess = std::get<MemberAccess>(node->node);
  auto res = resolveMemberAccess(membAccess);
  // error if ownerid is valid (means base is resolved statically), but member
  // cannot be resolved statically
  if (isValid(res.ownerID) && !res.resolved)
    _diag.putMsg(STUB_ERR, node->line, node->col);
}

void
TypedASTAnalyzer::analyzeArrayAccess(const TypedNode* node)
{
  // STUB: add bounds check (?) and check if index is integer
}

void
TypedASTAnalyzer::analyzeBool(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeNumber(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeString(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeArray(const TypedNode* node)
{
  // STUB
}

void
TypedASTAnalyzer::analyzeName(const TypedNode* node)
{
  auto name = std::get<NameExpr>(node->node);
  auto nameRes = resolveNameExpr(name);
  if (!nameRes.resolved)
    _diag.putMsg(STUB_ERR, node->line, node->col);
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
    },
    node->node);

  return res;
}

NameResolutionResult
TypedASTAnalyzer::resolveCallExpr(CallExpr& callExpr)
{
  // if already resolved
  if (callExpr.resType == CallExpr::ResolutionType::Static)
    return { {}, callExpr.fnRes.returnType, callExpr.fnRes.id, true };

  auto calleeRes = resolve(callExpr.callee);
  auto fnNameID = std::get_if<FnNameID>(&calleeRes.res);

  // this will be true if failed to resolve statically
  if (!fnNameID)
    return {};

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

  return { calleeRes.ownerID, fnRes.returnType, fnRes.id, true };
}

NameResolutionResult
TypedASTAnalyzer::resolveMemberAccess(MemberAccess& membAccess)
{
  // if base already resolved, dont try to resolve
  if (!std::holds_alternative<TypedNode*>(membAccess.base)) {
    _diag.putMsg(STUB_ERR, 0, 0);
    return {};
  }

  auto left = resolve(std::get<TypedNode*>(membAccess.base));
  if (!left.resolved || !isValid(left.type))
    return {};

  _reg.pushScope(left.type);
  NameResolutionResult rightRes =
    std::visit(Overloaded{
                 [&](TypedNode* node) { return resolve(node); },
                 [&](auto&&) {
                   _diag.putMsg(STUB_ERR, 0, 0);
                   return NameResolutionResult{};
                 },
               },
               membAccess.memb);
  _reg.popScope();

  if (auto varRes = std::get_if<VarResolution>(&rightRes.res))
    membAccess.memb = varRes->varInfo.id;
  else if (auto fnIDRes = std::get_if<FunctionID>(&rightRes.res))
    membAccess.memb = *fnIDRes;

  rightRes.ownerID = left.type;

  return rightRes;
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
  // 1. try to resolve in frames
  auto varRes = _reg.resolveVariable(nameExpr.name);
  if (isValid(varRes.varInfo.id)) {
    nameExpr.varRes = varRes;
    return { varRes.ownerID, varRes.varInfo.type, varRes, true };
  }

  // 2. try to resolve function name
  auto fnNameRes = _reg.resolveFunctionName(nameExpr.name);
  if (isValid(fnNameRes.id)) {
    nameExpr.fnNameRes = fnNameRes;
    return { fnNameRes.ownerID, {}, fnNameRes.id, true };
  }

  return {};
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
  TypeID ownerID;
  if (std::holds_alternative<TypedNode*>(membAccess.base)) {
    auto baseRes = resolve(std::get<TypedNode*>(membAccess.base));
    if (!baseRes.resolved)
      return {};

    ownerID = baseRes.ownerID;
  } else if (auto varInfoRes = std::get_if<VarResolution>(&membAccess.base))
    ownerID = varInfoRes->ownerID;
  else if (auto fnIDRes = std::get_if<FunctionID>(&membAccess.base))
    ownerID = _reg.resolveFunction(*fnIDRes)->returnType;

  // if member access was already statically resolved
  if (auto varIDRes = std::get_if<VarID>(&membAccess.memb))
    return _reg.resolveField(*varIDRes, ownerID)->type;
  else if (auto fnIDRes = std::get_if<FunctionID>(&membAccess.base))
    return _reg.resolveFunction(*fnIDRes)->returnType;

  auto membRes = resolve(std::get<TypedNode*>(membAccess.memb));
  if (!membRes.resolved)
    return {};

  if (auto varRes = std::get_if<VarResolution>(&membRes.res))
    return varRes->varInfo.type;
  else if (auto fnIDRes = std::get_if<FunctionID>(&membRes.res))
    return _reg.resolveFunction(*fnIDRes)->returnType;

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
  if (!isValid(nameExpr.fnNameRes.id) && !isValid(nameExpr.varRes.varInfo.id)) {
    auto res = resolveNameExpr(nameExpr);
    if (!res.resolved)
      return {};

    if (auto varRes = std::get_if<VarResolution>(&res.res))
      return varRes->varInfo.type;
    else if (auto fnIDRes = std::get_if<FunctionID>(&res.res))
      return _reg.resolveFunction(*fnIDRes)->returnType;
  }

  // only resolve type of variable since we cant resolve function knowing only
  // its name
  return nameExpr.varRes.varInfo.type;
}