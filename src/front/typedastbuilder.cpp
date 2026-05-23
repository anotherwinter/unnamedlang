#include "front/typedastbuilder.h"
#include "errorcode_bases.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include "shared.h"
using namespace HIR;

TypedASTBuilder::TypedASTBuilder(Diagnostics& diag,
                                 SymbolRegistry& reg,
                                 NodeAllocator& alloc)
  : _diag(diag)
  , _reg(reg)
  , _alloc(alloc)
{
  predeclarePrebuilts();
}

TypedNode*
TypedASTBuilder::buildFromAST(const ASTNode* node)
{
  if (!node)
    return nullptr;

  switch (node->type) {
    case ASTNodeType::NODE_LIST:
      return buildNodeList(node);
    case ASTNodeType::NODE_FN_DEF:
      return buildFnDef(node);
    case ASTNodeType::NODE_FN_CALL:
      return buildCallExpr(node);
    case ASTNodeType::NODE_CLASS_DEF:
      return buildClassDef(node);
    case ASTNodeType::NODE_ENUM_DEF:
      return buildEnumDef(node);
    case ASTNodeType::NODE_VAR_DECL:
      return buildVarDecl(node);
    case ASTNodeType::NODE_VAR_ASSIGN:
      return buildVarAssign(node);
    case ASTNodeType::NODE_WHL:
      return buildWhl(node);
    case ASTNodeType::NODE_FOR:
      return buildFor(node);
    case ASTNodeType::NODE_IF:
      return buildIf(node);
    case ASTNodeType::NODE_SWITCH:
      return buildSwitch(node);
    case ASTNodeType::NODE_RET:
      return buildRet(node);
    case ASTNodeType::NODE_BRK:
      return buildBrk(node);
    case ASTNodeType::NODE_BINARYOP:
      return buildBinaryExpr(node);
    case ASTNodeType::NODE_UNARYPRE:
      return buildUnaryExpr(node, UnaryExprPrec::Prefix);
    case ASTNodeType::NODE_UNARYPOST:
      return buildUnaryExpr(node, UnaryExprPrec::Postfix);
    case ASTNodeType::NODE_MEMBER_ACCESS:
      return buildMemberAccess(node);
    case ASTNodeType::NODE_ARR_ACCESS:
      return buildArrayAccess(node);
    case ASTNodeType::NODE_BOOL:
      return buildBool(node);
    case ASTNodeType::NODE_NUMBER:
      return buildNumber(node);
    case ASTNodeType::NODE_STRING:
      return buildString(node);
    case ASTNodeType::NODE_ARRAY:
      return buildArray(node);
    case ASTNodeType::NODE_NAME:
      return buildName(node);
    default:
      _diag.putMsg(STUB_ERR, node->line, node->col);
      return _alloc.allocTypedNode();
  }
}

void
TypedASTBuilder::predeclarePrebuilts()
{
  std::ignore = _reg.beginDeclareClass("dynamic", false, true);
  std::ignore = _reg.beginDeclareClass("Bool", true, true);
  std::ignore = _reg.beginDeclareClass("Number", true, true);
  std::ignore = _reg.beginDeclareClass("String", true, true);
  std::ignore = _reg.beginDeclareClass("Array", false, true);
}

void
TypedASTBuilder::predeclare(const ASTNode* node)
{
  if (!node)
    return;

  if (node->type == ASTNodeType::NODE_LIST) {
    ASTNodeLL* it = node->data.nodeList.list;
    while (it) {
      predeclare(it->node);
      it = it->next;
    }
  }

  else if (node->type == NODE_CLASS_DEF) {
    const char* className = node->data.classDef.name;
    TypeID id = _reg.beginDeclareClass(className);
  }
}

TypedNode*
TypedASTBuilder::buildNodeList(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<NodeList>(node);
  auto& nodes = std::get<NodeList>(n->node);
  const ASTNodeLL* it = nodeList(node);
  while (it) {
    nodes.nodes.push_back(buildFromAST(it->node));
    it = it->next;
  }

  return n;
}

TypedNode*
TypedASTBuilder::buildFnDef(const ASTNode* node, TypeID ownerID)
{
  std::vector<std::string> paramNames;
  std::vector<TypeID> paramTypes;
  FnDeclKey fnID = evalFnDecl(node, paramNames, paramTypes, ownerID);

  TypedNode* n = _alloc.allocTypedNode<FnDef>(node);
  auto& def = std::get<FnDef>(n->node);
  def.fnID = fnID.id;
  def.ownerID = ownerID;
  def.nameID = fnID.nameID;
  def.paramTypes = paramTypes;
  def.body = buildFromAST(node->data.fnDef.code);

  return n;
}

