#include "middle/tacbuilder.h"
#include "diagnostics.h"
#include "front/symbolregistry.h"
#include "middle/layoutregistry.h"
#include "middle/ssastate.h"
#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace MIR;
using namespace HIR;

TACBuilder::TACBuilder(Diagnostics& diag, SymbolRegistry& symReg)
  : _diag(diag)
  , _symReg(symReg)
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

Block*
TACBuilder::build(HIR::TypedTree<HIR::Analyzed> root)
{
  // TODO: fix memleaks related to begin
  Block* begin = allocBlock();
  pushBranchCtx(begin);
  buildFromAST(root.root);
  resolveSSA();
  popBranchCtx();

  return begin;
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

void
TACBuilder::pushBranchCtx(Block* current, Block* alt)
{
  _branches.push_back({ current, alt });
  _ssa->pushScope();
}

void
TACBuilder::popBranchCtx()
{
  _ssa->popScope();
  _branches.pop_back();
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

  std::visit(Overloaded{
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
                 // unary expression as statement allowed only if its increment
                 // or decrement
                 if (unaryExpr.op == ExprOp::UnaryInc ||
                     unaryExpr.op == ExprOp::UnaryDec)
                   buildUnary(unaryExpr);
               },
               [&](const MemberAccess& membAccess) { return; },
               [&](const ArrayAccess& arrAccess) { return; },
               [&](const BoolVal& boolVal) { return; },
               [&](const NumVal& numVal) { return; },
               [&](const StringVal& stringVal) { return; },
               [&](const ArrayVal& arrVal) { return; },
               [&](const NameExpr& nameExpr) { return; },
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

  // inject hidden params if building for method
  FunctionInfo* fnInfo = _symReg.resolveFunction(fnDef.fnID);
  for (auto& p : fnInfo->params) {
    TACValue val = { p.id, ValueType::Place, ValueSource::Dynamic };
    injectParameter(p.id, p.name.c_str(), val);
  }

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
  // if declaration has value, make instruction
  if (varDecl.val) {
    auto rhs = buildPlace(varDecl.val);

    // if lhs is ssa identity, then update ssa state for it
    auto idState = _ssa->getIdentityState(varDecl.id, varDecl.name);
    idState.ver = _ssa->assign(idState.id, rhs.val);
    TACValue lhs = { idState, ValueType::SSAIdentity, ValueSource::Computed };

    ValueID loadVal = _ssa->makeValueID();
    Instruction load = { OpCode::Load, { rhs.val }, loadVal };
    addInstruction(load);

    // store rhs into lhs
    Instruction store = { OpCode::Store, { TACValue{ loadVal }, lhs } };
    addInstruction(store);
  }

  return {};
}

InstructionResult
TACBuilder::buildVarAssign(const VarAssign& varAssign)
{
  auto lhs = buildPlace(varAssign.lhs, { TACValue{}, true, false, true });
  auto rhs = buildPlace(varAssign.rhs);

  TACValue rhsOperand = rhs.val;

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
    rhsOperand = { opVal };
  }

  // if lhs is ssa identity, then update ssa state for it
  if (lhs.val.type == ValueType::SSAIdentity) {
    auto& ssaId = std::get<SSAIdentityState>(lhs.val.id);
    ssaId.ver = _ssa->assign(ssaId.id, rhsOperand);
  }

  Instruction store = { OpCode::Store, { rhsOperand, lhs.val } };
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

    // swap next and else so it looks more consistent
    Block* temp = next;
    next = elseBody;
    elseBody = temp;

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

  // if accessing local name, try to resolve into SSA identity
  if (!ctx.enabled && isValid(name.varInfo.id)) {
    auto idState = _ssa->getIdentityState(name.varInfo.id);
    operand = { idState, ValueType::SSAIdentity, ValueSource::Computed };
    if (ctx.ssaImmediate)
      return { {}, operand };

  } else if (isValid(name.varInfo.id))
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
  auto idState = _ssa->getIdentityState(SSAIdentityID{ 0 });
  TACValue ssaVal = { idState, ValueType::SSAIdentity, ValueSource::Computed };
  if (ctx.place)
    return { {}, ssaVal };

  return addAccessInstruction(ssaVal, ctx);
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
TACBuilder::injectParameter(VarID id, const char* name, TACValue val)
{
  auto res = addAccessInstruction(val, {});

  auto idState = _ssa->getIdentityState(id, name);
  _ssa->assign(idState.id, res.val);

  TACValue ssaVal = { idState, ValueType::SSAIdentity, ValueSource::Computed };

  Instruction store = { OpCode::Store, { res.val, ssaVal } };
  addInstruction(store);
}

void
TACBuilder::resolveSSA()
{
  // pass1 - resolve incoming/outcoming versions
  resolveIdentities();

  // pass2 - compute phi functions
  std::vector<Block*> worklist;
  worklist.push_back(*_blocks.begin());
  size_t idx = 0;
  while (idx < worklist.size()) {
    Block* current = worklist[idx];
    ++idx;
    auto ctxIt = _ssaBlocksMap.find(current->id.val);
    auto& ctx = ctxIt->second;
    if (ctx.inVersions.empty() && ctx.outVersions.empty())
      continue;

    for (auto& pair : ctx.inVersions)
      ensurePhi(ctx, pair.first, pair.second);

    propagateVersions(ctx, worklist);
  }

  // pass3 - insert phi instructions into beginning of blocks
  // use vector of tuples for later inserting functions so when renaming uses
  // instruction pointers wont get invalidated
  std::vector<std::pair<Block*, PhiFunction*>> phiToInsert;
  for (auto& pair : _ssaBlocksMap) {
    auto& b = _blocks.at(pair.first);
    auto& ctx = pair.second;
    for (auto& usedPair : ctx.usedVersions) {
      auto phiIt = ctx.phiFunctions.find(usedPair.first);
      bool isPhi = phiIt != ctx.phiFunctions.end();

      // change version used in already existing load instructions
      auto outerIt = ctx.usedIdentities.find(usedPair.first);
      TACValue operand =
        isPhi ? TACValue{ phiIt->second->res }
              : TACValue{ SSAIdentityState{ usedPair.first, usedPair.second },
                          ValueType::SSAIdentity,
                          ValueSource::Computed };

      if (outerIt != ctx.usedIdentities.end()) {
        auto& vec = outerIt->second;
        for (auto& instr : outerIt->second)
          instr->operands.at(0) = operand;
      }

      if (isPhi)
        phiToInsert.emplace_back(b, phiIt->second);
    }
  }

  // insert phi after modifying all existing instructions
  for (auto& pair : phiToInsert)
    insertPhi(pair.first, *pair.second);
}

void
TACBuilder::resolveIdentities()
{
  // TODO: optimize this
  std::unordered_set<BlockID::__BlockID> visited;
  std::vector<Block*> worklist;

  worklist.push_back(*_blocks.begin());
  auto idx = 0;
  while (idx < worklist.size()) {
    Block* current = worklist[idx];
    ++idx;
    if (visited.find(current->id.val) != visited.end())
      continue;

    visited.emplace(current->id.val);
    auto ctxIt = _ssaBlocksMap.try_emplace(current->id.val);
    auto& ctx = ctxIt.first->second;

    for (auto& instr : current->instructions) {
      if (instr.op == OpCode::Store &&
          instr.operands.at(1).type == ValueType::SSAIdentity) {
        auto operand = std::get<SSAIdentityState>(instr.operands.at(1).id);
        auto versionsIt = ctx.outVersions.try_emplace(operand.id);
        versionsIt.first->second = operand.ver;

        ctx.usedDefinitions.emplace(operand.id);
      } else if (instr.op == OpCode::Jmp) {
        auto blockID = std::get<BlockID>(instr.operands.at(0).id);
        Block* b = _blocks.at(blockID.val);
        ctx.outBlocks.push_back(b);
        worklist.push_back(b);
      } else if (instr.op == OpCode::CondJmp) {
        auto ifBlockID = std::get<BlockID>(instr.operands.at(1).id);
        auto elseBlockID = std::get<BlockID>(instr.operands.at(2).id);
        Block* ifBlock = _blocks.at(ifBlockID.val);
        Block* elseBlock = _blocks.at(elseBlockID.val);
        ctx.outBlocks.push_back(ifBlock);
        ctx.outBlocks.push_back(elseBlock);

        worklist.push_back(ifBlock);
        worklist.push_back(elseBlock);
      } else if (instr.op == OpCode::Load &&
                 instr.operands.at(0).type == ValueType::SSAIdentity) {
        auto operand = std::get<SSAIdentityState>(instr.operands.at(0).id);
        auto it = ctx.outVersions.find(operand.id);
        // if this load instruction comes after existing definition in block,
        // ignore it
        if (it == ctx.outVersions.end()) {
          auto outer = ctx.usedIdentities.try_emplace(operand.id);
          outer.first->second.push_back(&instr);
        }
      }
      // ignore block's remaining instructions if encountered return
      else if (instr.op == OpCode::Ret) {
        break;
      }
    }
  }
}

void
TACBuilder::propagateVersions(BlockContext& ctx, std::vector<Block*>& worklist)
{
  // propagate incoming versions into successors
  for (auto& pair : ctx.inVersions) {
    SSAIdentityID id = pair.first;
    if (ctx.usedDefinitions.find(id) == ctx.usedDefinitions.end() &&
        ctx.outVersions.find(id) == ctx.outVersions.end())
      ctx.outVersions.try_emplace(id, pair.second.back());
  }

  ctx.inVersions.clear();
  if (ctx.outVersions.empty())
    return;

  for (auto& b : ctx.outBlocks) {
    worklist.push_back(b);
    auto outCtxIt = _ssaBlocksMap.try_emplace(b->id.val);
    auto& outCtx = outCtxIt.first->second;

    for (auto& out : ctx.outVersions) {
      auto inVersionsIt = outCtx.inVersions.try_emplace(out.first);
      inVersionsIt.first->second.push_back(out.second);
    }
  }

  ctx.outVersions.clear();
}

void
TACBuilder::ensurePhi(BlockContext& ctx,
                      SSAIdentityID id,
                      std::vector<SSAVersion>& versions)
{
  auto ctxVerIt = ctx.usedVersions.find(id);

  // update existing phi if needed
  if (ctxVerIt != ctx.usedVersions.end()) {
    bool needsPhi = false;
    SSAVersion currentVer = ctxVerIt->second;
    auto currentUnfolded = unfoldVersions(id, { currentVer });
    auto incomingUnfolded = unfoldVersions(id, versions);

    auto& smaller = currentUnfolded.size() > incomingUnfolded.size()
                      ? incomingUnfolded
                      : currentUnfolded;
    auto& bigger = currentUnfolded.size() > incomingUnfolded.size()
                     ? currentUnfolded
                     : incomingUnfolded;

    // check if vectors versions are compatible
    for (auto& v : smaller) {
      if (bigger.find(v) == bigger.end()) {
        needsPhi = true;
        break;
      }
    }

    if (!needsPhi) {
      // update version if it gives more info and is compatible
      if (versions.size() == 1 &&
          incomingUnfolded.size() > currentUnfolded.size())
        ctxVerIt->second = versions.back();

      return;
    }

    versions.insert(versions.begin(), currentVer);
  }
  // dont make phi if symbol is unused and only one version incoming
  // also dont make if both incoming versions are the same
  // TODO: fix this
  else if (versions.size() == 1 || versions.at(0) == versions.at(1)) {
    ctx.usedVersions.try_emplace(id, versions.back());
    return;
  }

  // make new phi
  makePhi(ctx, id, versions);
}

void
TACBuilder::makePhi(BlockContext& ctx,
                    SSAIdentityID id,
                    const std::vector<SSAVersion>& versions)
{
  auto phiIt = ctx.phiFunctions.find(id);
  if (phiIt != ctx.phiFunctions.end()) {
    auto& choices = phiIt->second->choices;
    choices.insert(choices.end(), versions.begin(), versions.end());
    ctx.outVersions.try_emplace(id, phiIt->second->ver);
    return;
  }

  PhiFunction phi = {};
  phi.id = id;
  phi.ver = _ssa->assign(id,
                         TACValue{ _ssa->makeValueID(),
                                   ValueType::SSAValue,
                                   ValueSource::Computed });
  phi.choices = versions;
  phi.res = _ssa->makeValueID();

  auto outerIt = _phiFunctions.try_emplace(id);
  auto newPhiIt = outerIt.first->second.try_emplace(phi.ver, phi);
  auto& newPhi = newPhiIt.first->second;
  ctx.phiFunctions.try_emplace(id, &newPhi);
  ctx.usedVersions.insert_or_assign(id, newPhi.ver);

  // if there is no definition of given identity later, then add it to outcoming
  // versions
  if (ctx.usedDefinitions.find(id) == ctx.usedDefinitions.end() &&
      newPhi.choices.size() > 1)
    ctx.outVersions.try_emplace(id, newPhi.ver);
}

std::set<SSAVersion>
TACBuilder::unfoldVersions(SSAIdentityID id,
                           const std::vector<SSAVersion>& versions)
{
  std::set<SSAVersion> unfolded;
  std::vector<SSAVersion> worklist = versions;
  auto outerIt = _phiFunctions.try_emplace(id);
  auto& phiVersions = outerIt.first->second;

  while (!worklist.empty()) {
    SSAVersion v = worklist.back();
    worklist.pop_back();

    auto phiIt = phiVersions.find(v);
    if (phiIt != phiVersions.end())
      worklist.insert(worklist.end(),
                      phiIt->second.choices.begin(),
                      phiIt->second.choices.end());
    else
      unfolded.emplace(v);
  }

  return unfolded;
}

void
TACBuilder::insertPhi(Block* b, PhiFunction& phi)
{
  std::vector<TACValue> operands;
  for (auto& ver : phi.choices)
    operands.push_back({ SSAIdentityState{ phi.id, ver },
                         ValueType::SSAIdentity,
                         ValueSource::Computed });

  TACValue newVal = { SSAIdentityState{ phi.id, phi.ver },
                      ValueType::SSAIdentity,
                      ValueSource::Computed };

  Instruction phiInstr = { OpCode::Phi, operands, phi.res };
  Instruction store = { OpCode::Store, { TACValue{ phi.res }, newVal } };

  b->instructions.insert(b->instructions.begin(), store);
  b->instructions.insert(b->instructions.begin(), phiInstr);
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
    case OpCode::Phi:
      return "phi";
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
  else if (val.src == ValueSource::Parameter)
    str += "param ";

  if (auto boolVal = std::get_if<bool>(&val.id)) {
    str += boolVal ? "true" : "false";
  } else if (auto numVal = std::get_if<double>(&val.id)) {
    str += std::to_string(*numVal);
  } else if (auto literalID = std::get_if<LiteralID>(&val.id)) {
    str += "ltrl" + std::to_string(literalID->val);
  } else if (auto valID = std::get_if<ValueID>(&val.id)) {
    str += "@" + std::to_string(valID->val);
  } else if (auto ssaId = std::get_if<SSAIdentityState>(&val.id)) {
    str += _ssa->getIdentityStr(ssaId->id) + std::to_string(ssaId->id.val) +
           "_" + std::to_string(ssaId->ver);
  } else if (auto varID = std::get_if<SSAIdentityID>(&val.id)) {
    str += "var" + std::to_string(varID->val);
  } else if (auto fnID = std::get_if<FunctionID>(&val.id)) {
    str += "fn" + std::to_string(fnID->val);
  } else if (auto blockID = std::get_if<BlockID>(&val.id)) {
    str += "blk" + std::to_string(blockID->val);
  }

  return str;
}