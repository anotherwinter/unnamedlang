#include "middle/tacbuilder.h"
#include "diagnostics.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include "middle/layoutregistry.h"
#include "middle/ssastate.h"
#include <cstdio>
#include <string>

using namespace MIR;
using namespace HIR;

TACBuilder::TACBuilder(Diagnostics& diag)
  : _diag(diag)
  , _arena()

{
  _layReg = std::make_unique<LayoutRegistry>();
  _ssa = std::make_unique<SSAState>();
}

TACBuilder::~TACBuilder()
{
  for (auto& b : _blocks)
    b->~Block();
}

void
TACBuilder::print()
{
  for (auto& b : _blocks) {
    printf("\nblock %u:\n", b->id.val);
    for (auto& i : b->instructions) {
      printInstruction(i);
    }
  }
}

InstructionResult
TACBuilder::addAccessInstruction(TACValue expr, MemberAccessContext ctx)

{
  ValueID id = _ssa->makeValueID();
  OpCode op;
  std::vector<TACValue> operands;
  if (ctx.enabled) {
    operands = { ctx.base, expr };
    op = ctx.place ? OpCode::GetRef : OpCode::Get;
  } else {
    operands = { expr };
    op = ctx.place ? OpCode::Move : OpCode::Load;
  }

  if (expr.src == ValueSource::Dynamic)
    op = OpCode::DynRead;

  addInstruction({ op, operands, id });
  ValueType type = ctx.place ? ValueType::Place : ValueType::SSAValue;

  return { id, TACValue{ id, type, ValueSource::Computed } };
}

Block*
TACBuilder::allocBlock()
{
  void* raw = _arena.alloc(sizeof(Block), alignof(Block));
  if (!raw) {
    _diag.putMsg(STUB_ERR, 0, 0);
    return nullptr;
  }

  Block* b = new (raw) Block();
  _blocks.push_back(b);
  b->id = _blockID;
  ++_blockID;

  return b;
}

void
TACBuilder::buildFromAST(const TypedNode* node)
{
  if (!node)
    return;

  std::visit(
    Overloaded{
      [&](const NodeList& list) { buildNodeList(list); },
      [&](const FnDef& fnDef) { buildFnDef(fnDef); },
      [&](const ClassDef& classDef) { buildClassDef(classDef); },
      [&](const VarDecl& varDecl) { buildVarDecl(varDecl); },
      [&](const VarAssign& varAssign) { buildVarAssign(varAssign); },
      [&](const LoopWhl& loopWhl) { buildLoopWhl(loopWhl); },
      [&](const LoopFor& loopFor) { buildLoopFor(loopFor); },
      [&](const StmtIf& stmtIf) { buildIf(stmtIf); },
      [&](const StmtSwitch& stmtSwitch) { buildSwitch(stmtSwitch); },
      [&](const StmtRet& stmtRet) { buildRet(stmtRet); },
      [&](const StmtBrk& stmtBrk) { buildBrk(stmtBrk); },
      [&](const CallExpr& callExpr) { buildCallExpr(callExpr); },
      [&](const BinaryExpr& binaryExpr) { return; },
      [&](const UnaryExpr& unaryExpr) {
        // unary expression as statement allowed only if its increment or
        // decrement
        if (unaryExpr.op == ExprOp::UnaryInc ||
            unaryExpr.op == ExprOp::UnaryDec)
          buildUnary(unaryExpr);
      },
      [&](const MemberAccess& membAccess) { buildMemberAccess(membAccess); },
      [&](const ArrayAccess& arrAccess) { buildArrayAccess(arrAccess); },
      [&](const BoolVal& boolVal) { buildBool(boolVal); },
      [&](const NumVal& numVal) { buildNumber(numVal); },
      [&](const StringVal& stringVal) { buildString(stringVal); },
      [&](const ArrayVal& arrVal) { buildArray(arrVal); },
      [&](const NameExpr& nameExpr) { buildName(nameExpr); },
      [&](auto&) { return; },
    },
    node->node);
}

InstructionResult
TACBuilder::buildNodeList(const NodeList& list)
{
  for (auto& n : list.nodes)
    buildFromAST(n);

  return {};
}

InstructionResult
TACBuilder::buildFnDef(const FnDef& fnDef)
{
  Block* b = allocBlock();
  pushBranchCtx(b);
  buildFromAST(fnDef.body);
  popBranchCtx();

  _layReg->addFnLayout(fnDef.fnID, { b->id });

  return {};
}

