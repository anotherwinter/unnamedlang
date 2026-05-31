#include "middle/tacoptimizer.h"
#include "foldfunctions.h"
#include "middle/tac.h"
#include <cmath>
#include <unordered_set>

using namespace MIR;

TACOptimizer::TACOptimizer()
{
  constructFoldTable();
}

TACOptimizer::~TACOptimizer() {}

TACBlocks<Optimized>
TACOptimizer::optimize(TACBlocks<Unoptimized> blocks)
{
  _symUses.clear();
  _valueUses.clear();
  _optimizedBlocks.clear();
  _deadBlocks.clear();
  _optimizerBlocksCtx.clear();

  _begin = blocks.begin;
  _end = blocks.blocks->back();
  _unoptimizedBlocks = blocks.blocks;

  optimizerPass();

  return { _begin, &_optimizedBlocks };
}

void
TACOptimizer::constructFoldTable()
{
  addFoldFunction(foldBoolBool, FoldBoolType, FoldBoolType);
  addFoldFunction(foldNumberNumber, FoldNumberType, FoldNumberType);
}

void
TACOptimizer::constructDefUse()
{
  bool afterJmp = false;
  std::unordered_set<BlockID::__BlockID> visited;
  std::vector<Block*> worklist;
  std::vector<BlockID> splits;

  worklist.push_back(_begin);
  size_t idx = 0;
  while (idx < worklist.size()) {
    afterJmp = false;
    Block* current = worklist[idx];
    ++idx;
    if (visited.find(current->id.val) != visited.end())
      continue;

    visited.emplace(current->id.val);
    auto& ctx = _optimizerBlocksCtx.try_emplace(current->id.val).first->second;
    for (auto& instr : current->instructions) {
      // mark instruction as dead if it appears after jmp/condjmp/ret
      if (afterJmp) {
        instr->op = OpCode::Dead;
        continue;
      }

      if (isValid(instr->res)) {
        auto valUsesOuterIt = _valueUses.try_emplace(instr->res.val);
        valUsesOuterIt.first->second.def = instr;

        if (instr->op == OpCode::Phi)
          ctx.phiFunctions.push_back(instr);

      } else if (instr->op == OpCode::Store) {
        auto operand = instr->operands.at(1);
        if (operand.type == ValueType::SSAIdentity) {
          auto id = std::get<SSAIdentityState>(operand.id);
          auto symUsesOuterIt = _symUses.try_emplace(id);
          symUsesOuterIt.first->second.def = instr;
        }
      } else if (instr->op == OpCode::Jmp) {
        auto blockID = std::get<BlockID>(instr->operands.at(0).id);
        afterJmp = true;
        ctx.jump = instr;

        auto& preds = _preds.try_emplace(blockID.val).first->second;
        preds.push_back(current->id.val);

        worklist.push_back(_unoptimizedBlocks->at(blockID.val));
      } else if (instr->op == OpCode::CondJmp) {
        auto& ifBlockID = std::get<BlockID>(instr->operands.at(1).id);
        auto& elseBlockID = std::get<BlockID>(instr->operands.at(2).id);
        afterJmp = true;
        ctx.jump = instr;

        splits.push_back(current->id);

        auto& ifPreds = _preds.try_emplace(ifBlockID.val).first->second;
        ifPreds.push_back(current->id.val);
        auto& elsePreds = _preds.try_emplace(elseBlockID.val).first->second;
        elsePreds.push_back(current->id.val);

        worklist.push_back(_unoptimizedBlocks->at(ifBlockID.val));
        worklist.push_back(_unoptimizedBlocks->at(elseBlockID.val));
      } else if (instr->op == OpCode::Ret) {
        afterJmp = true;
      }

      // jumps are not counted towards alive instructions counter
      if (instr->op != OpCode::Jmp)
        ++ctx.aliveInstructions;

      for (size_t i = 0; i < instr->operands.size(); ++i) {
        auto operand = instr->operands.at(i);
        DefUseChain* defUse = nullptr;
        if (operand.type == ValueType::SSAValue) {
          auto id = std::get<ValueID>(operand.id);
          auto valUsesOuterIt = _valueUses.try_emplace(id.val);

          defUse = &valUsesOuterIt.first->second;
        } else if (operand.type == ValueType::SSAIdentity) {
          auto id = std::get<SSAIdentityState>(operand.id);
          auto symUsesOuterIt = _symUses.try_emplace(id);

          defUse = &symUsesOuterIt.first->second;
        }

        if (defUse) {
          defUse->uses.push_back({ instr, i });

          // dont add to aliveuses for operands if phi since phi has weak
          // binding
          if (instr->op != OpCode::Phi)
            ++defUse->aliveUses;
        }
      }
    }
  }
}

