#pragma once
#include "middle/tac.h"
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace MIR {

struct Use
{
  Instruction* user;
  size_t operandIndex;
};

struct DefUseChain
{
  Instruction* def;
  std::vector<Use> uses;
  size_t aliveUses;
};

struct OptimizerBlockContext
{
  size_t aliveInstructions;
  Instruction* jump;
  std::vector<Instruction*> phiFunctions;
};

using SymbolUsesMap =
  std::unordered_map<SSAIdentityState, DefUseChain, SSAIdentityStateHash>;
using ValueUsesMap = std::unordered_map<ValueID::__ValueID, DefUseChain>;
using OptimizerBlockContextMap =
  std::unordered_map<BlockID::__BlockID, OptimizerBlockContext>;
using SplitMergeMap =
  std::unordered_map<BlockID::__BlockID, BlockID::__BlockID>;
using UsedBlocks = std::unordered_set<BlockID::__BlockID>;

using FoldFn =
  std::function<TACValue(OpCode op, const TACValue* lhs, const TACValue* rhs)>;

constexpr size_t FoldTypesCount = 2;
constexpr size_t FoldBoolType = 0;
constexpr size_t FoldNumberType = 1;

class TACOptimizer
{
public:
  TACOptimizer();
  ~TACOptimizer();

  TACBlocks<Optimized> optimize(TACBlocks<Unoptimized> blocks);

private:
  TACOptimizer(const TACOptimizer& other) = delete;
  TACOptimizer(TACOptimizer&& other) = delete;

  SymbolUsesMap _symUses;
  ValueUsesMap _valueUses;
  Block* _begin;
  Block* _end;
  std::vector<Block*>* _unoptimizedBlocks;
  std::vector<Block*> _optimizedBlocks;
  std::vector<Block*> _deadBlocks;
  OptimizerBlockContextMap _optimizerBlocksCtx;
  SplitMergeMap _splitMerge;

  FoldFn foldFunctions[FoldTypesCount][FoldTypesCount];

  std::vector<DefUseChain*> _foldingWorklist;
  std::vector<DefUseChain*> _reductionWorklist;
  std::unordered_map<BlockID::__BlockID, std::vector<BlockID::__BlockID>>
    _preds;
  UsedBlocks _usedBlocks;

  inline void addFoldFunction(FoldFn fn, size_t type1, size_t type2)
  {
    foldFunctions[type1][type2] = fn;
  }

  inline void killInstruction(Instruction* instr)
  {
    instr->op = OpCode::Dead;
    auto& ctx =
      _optimizerBlocksCtx.try_emplace(instr->owner->id.val).first->second;
    --ctx.aliveInstructions;

    for (auto& operand : instr->operands) {
      if (operand.type == ValueType::SSAValue) {
        auto& id = std::get<ValueID>(operand.id);
        auto& defUse = _valueUses.find(id.val)->second;
        --defUse.aliveUses;
      }
    }
  }

  void constructFoldTable();

  void constructDefUse();

  void copyPropagation();

  void constantFolding();

  void strengthReduction();

  const std::vector<BlockID::__BlockID> peepholeSimplify();

  void peepholeRewire();

  BlockID resolveTarget(BlockID begin);

  void cleanupDead(const std::vector<BlockID::__BlockID>& reachable);

  void dce();

  void repair();

  void updatePhi(BlockID phiBlock, BlockID in);

  void updatePreds();

  inline void optimizerPass()
  {
    constructDefUse();

    copyPropagation();
    constantFolding();

    strengthReduction();
    constantFolding();

    auto reachable = peepholeSimplify();
    repair();
    // use peephole to re-wire blocks
    peepholeRewire();
    repair();

    dce();
  }
};

}