InstructionResult
TACBuilder::buildClassDef(const ClassDef& classDef)
{
  for (auto& m : classDef.methods) {
    auto fnDef = std::get<FnDef>(m->node);
    buildFnDef(fnDef);
  }

  return {};
}

InstructionResult
TACBuilder::buildVarDecl(const VarDecl& varDecl)
{
  //_ssa->declare(varDecl.id, { varDecl.name, {} });

  // if declaration has value, make instruction
  if (varDecl.val) {
    //_ssa->assign(varDecl.id, val);
    auto lhs = varDecl.id;
    auto rhs = buildPlace(varDecl.val);

    // store rhs into lhs
    Instruction store = {
      OpCode::Store, { rhs.val, { lhs, ValueType::Place, ValueSource::Static } }
    };
    addInstruction(store);
  }

  return {};
}

InstructionResult
TACBuilder::buildVarAssign(const VarAssign& varAssign)
{
  auto lhs = buildPlace(varAssign.lhs, { {}, true });
  auto rhs = buildPlace(varAssign.rhs);

  auto rhsID = rhs.id;

  if (varAssign.op > ExprOp::Assign) {
    OpCode op;
    switch (varAssign.op) {
      case ExprOp::AssignAdd:
        op = OpCode::Add;
        break;
      case ExprOp::AssignSub:
        op = OpCode::Sub;
        break;
      case ExprOp::AssignMul:
        op = OpCode::Mul;
        break;
      case ExprOp::AssignDiv:
        op = OpCode::Div;
        break;
      case ExprOp::AssignMod:
        op = OpCode::Mod;
        break;
      case ExprOp::AssignShiftl:
        op = OpCode::Shiftl;
        break;
      case ExprOp::AssignShiftr:
        op = OpCode::Shiftr;
        break;
      case ExprOp::AssignAnd:
        op = OpCode::And;
        break;
      case ExprOp::AssignOr:
        op = OpCode::Or;
        break;
      case ExprOp::AssignXor:
        op = OpCode::Xor;
        break;
      default:
        break;
    }

    ValueID loadVal = _ssa->makeValueID();
    Instruction load = { OpCode::Load, { lhs.val }, loadVal };
    addInstruction(load);

    ValueID opVal = _ssa->makeValueID();
    Instruction binaryOp = { op, { TACValue{ loadVal }, rhs.val }, opVal };
    addInstruction(binaryOp);
    rhsID = opVal;
  }

  Instruction store = { OpCode::Store, { TACValue{ rhsID }, lhs.val } };
  addInstruction(store);

  return {};
}

InstructionResult
TACBuilder::buildLoopWhl(const LoopWhl& loopWhl)
{
  Block* cond = allocBlock();
  Block* body = allocBlock();
  Block* next = allocBlock();
  Instruction jmp = { OpCode::Jmp,
                      { { cond->id, ValueType::Block, ValueSource::Static } } };
  addInstruction(jmp);

  pushBranchCtx(cond);

  auto condVal = buildPlace(loopWhl.cond);
  TACValue bodyVal = { body->id, ValueType::Block, ValueSource::Static };
  TACValue nextVal = { next->id, ValueType::Block, ValueSource::Static };
  Instruction condJmp = { OpCode::CondJmp, { condVal.val, bodyVal, nextVal } };
  addInstruction(condJmp);

  popBranchCtx();

  pushBranchCtx(body, next);
  buildFromAST(loopWhl.body);
  addInstruction(jmp);
  popBranchCtx();

  _branches.back().current = next;

  return {};
}

InstructionResult
TACBuilder::buildLoopFor(const LoopFor& loopFor)
{
  buildFromAST(loopFor.assigns);

  Block* cond = allocBlock();
  Block* body = allocBlock();
  Block* next = allocBlock();
  Instruction jmp = { OpCode::Jmp,
                      { { cond->id, ValueType::Block, ValueSource::Static } } };
  addInstruction(jmp);

  pushBranchCtx(cond);

  auto condVal = buildPlace(loopFor.cond);
  TACValue bodyVal = { body->id, ValueType::Block, ValueSource::Static };
  TACValue nextVal = { next->id, ValueType::Block, ValueSource::Static };
  Instruction condJmp = { OpCode::CondJmp, { condVal.val, bodyVal, nextVal } };
  addInstruction(condJmp);

  popBranchCtx();

  pushBranchCtx(body, next);
  buildFromAST(loopFor.body);
  buildFromAST(loopFor.postIter);
  addInstruction(jmp);
  popBranchCtx();

  _branches.back().current = next;

  return {};
}