void
TACOptimizer::copyPropagation()
{
  // SSA identities rewriting pass
  size_t idx = 0;
  for (auto& [id, defUse] : _symUses) {
    auto& operand = defUse.def->operands.at(0);

    DefUseChain* useDef = nullptr;
    if (operand.type == ValueType::SSAValue) {
      useDef = &_valueUses.find(std::get<ValueID>(operand.id).val)->second;
      if (useDef)
        --useDef->aliveUses;
    }
    for (auto& use : defUse.uses) {
      if (use.user->op == OpCode::Store) {
        killInstruction(use.user);
        continue;
      }

      use.user->operands.at(use.operandIndex) = operand;
      if (useDef) {
        useDef->uses.push_back(use);

        // dont add to aliveuses for operands if phi since phi has weak
        // binding
        if (use.user->op != OpCode::Phi)
          ++useDef->aliveUses;
      }
    }
  }

  for (auto& [id, defUse] : _valueUses) {
    if (isValid(defUse.def->res))
      _foldingWorklist.push_back(&defUse);
  }
}

void
TACOptimizer::constantFolding()
{
  size_t idx = 0;
  while (idx < _foldingWorklist.size()) {
    DefUseChain* current = _foldingWorklist[idx];
    ++idx;

    auto& def = current->def;
    bool compatible = true;
    for (auto& operand : def->operands) {
      if (to_underlying(operand.type) >= FoldTypesCount) {
        compatible = false;
        break;
      }
    }

    TACValue propagateVal;
    // if requirements are met, try constant folding
    if (def->op >= OpCode::Add && def->op <= OpCode::ArithmNeg) {
      if (compatible) {
        TACValue& operand1 = def->operands.at(0);
        TACValue& operand2 =
          def->operands.size() == 2 ? def->operands.at(1) : def->operands.at(0);
        auto& fn = foldFunctions[to_underlying(operand1.type)]
                                [to_underlying(operand2.type)];
        if (fn)
          propagateVal = fn(def->op, &operand1, &operand2);
        else
          continue;
      }
      // try to use strength reduction later if cannot be folded now
      else {
        _reductionWorklist.push_back(current);
        continue;
      }
    } else if (def->op == OpCode::Load) {
      propagateVal = def->operands.at(0);
    }
    // try to fold phi if has same values for both operands
    // TODO: change this
    else if (def->op == OpCode::Phi) {
      auto& lhs = def->operands.at(1);
      auto& rhs = def->operands.at(3);

      bool propagated = false;
      if (lhs.type == ValueType::SSAValue &&
          std::get<ValueID>(lhs.id) == def->res) {
        propagateVal = rhs;
        propagated = true;
      } else if (rhs.type == ValueType::SSAValue &&
                 std::get<ValueID>(rhs.id) == def->res) {
        propagateVal = lhs;
        propagated = true;
      }

      bool operandsEqual = lhs.id == rhs.id;
      if (!propagated && operandsEqual) {
        propagateVal = def->operands.at(1);
      } else if (propagated && !operandsEqual) {

      } else
        continue;
    } else {
      continue;
    }

    killInstruction(def);

    // try copy/constant propagation
    DefUseChain* useDef = nullptr;
    if (propagateVal.type == ValueType::SSAValue)
      useDef = &_valueUses.find(std::get<ValueID>(propagateVal.id).val)->second;
    for (auto& use : current->uses) {
      use.user->operands.at(use.operandIndex) = propagateVal;
      if (isValid(use.user->res)) {
        auto it = _valueUses.find(use.user->res.val);
        _foldingWorklist.push_back(&it->second);
      }
      if (useDef)
        useDef->uses.push_back(use);
    }
  }

  _foldingWorklist.clear();
}