TypedNode*
TypedASTBuilder::buildCallExpr(const ASTNode* node)
{
  ASTNode* callee = node->data.fnCall.callee;

  TypedNode* n = _alloc.allocTypedNode<CallExpr>(node);
  auto& call = std::get<CallExpr>(n->node);
  call.callee = buildFromAST(callee);

  ASTNodeLL* it = nodeList(node->data.fnCall.arguments);
  while (it) {
    TypedNode* arg = buildFromAST(it->node);
    call.args.push_back(arg);

    it = it->next;
  }

  return n;
}

TypedNode*
TypedASTBuilder::buildClassDef(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<ClassDef>(node);
  auto& def = std::get<ClassDef>(n->node);

  const char* className = node->data.classDef.name;
  def.classID = _reg.resolveClass(className);
  if (!isValid(def.classID)) {
    _diag.putMsg(STUB_ERR, node->line, node->col);
    return n;
  }

  std::vector<MethodDeclInfo> methods;
  std::vector<FieldDeclInfo> fields;
  ASTNodeLL* it = nodeList(node->data.classDef.members);
  VarID fieldIdx = { 0 };
  while (it) {
    ASTNode* node = it->node;
    Modifier mod = {};
    // if defining field
    if (node->type == NODE_CLASSFIELD_DEF) {
      mod = node->data.classFieldDef.modifier;
      fields.push_back(
        { node->data.classFieldDef.name, VarInfo{ fieldIdx, TypeID{}, mod } });

      def.fields.push_back(node->data.classFieldDef.name);
    }
    // if defining method
    else if (node->type == NODE_CLASSMETHOD_DEF) {
      mod = node->data.classMethodDef.modifier;
      ASTNode* fnDef = node->data.classMethodDef.function;
      auto methodNode = buildFnDef(fnDef, def.classID);
      auto methodDef = std::get<FnDef>(methodNode->node);
      const char* methodName = fnDef->data.fnDef.name;
      methods.push_back({ methodDef.nameID, methodDef.fnID });

      def.methods.push_back(methodNode);
    }

    it = it->next;
  }

  def.classID = _reg.finishDeclareClass(def.classID, methods, fields);

  return n;
}

TypedNode*
TypedASTBuilder::buildEnumDef(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<EnumDef>(node);
  auto& def = std::get<EnumDef>(n->node);

  ASTNodeLL* it = nodeList(node->data.enumDef.elements);
  while (it) {
    ASTNode* elem = it->node;
    ASTNode* rhs = elem->data.enumElement.expr;
    EnumElement el = { it->node->data.stringValue, buildFromAST(rhs) };

    def.vals.push_back(el);
    it = it->next;
  }

  return n;
}

TypedNode*
TypedASTBuilder::buildVarDecl(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<VarDecl>(node);
  auto& decl = std::get<VarDecl>(n->node);

  const char* name = node->data.varDecl.name;
  decl.name = name;
  decl.mod = node->data.varDecl.modifier;
  decl.val = buildFromAST(node->data.varDecl.expr);

  return n;
}

TypedNode*
TypedASTBuilder::buildVarAssign(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<VarAssign>(node);
  auto& assign = std::get<VarAssign>(n->node);

  assign.op = static_cast<ExprOp>(node->data.varAssign.op);
  assign.lhs = buildFromAST(node->data.varAssign.lhs);
  assign.rhs = buildFromAST(node->data.varAssign.rhs);

  return n;
}

TypedNode*
TypedASTBuilder::buildWhl(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<LoopWhl>(node);
  auto& loopWhl = std::get<LoopWhl>(n->node);

  loopWhl.cond = buildFromAST(node->data.loopWhl.condition);
  loopWhl.body = buildFromAST(node->data.loopWhl.code);

  return n;
}

TypedNode*
TypedASTBuilder::buildFor(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<LoopFor>(node);
  auto& loopFor = std::get<LoopFor>(n->node);

  loopFor.assigns = buildNodeList(node->data.loopFor.assigns);
  loopFor.cond = buildNodeList(node->data.loopFor.condition);
  loopFor.postIter = buildNodeList(node->data.loopFor.postIterationAssigns);
  loopFor.body = buildFromAST(node->data.loopFor.code);

  return n;
}

TypedNode*
TypedASTBuilder::buildIf(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<StmtIf>(node);
  auto& stmtIf = std::get<StmtIf>(n->node);

  stmtIf.cond = buildFromAST(node->data.ifBlock.condition);
  stmtIf.body = buildFromAST(node->data.ifBlock.code);
  stmtIf.elseBody = buildFromAST(node->data.ifBlock.elseBlock);

  return n;
}

TypedNode*
TypedASTBuilder::buildSwitch(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<StmtSwitch>(node);
  auto& stmtSwitch = std::get<StmtSwitch>(n->node);

  stmtSwitch.expr = buildFromAST(node->data.switchStmt.expr);
  ASTNodeLL* casesIt = nodeList(node->data.switchStmt.cases);
  while (casesIt) {
    auto caseNode = casesIt->node->data.switchCase;
    stmtSwitch.cases.push_back(
      { buildFromAST(caseNode.expr), buildFromAST(caseNode.code) });

    casesIt = casesIt->next;
  }

  return n;
}