InstructionResult
TACBuilder::buildIf(const StmtIf& stmtIf)
{
  Block* cond = allocBlock();
  Block* body = allocBlock();
  Block* next = allocBlock();
  Instruction jmp = { OpCode::Jmp,
                      { { cond->id, ValueType::Block, ValueSource::Static } } };
  addInstruction(jmp);

  Instruction condJmp;
  Block* elseBody = next;
  TACValue bodyVal = { body->id, ValueType::Block, ValueSource::Static };
  TACValue elseVal = { elseBody->id, ValueType::Block, ValueSource::Static };
  if (stmtIf.elseBody) {
    elseBody = allocBlock();
    elseVal = { elseBody->id, ValueType::Block, ValueSource::Static };
  }

  pushBranchCtx(cond);
  auto condVal = buildPlace(stmtIf.cond);
  condJmp = { OpCode::CondJmp, { condVal.val, bodyVal, elseVal } };
  addInstruction(condJmp);

  popBranchCtx();

  pushBranchCtx(body);
  buildFromAST(stmtIf.body);

  Instruction nextJmp = {
    OpCode::Jmp, { TACValue{ next->id, ValueType::Block, ValueSource::Static } }
  };

  // add jmp to next block on the end of if block
  addInstruction(nextJmp);
  popBranchCtx();

  if (stmtIf.elseBody) {
    pushBranchCtx(elseBody);
    buildFromAST(stmtIf.elseBody);

    // add jmp to next block on the end of else block
    addInstruction(nextJmp);

    popBranchCtx();
  }

  _branches.back().current = next;

  return {};
}

InstructionResult
TACBuilder::buildSwitch(const StmtSwitch& stmtSwitch)
{
}

InstructionResult
TACBuilder::buildRet(const StmtRet& stmtRet)
{
  auto retVal = buildPlace(stmtRet.retValue);
  Instruction ret = { OpCode::Ret, { retVal.val } };
  addInstruction(ret);

  return {};
}

InstructionResult
TACBuilder::buildBrk(const StmtBrk& stmtBrk)
{
  TACValue out = { currentBranchCtx().alt->id,
                   ValueType::Block,
                   ValueSource::Static };

  // if has condition
  if (stmtBrk.cond) {
    Block* next = allocBlock();
    TACValue nextVal = { next->id, ValueType::Block, ValueSource::Static };

    auto cond = buildPlace(stmtBrk.cond);
    Instruction condJmp = { OpCode::CondJmp, { cond.val, out, nextVal } };
    addInstruction(condJmp);

    _branches.back().current = next;
  } else {
    Instruction jmp = { OpCode::Jmp, { out } };
    addInstruction(jmp);
  }

  return {};
}

InstructionResult
TACBuilder::buildPlace(const TypedNode* expr, MemberAccessContext ctx)
{
  if (!expr)
    return {};

  auto res = std::visit(
    Overloaded{
      [&](const CallExpr& callExpr) { return buildCallExpr(callExpr); },
      [&](const BinaryExpr& binary) { return buildBinary(binary); },
      [&](const UnaryExpr& unary) { return buildUnary(unary); },
      [&](const BoolVal& val) { return buildBool(val); },
      [&](const NumVal& val) { return buildNumber(val); },
      [&](const StringVal& val) { return buildString(val); },
      [&](const ArrayVal& val) { return buildArray(val); },
      [&](const MemberAccess& membAccess) {
        return buildMemberAccess(membAccess, ctx);
      },
      [&](const ArrayAccess& arrAccess) {
        return buildArrayAccess(arrAccess, ctx);
      },
      [&](const NameExpr& nameExpr) { return buildName(nameExpr, ctx); },
      [&](const SelfExpr& selfExpr) { return buildSelf(selfExpr, ctx); },
      [&](const auto& val) { return InstructionResult{}; },
    },
    expr->node);

  return res;
}

InstructionResult
TACBuilder::buildCallExpr(const CallExpr& callExpr)
{
  // TODO: differentiate between by ref and by val
  for (auto& a : callExpr.args)
    buildCallArgument(a);

  ValueID retVal;

  // if expression was resolved statically
  if (callExpr.resType == CallExpr::ResolutionType::Static) {
    retVal = _ssa->makeValueID();
    addInstruction(
      { OpCode::Call,
        { TACValue{
          callExpr.fnRes.id, ValueType::StaticFn, ValueSource::Static } },
        retVal });
  }
  // if expression to be resolved dynamically
  else if (callExpr.resType == CallExpr::ResolutionType::Dynamic) {
    auto calleeVal = buildPlace(callExpr.callee, { {}, true });
    retVal = _ssa->makeValueID();

    addInstruction({ OpCode::Call, { calleeVal.val }, retVal });
  }

  return { retVal, TACValue{ retVal } };
}