void
TACOptimizer::strengthReduction()
{
  size_t idx = 0;
  while (idx < _reductionWorklist.size()) {
    DefUseChain* current = _reductionWorklist[idx];
    ++idx;

    // TODO: change this
    // for now, there are only binary operations to reduce strength for, so it
    // is safe to assume that operands count == 2
    size_t immediateIdx = 0;
    size_t otherIdx = 0;
    auto def = current->def;
    for (size_t i = 0; i < def->operands.size(); ++i) {
      if (def->operands.at(i).src == ValueSource::Immediate)
        immediateIdx += (i + 1);
      else if (def->operands.at(i).type == ValueType::SSAValue)
        otherIdx = i;
    }

    if (immediateIdx == 0 || immediateIdx > 2)
      continue;

    auto& immediateOperand = def->operands.at(immediateIdx - 1);
    double numVal;
    bool boolVal;
    bool zeroOrFalse;
    bool isOne;
    bool isPowerOfTwo;
    double power;
    if (immediateOperand.type == ValueType::ImmediateDouble) {
      numVal = std::get<double>(immediateOperand.id);
      zeroOrFalse = numVal == 0.0;
      isOne = numVal == 1.0;

      double intpart;
      bool isInteger = std::modf(numVal, &intpart) == 0.0;
      uint64_t uintVal = numVal;
      isPowerOfTwo = isInteger && numVal > 0.0 &&
                     (uintVal != 0 && (uintVal & (uintVal - 1)) == 0);
      if (isPowerOfTwo)
        power = __builtin_ctzll(uintVal);
    } else if (immediateOperand.type == ValueType::ImmediateBool) {
      boolVal = std::get<bool>(immediateOperand.id);
      zeroOrFalse = !boolVal;
    }

    auto& otherOperand = def->operands.at(otherIdx);
    switch (def->op) {
      case OpCode::Add:
      case OpCode::Sub:
      case OpCode::Shiftl:
      case OpCode::Shiftr:
      case OpCode::Or:
      case OpCode::Xor: {
        if (zeroOrFalse) {
          def->op = OpCode::Load;
          def->operands = { otherOperand };
        }

        break;
      }
      case OpCode::Mul: {
        if (isPowerOfTwo) {
          def->op = OpCode::Shiftl;
          immediateOperand.id = power;
          break;
        }
      }
      case OpCode::Div: {
        if (isPowerOfTwo) {
          def->op = OpCode::Shiftr;
          immediateOperand.id = power;
          break;
        }
      }
      case OpCode::And:
      case OpCode::Mod: {
        if (zeroOrFalse) {
          def->op = OpCode::Load;
          TACValue operandZero = { 0.0,
                                   ValueType::ImmediateDouble,
                                   ValueSource::Immediate };
          def->operands = { operandZero };
        } else if (isOne) {
          def->op = OpCode::Load;
          def->operands = { otherOperand };
        }

        break;
      }
      case OpCode::LogicAnd: {
        if (otherIdx == 1) {
          def->op = OpCode::Load;
          if (zeroOrFalse) {
            TACValue operandFalse = { false,
                                      ValueType::ImmediateBool,
                                      ValueSource::Immediate };
            def->operands = { operandFalse };
          } else {
            def->operands = { otherOperand };
          }
        }

        break;
      }
      case OpCode::LogicOr: {
        if (otherIdx == 1) {
          def->op = OpCode::Load;
          if (!zeroOrFalse) {
            TACValue operandTrue = { true,
                                     ValueType::ImmediateBool,
                                     ValueSource::Immediate };
            def->operands = { operandTrue };
          } else {
            def->operands = { otherOperand };
          }
        }

        break;
      }
      default:
        break;
    }

    // if reduced successfully, add to folding worklist
    if (def->op == OpCode::Load)
      _foldingWorklist.push_back(current);
  }

  _reductionWorklist.clear();
}