TypedNode*
TypedASTBuilder::buildRet(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<StmtRet>(node);
  auto& stmtRet = std::get<StmtRet>(n->node);

  stmtRet.retValue = buildFromAST(node->data.ret.retValue);

  return n;
}

TypedNode*
TypedASTBuilder::buildBrk(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<StmtBrk>(node);
  auto& stmtBrk = std::get<StmtBrk>(n->node);

  stmtBrk.cond = buildFromAST(node->data.brk.condition);

  return n;
}

TypedNode*
TypedASTBuilder::buildBinaryExpr(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<BinaryExpr>(node);
  auto& binaryExpr = std::get<BinaryExpr>(n->node);

  auto nodeBinary = node->data.binaryOp;
  binaryExpr.op = static_cast<ExprOp>(nodeBinary.op);
  binaryExpr.lhs = buildFromAST(nodeBinary.lhs);
  binaryExpr.rhs = buildFromAST(nodeBinary.rhs);

  return n;
}

TypedNode*
TypedASTBuilder::buildUnaryExpr(const ASTNode* node, UnaryExprPrec prec)
{
  TypedNode* n = _alloc.allocTypedNode<UnaryExpr>(node);
  auto& unaryExpr = std::get<UnaryExpr>(n->node);

  auto nodeUnary = node->data.unaryOp;

  unaryExpr.op = static_cast<ExprOp>(nodeUnary.op);
  unaryExpr.prec = prec;
  unaryExpr.expr = buildFromAST(nodeUnary.expr);

  return n;
}

TypedNode*
TypedASTBuilder::buildMemberAccess(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<MemberAccess>(node);
  auto& membAccess = std::get<MemberAccess>(n->node);

  auto left = node;
  std::vector<TypedNode*> nodes;
  while (left->type == NODE_MEMBER_ACCESS) {
    nodes.push_back(buildFromAST(left->data.memberAccess.right));
    left = left->data.memberAccess.left;
  }

  membAccess.base = buildFromAST(left);
  for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    membAccess.memb.push_back(*it);

  return n;
}

TypedNode*
TypedASTBuilder::buildArrayAccess(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<ArrayAccess>(node);
  auto& arrAccess = std::get<ArrayAccess>(n->node);

  arrAccess.base = buildFromAST(node->data.arrayAccess.left);
  arrAccess.index = buildFromAST(node->data.arrayAccess.subscript);

  return n;
}

TypedNode*
TypedASTBuilder::buildBool(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<BoolVal>(node);
  auto& boolVal = std::get<BoolVal>(n->node);

  boolVal.val = node->data.boolValue;
  return n;
}

TypedNode*
TypedASTBuilder::buildNumber(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<NumVal>(node);
  auto& numVal = std::get<NumVal>(n->node);

  numVal.val = node->data.numberValue;
  return n;
}

TypedNode*
TypedASTBuilder::buildString(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<StringVal>(node);
  auto& stringVal = std::get<StringVal>(n->node);

  stringVal.val = node->data.stringValue;
  return n;
}

TypedNode*
TypedASTBuilder::buildArray(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<ArrayVal>(node);
  auto& arrVal = std::get<ArrayVal>(n->node);

  auto arrElemsIt = node->data.array.elements->data.nodeList.list;
  while (arrElemsIt) {
    arrVal.list.push_back(buildFromAST(arrElemsIt->node));

    arrElemsIt = arrElemsIt->next;
  }

  return n;
}

TypedNode*
TypedASTBuilder::buildName(const ASTNode* node)
{
  TypedNode* n = _alloc.allocTypedNode<NameExpr>(node);
  auto& nameExpr = std::get<NameExpr>(n->node);

  nameExpr.name = node->data.stringValue;
  return n;
}

FnDeclKey
TypedASTBuilder::evalFnDecl(const ASTNode* node,
                            std::vector<std::string>& paramNames,
                            std::vector<TypeID>& paramTypes,
                            TypeID ownerID)
{
  if (!node)
    return {};

  const char* fnName = node->data.fnDef.name;

  // fill in parameters
  ASTNodeLL* paramsIt = node->data.fnDef.params->data.nodeList.list;
  while (paramsIt) {
    const char* paramName = paramsIt->node->data.paramInfo.name;
    const char* paramType =
      paramsIt->node->data.paramInfo.type->data.stringValue;

    paramNames.push_back(paramName);
    paramTypes.push_back(_reg.resolveClass(paramType));

    paramsIt = paramsIt->next;
  }

  auto declKey = _reg.declareFunction(fnName, paramNames, paramTypes, ownerID);

  return declKey;
}