InstructionResult
TACBuilder::buildBinary(const BinaryExpr& binary)
{
  InstructionResult lhs = buildPlace(binary.lhs);
  InstructionResult rhs = buildPlace(binary.rhs);

  OpCode op;
  switch (binary.op) {
    case ExprOp::Add:
      op = OpCode::Add;
      break;
    case ExprOp::Sub:
      op = OpCode::Sub;
      break;
    case ExprOp::Mul:
      op = OpCode::Mul;
      break;
    case ExprOp::Div:
      op = OpCode::Div;
      break;
    case ExprOp::Mod:
      op = OpCode::Mod;
      break;
    case ExprOp::Shiftl:
      op = OpCode::Shiftl;
      break;
    case ExprOp::Shiftr:
      op = OpCode::Shiftr;
      break;
    case ExprOp::And:
      op = OpCode::And;
      break;
    case ExprOp::Or:
      op = OpCode::Or;
      break;
    case ExprOp::Xor:
      op = OpCode::Xor;
      break;
    case ExprOp::Lt:
      op = OpCode::Lt;
      break;
    case ExprOp::Gt:
      op = OpCode::Gt;
      break;
    case ExprOp::Leq:
      op = OpCode::Leq;
      break;
    case ExprOp::Geq:
      op = OpCode::Geq;
      break;
    case ExprOp::Eq:
      op = OpCode::Eq;
      break;
    case ExprOp::Neq:
      op = OpCode::Neq;
      break;
    case ExprOp::LogicAnd:
      op = OpCode::LogicAnd;
      break;
    case ExprOp::LogicOr:
      op = OpCode::LogicOr;
      break;
    default:
      break;
  }

  ValueID opVal = _ssa->makeValueID();
  Instruction binaryInstr = { op, { lhs.val, rhs.val }, opVal };
  addInstruction(binaryInstr);

  return { opVal, TACValue{ opVal } };
}

InstructionResult
TACBuilder::buildUnary(const UnaryExpr& unary)
{
  InstructionResult expr;
  if (unary.op == ExprOp::UnaryInc || unary.op == ExprOp::UnaryDec)
    expr = buildPlace(unary.expr, { TACValue{}, true, true });
  else
    expr = buildPlace(unary.expr);

  OpCode op;
  if (unary.op == ExprOp::UnaryNeg)
    op = OpCode::LogicNeg;
  else if (unary.op == ExprOp::UnaryArithmNeg)
    op = OpCode::ArithmNeg;
  else if (unary.op == ExprOp::UnaryInc)
    op = OpCode::Inc;
  else if (unary.op == ExprOp::UnaryDec)
    op = OpCode::Dec;

  ValueID opVal = _ssa->makeValueID();
  Instruction unaryInstr = { op, { expr.val }, opVal };
  addInstruction(unaryInstr);

  return { opVal, TACValue{ opVal } };
}

InstructionResult
TACBuilder::buildBool(const BoolVal& val)
{
  TACValue tacVal = { val.val,
                      ValueType::ImmediateBool,
                      ValueSource::Immediate };

  return { {}, tacVal };
}

InstructionResult
TACBuilder::buildNumber(const NumVal& val)
{
  TACValue tacVal = { val.val,
                      ValueType::ImmediateDouble,
                      ValueSource::Immediate };

  return { {}, tacVal };
}

InstructionResult
TACBuilder::buildString(const StringVal& val)
{
  TACValue tacVal = { _ssa->internalize(val.val),
                      ValueType::Literal,
                      ValueSource::Literal };

  return { {}, tacVal };
}

InstructionResult
TACBuilder::buildArray(const HIR::ArrayVal& val)
{
  // STUB
}

InstructionResult
TACBuilder::buildMemberAccess(const MemberAccess& membAccess,
                              MemberAccessContext ctx)
{
  auto base = buildPlace(membAccess.base, { {}, true });
  for (size_t i = 0; i < membAccess.memb.size() - 1; ++i)
    base = buildPlace(membAccess.memb[i], { base.val, true, true });

  auto val = buildPlace(membAccess.memb.back(),
                        { base.val, membAccess.reference, true });

  return val;
}