const std::vector<BlockID::__BlockID>
TACOptimizer::peepholeSimplify()
{
  // control flow simplification
  std::unordered_set<BlockID::__BlockID> visited;
  std::vector<Block*> worklist;

  worklist.push_back(_begin);
  size_t idx = 0;
  while (idx < worklist.size()) {
    Block* current = worklist[idx];
    ++idx;

    if (visited.find(current->id.val) != visited.end())
      continue;

    visited.emplace(current->id.val);

    // TODO: make this more aware about dead instructions
    auto& ctx = _optimizerBlocksCtx.try_emplace(current->id.val).first->second;
    auto jump = ctx.jump;
    if (!jump)
      continue;

    if (jump->op == OpCode::Jmp) {
      // TODO: make this more aware about dead instructions
      auto operand = jump->operands.at(0);
      auto nextBlock =
        _unoptimizedBlocks->at(std::get<BlockID>(operand.id).val);
      auto& nextBlockCtx =
        _optimizerBlocksCtx.try_emplace(nextBlock->id.val).first->second;

      worklist.push_back(nextBlock);
    }
    // try to simplify condjmps if condition is known at compile-time
    else if (jump->op == OpCode::CondJmp) {
      auto condOperand = jump->operands.at(0);
      bool cond;
      auto& ifBlockID = std::get<BlockID>(jump->operands.at(1).id);
      auto& elseBlockID = std::get<BlockID>(jump->operands.at(2).id);

      if (condOperand.type == ValueType::ImmediateBool)
        cond = std::get<bool>(condOperand.id);
      else if (condOperand.type == ValueType::ImmediateDouble)
        cond = std::get<double>(condOperand.id);
      else if (ifBlockID == elseBlockID) {
        cond = true;

        if (condOperand.type == ValueType::SSAValue) {
          auto& valUsesIt =
            _valueUses.find(std::get<ValueID>(condOperand.id).val)->second;
          --valUsesIt.aliveUses;
        }
      } else {
        worklist.push_back(_unoptimizedBlocks->at(ifBlockID.val));
        worklist.push_back(_unoptimizedBlocks->at(elseBlockID.val));

        continue;
      }

      TACValue jmpTarget = cond ? jump->operands.at(1) : jump->operands.at(2);
      jump->op = OpCode::Jmp;
      jump->operands = { jmpTarget };

      auto nextBlock =
        _unoptimizedBlocks->at(std::get<BlockID>(jmpTarget.id).val);
      auto& nextBlockCtx =
        _optimizerBlocksCtx.try_emplace(nextBlock->id.val).first->second;

      worklist.push_back(nextBlock);
      // remove current block from visited so it will be rechecked
      visited.erase(current->id.val);

      --ctx.aliveInstructions;
    }
  }

  std::vector<BlockID::__BlockID> vec = {};
  vec.insert(vec.begin(), visited.begin(), visited.end());

  return vec;
}

void
TACOptimizer::peepholeRewire()
{
  // control flow rewiring
  std::unordered_set<BlockID::__BlockID> visited;
  std::vector<Block*> worklist;
  worklist.push_back(_begin);
  size_t idx = 0;
  while (idx < worklist.size()) {
    Block* current = worklist[idx];
    ++idx;

    if (visited.find(current->id.val) != visited.end())
      continue;

    visited.emplace(current->id.val);

    // TODO: make this more aware about dead instructions
    auto& ctx = _optimizerBlocksCtx.try_emplace(current->id.val).first->second;
    auto jump = ctx.jump;
    if (!jump)
      continue;

    if (jump->op == OpCode::Jmp) {
      // TODO: make this more aware about dead instructions
      auto& nextBlockID = std::get<BlockID>(jump->operands.at(0).id);
      auto& nextBlockCtx =
        _optimizerBlocksCtx.try_emplace(nextBlockID.val).first->second;

      if (nextBlockCtx.aliveInstructions == 0) {
        BlockID resolvedID = resolveTarget(nextBlockID);
        if (isValid(resolvedID)) {
          // increment alive instructions if detected endless loop so it wont be
          // optimized out
          // TODO: change this
          if (nextBlockID == resolvedID) {
            ++ctx.aliveInstructions;
            nextBlockID = current->id;

            continue;
          }

          nextBlockID = resolvedID;
        }
      }

      worklist.push_back(_unoptimizedBlocks->at(nextBlockID.val));
    } else if (jump->op == OpCode::CondJmp) {
      auto& ifBlockID = std::get<BlockID>(jump->operands.at(1).id);
      auto& elseBlockID = std::get<BlockID>(jump->operands.at(2).id);

      auto& ifBlockCtx =
        _optimizerBlocksCtx.try_emplace(ifBlockID.val).first->second;
      auto& elseBlockCtx =
        _optimizerBlocksCtx.try_emplace(elseBlockID.val).first->second;

      if (ctx.aliveInstructions > 0) {
        BlockID resolvedID;
        if (ifBlockCtx.aliveInstructions == 1 && ifBlockCtx.jump) {
          resolvedID = resolveTarget(ifBlockID);
          if (isValid(resolvedID))
            ifBlockID = resolvedID;
        }

        if (elseBlockCtx.aliveInstructions == 1 && elseBlockCtx.jump) {
          resolvedID = resolveTarget(elseBlockID);
          if (isValid(resolvedID))
            elseBlockID = resolvedID;
        }
      }

      worklist.push_back(_unoptimizedBlocks->at(ifBlockID.val));
      worklist.push_back(_unoptimizedBlocks->at(elseBlockID.val));
    }
  }
}

BlockID
TACOptimizer::resolveTarget(BlockID begin)
{
  std::unordered_set<BlockID::__BlockID> visited;
  Block* startBlock = _unoptimizedBlocks->at(begin.val);
  std::vector<Block*> worklist;
  worklist.push_back(startBlock);
  while (!worklist.empty()) {
    Block* current = worklist.back();
    worklist.pop_back();

    if (visited.find(current->id.val) != visited.end())
      continue;

    visited.emplace(current->id.val);
    auto& ctx = _optimizerBlocksCtx.find(current->id.val)->second;
    if (ctx.aliveInstructions > 1 || (ctx.aliveInstructions == 1 && !ctx.jump))
      return current->id;

    if (!ctx.jump)
      break;

    if (ctx.jump->op == OpCode::Jmp) {
      auto blockID = std::get<BlockID>(ctx.jump->operands.at(0).id);
      // if detected loop, return from it
      if (blockID == begin)
        return begin;

      worklist.push_back(_unoptimizedBlocks->at(blockID.val));
    }
  }

  return {};
}

void
TACOptimizer::cleanupDead(const std::vector<BlockID::__BlockID>& reachable)
{
  // kill unused phi if their users are not used anywhere
  // TODO: change this and move somewhere
  std::vector<DefUseChain*> cleanupWorklist;
  for (auto& id : reachable) {
  }
}

void
TACOptimizer::dce()
{
  // pass1 - remove dead instructions and blocks
  std::vector<Block*> worklist;
  std::unordered_set<BlockID::__BlockID> visited;
  std::vector<Block*> reachable = {};
  worklist.push_back(_begin);
  size_t idx = 0;
  Block* newBegin = nullptr;
  while (idx < worklist.size()) {
    Block* current = worklist[idx];
    ++idx;

    if (visited.find(current->id.val) != visited.end())
      continue;

    auto& ctx = _optimizerBlocksCtx.try_emplace(current->id.val).first->second;
    visited.emplace(current->id.val);
    if (_usedBlocks.find(current->id.val) == _usedBlocks.end() &&
        ctx.aliveInstructions == 0 && current != _end) {
      _deadBlocks.push_back(current);
    } else {
      reachable.push_back(current);
      if (!newBegin)
        newBegin = current;

      std::vector<Instruction*> usedInstructions;
      for (auto& instr : current->instructions) {
        if (instr->op == OpCode::Dead)
          continue;

        usedInstructions.push_back(instr);
      }
      current->instructions = usedInstructions;
    }

    if (!ctx.jump)
      continue;

    auto& jump = ctx.jump;
    if (jump->op == OpCode::Jmp) {
      auto blockID = std::get<BlockID>(jump->operands.at(0).id);
      worklist.push_back(_unoptimizedBlocks->at(blockID.val));

    } else if (jump->op == OpCode::CondJmp) {
      auto ifBlockID = std::get<BlockID>(jump->operands.at(1).id);
      auto elseBlockID = std::get<BlockID>(jump->operands.at(2).id);

      worklist.push_back(_unoptimizedBlocks->at(ifBlockID.val));
      worklist.push_back(_unoptimizedBlocks->at(elseBlockID.val));
    }
  }

  _optimizedBlocks = reachable;
  _begin = newBegin;
}