InstructionResult
TACBuilder::buildArrayAccess(const ArrayAccess& arrAccess,
                             MemberAccessContext ctx)
{
}

InstructionResult
TACBuilder::buildName(const NameExpr& name, MemberAccessContext ctx)
{
  TACValue operand;
  if (isValid(name.varInfo.id))
    operand = { name.varInfo.id, ValueType::Place, ValueSource::Static };
  else
    operand = { _ssa->internalize(name.name),
                ValueType::Place,
                ValueSource::Dynamic };

  return addAccessInstruction(operand, ctx);
}

InstructionResult
TACBuilder::buildSelf(const SelfExpr& self, MemberAccessContext ctx)
{
  TACValue operand = { {}, ValueType::Self, ValueSource::Self };

  return addAccessInstruction(operand, ctx);
}

InstructionResult
TACBuilder::buildCallArgument(const HIR::TypedNode* arg)
{
  auto res = buildPlace(arg);
  Instruction param = { OpCode::Param, { res.val } };
  addInstruction(param);

  return {};
}

void
TACBuilder::resolveSSA(Block* begin)
{
}

void
TACBuilder::printInstruction(Instruction& instr)
{
  std::string str =
    isValid(instr.res) ? "@" + std::to_string(instr.res.val) + " = " : "";
  str += getOpCodeStr(instr.op);
  if (!instr.operands.empty()) {
    str += " " + tacValueToStr(instr.operands.front());

    for (size_t i = 1; i < instr.operands.size(); ++i)
      str += ", " + tacValueToStr(instr.operands[i]);
  }

  printf("%s\n", str.c_str());
}

const char*
TACBuilder::getOpCodeStr(OpCode op)
{
  switch (op) {
    case OpCode::Move:
      return "move";
    case OpCode::Load:
      return "load";
    case OpCode::DynRead:
      return "dynread";
    case OpCode::Store:
      return "store";
    case OpCode::Jmp:
      return "jmp";
    case OpCode::Call:
      return "call";
    case OpCode::Param:
      return "param";
    case OpCode::CondJmp:
      return "condjmp";
    case OpCode::Add:
      return "add";
    case OpCode::Sub:
      return "sub";
    case OpCode::Mul:
      return "mul";
    case OpCode::Div:
      return "div";
    case OpCode::Mod:
      return "mod";
    case OpCode::Shiftl:
      return "shiftl";
    case OpCode::Shiftr:
      return "shiftr";
    case OpCode::And:
      return "and";
    case OpCode::Or:
      return "or";
    case OpCode::Xor:
      return "xor";
    case OpCode::Lt:
      return "lt";
    case OpCode::Gt:
      return "gt";
    case OpCode::Leq:
      return "leq";
    case OpCode::Geq:
      return "geq";
    case OpCode::Eq:
      return "eq";
    case OpCode::Neq:
      return "neq";
    case OpCode::LogicAnd:
      return "logicand";
    case OpCode::LogicOr:
      return "logicor";
    case OpCode::LogicNeg:
      return "lneg";
    case OpCode::ArithmNeg:
      return "aneg";
    case OpCode::Inc:
      return "inc";
    case OpCode::Dec:
      return "dec";
    case OpCode::Get:
      return "get";
    case OpCode::GetRef:
      return "getref";
    case OpCode::Ret:
      return "ret";
    default:
      return "unknown";
  }
}

std::string
TACBuilder::tacValueToStr(TACValue val)
{
  if (val.type == ValueType::Self)
    return "self";

  std::string str = "";
  if (val.src == ValueSource::Dynamic)
    str += "dyn ";

  if (auto valID = std::get_if<ValueID>(&val.id)) {
    str += "@" + std::to_string(valID->val);
  } else if (auto varID = std::get_if<VarID>(&val.id)) {
    str += "var" + std::to_string(varID->val);
  } else if (auto fnID = std::get_if<FunctionID>(&val.id)) {
    str += "fn" + std::to_string(fnID->val);
  } else if (auto blockID = std::get_if<BlockID>(&val.id)) {
    str += "blk" + std::to_string(blockID->val);
  } else if (auto literalID = std::get_if<LiteralID>(&val.id)) {
    str += "ltrl" + std::to_string(literalID->val);
  } else if (auto boolVal = std::get_if<bool>(&val.id)) {
    str += boolVal ? "true" : "false";
  } else if (auto numVal = std::get_if<double>(&val.id)) {
    str += std::to_string(*numVal);
  }

  return str;
}