void
TACOptimizer::repair()
{
  _usedBlocks.clear();

  for (auto& ctx : _optimizerBlocksCtx) {
    std::vector<Instruction*> newPhiFunctions;
    for (auto& phi : ctx.second.phiFunctions) {
      if (phi->op != OpCode::Dead)
        newPhiFunctions.push_back(phi);
    }

    ctx.second.phiFunctions = newPhiFunctions;
  }

  std::unordered_set<Block*> visited;
  std::vector<Block*> worklist;
  worklist.push_back(_begin);
  while (!worklist.empty()) {
    auto& current = worklist.back();
    worklist.pop_back();

    if (visited.find(current) != visited.end())
      continue;

    visited.emplace(current);
    auto& ctx = _optimizerBlocksCtx.find(current->id.val)->second;
    if (!ctx.jump)
      continue;

    if (ctx.jump->op == OpCode::Jmp) {
      auto& nextBlockID = std::get<BlockID>(ctx.jump->operands.at(0).id);
      updatePhi(nextBlockID, current->id);

      worklist.push_back(_unoptimizedBlocks->at(nextBlockID.val));
    } else if (ctx.jump->op == OpCode::CondJmp) {
      auto& ifBlockID = std::get<BlockID>(ctx.jump->operands.at(1).id);
      auto& elseBlockID = std::get<BlockID>(ctx.jump->operands.at(2).id);

      updatePhi(ifBlockID, current->id);
      updatePhi(elseBlockID, current->id);

      worklist.push_back(_unoptimizedBlocks->at(ifBlockID.val));
      worklist.push_back(_unoptimizedBlocks->at(elseBlockID.val));
    }
  }

  for (auto& ctx : _optimizerBlocksCtx) {
    for (auto& phi : ctx.second.phiFunctions) {
      if (phi->op != OpCode::Phi)
        continue;

      bool simplifiedPhi = false;
      size_t phiValidIdx = 0;
      size_t phiInvalidIdx;
      for (size_t i = 0; i < phi->operands.size(); i += 2) {
        BlockID& id = std::get<BlockID>(phi->operands.at(i).id);
        if (!isValid(id)) {
          phiInvalidIdx = i + 1;
          simplifiedPhi = true;
        } else {
          _usedBlocks.emplace(id.val);
          phiValidIdx = i + 1;
        }
      }

      if (simplifiedPhi && phiValidIdx != 0) {
        phi->operands.at(phiInvalidIdx) = phi->operands.at(phiValidIdx);
        auto& defUse = _valueUses.find(phi->res.val)->second;
        _foldingWorklist.push_back(&defUse);
      }
    }
  }

  updatePreds();
}

void
TACOptimizer::updatePhi(BlockID phiBlock, BlockID in)
{
  auto& ctx = _optimizerBlocksCtx.find(phiBlock.val)->second;
  if (ctx.phiFunctions.empty())
    return;

  size_t choiceIdx;
  auto& preds = _preds.try_emplace(phiBlock.val).first->second;
  bool found = false;
  for (size_t i = 0; i < preds.size(); ++i) {
    if (preds[i] == in.val) {
      choiceIdx = i;
      found = true;
      break;
    }
  }

  if (!found)
    return;

  choiceIdx *= 2;

  for (auto& phi : ctx.phiFunctions) {
    BlockID& choiceID = std::get<BlockID>(phi->operands.at(choiceIdx).id);
    choiceID = in;
  }
}

void
TACOptimizer::updatePreds()
{
  _preds.clear();
  std::unordered_set<BlockID::__BlockID> visited;
  std::vector<Block*> worklist;

  worklist.push_back(_begin);
  size_t idx = 0;
  while (idx < worklist.size()) {
    Block* current = worklist[idx];
    ++idx;
    if (visited.find(current->id.val) != visited.end())
      continue;

    visited.emplace(current->id.val);
    auto& ctx = _optimizerBlocksCtx.find(current->id.val)->second;
    if (!ctx.jump)
      continue;

    if (ctx.jump->op == OpCode::Jmp) {
      auto blockID = std::get<BlockID>(ctx.jump->operands.at(0).id);

      auto& preds = _preds.try_emplace(blockID.val).first->second;
      preds.push_back(current->id.val);

      worklist.push_back(_unoptimizedBlocks->at(blockID.val));
    } else if (ctx.jump->op == OpCode::CondJmp) {
      auto ifBlockID = std::get<BlockID>(ctx.jump->operands.at(1).id);
      auto elseBlockID = std::get<BlockID>(ctx.jump->operands.at(2).id);

      auto& ifPreds = _preds.try_emplace(ifBlockID.val).first->second;
      ifPreds.push_back(current->id.val);
      auto& elsePreds = _preds.try_emplace(elseBlockID.val).first->second;
      elsePreds.push_back(current->id.val);

      worklist.push_back(_unoptimizedBlocks->at(ifBlockID.val));
      worklist.push_back(_unoptimizedBlocks->at(elseBlockID.val));
    }
  }
}