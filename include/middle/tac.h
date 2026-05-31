#pragma once
#include "alloc/arena.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace MIR {
enum class OpCode : uint8_t
{
  // get operand without deref
  Move,

  // deref operand and get value
  Load,

  // dynamic read - reads operand appropriately to its type
  // may get address or value
  DynRead,

  Store,
  Jmp,
  Call,
  Param,
  LoadParam,
  CondJmp,
  Add,
  Sub,
  Mul,
  Div,
  Mod,
  Shiftl,
  Shiftr,
  And,
  Or,
  Xor,
  Lt,
  Gt,
  Leq,
  Geq,
  Eq,
  Neq,

  LogicAnd,
  LogicOr,

  LogicNeg,
  ArithmNeg,
  Inc,
  Dec,
  Get,
  GetRef,
  Ret,

  Exit,

  // phi-function pseudoinstruction, first operand is destination
  Phi,

  // optimized out instruction
  Dead,
};

// id for value, produced by instruction
struct ValueID
{
  using __ValueID = uint32_t;
  static constexpr __ValueID invalidID = std::numeric_limits<__ValueID>::max();
  uint32_t val = std::numeric_limits<__ValueID>::max();

  inline bool operator==(const ValueID& other) const
  {
    return val == other.val;
  }

  inline bool operator!=(const ValueID& other) const
  {
    return val != other.val;
  }

  inline bool operator<(const ValueID& other) const { return val < other.val; }

  inline ValueID operator++()
  {
    ++val;
    return *this;
  }

  inline ValueID operator++(int)
  {
    val++;
    return *this;
  }
};

// id for  of instructions
struct BlockID
{
  using __BlockID = uint32_t;
  static constexpr __BlockID invalidID = std::numeric_limits<__BlockID>::max();
  uint32_t val = std::numeric_limits<__BlockID>::max();

  inline bool operator==(const BlockID& other) const
  {
    return val == other.val;
  }

  inline bool operator!=(const BlockID& other) const
  {
    return val != other.val;
  }

  inline bool operator<(const BlockID& other) const { return val < other.val; }

  inline BlockID operator++()
  {
    ++val;
    return *this;
  }

  inline BlockID operator++(int)
  {
    val++;
    return *this;
  }
};

// id for literal value
struct LiteralID
{
  using __LiteralID = uint32_t;
  static constexpr __LiteralID invalidID =
    std::numeric_limits<__LiteralID>::max();
  uint32_t val = std::numeric_limits<__LiteralID>::max();

  inline bool operator==(const LiteralID& other) const
  {
    return val == other.val;
  }

  inline bool operator!=(const LiteralID& other) const
  {
    return val != other.val;
  }

  inline bool operator<(const LiteralID& other) const
  {
    return val < other.val;
  }

  inline LiteralID operator++()
  {
    ++val;
    return *this;
  }

  inline LiteralID operator++(int)
  {
    val++;
    return *this;
  }
};

inline constexpr bool
isValid(ValueID id)
{
  return id.val != ValueID::invalidID;
}

inline constexpr bool
isValid(BlockID id)
{
  return id.val != BlockID::invalidID;
}

inline constexpr bool
isValid(LiteralID id)
{
  return id.val != LiteralID::invalidID;
}

struct BoolLiteral
{
  bool val;
};

struct NumberLiteral
{
  double val;
};

struct StringLiteral
{
  const char* val;
};

struct ArrayLiteral
{
  std::vector<LiteralID> values;
};

struct LiteralPayload
{
  std::variant<BoolLiteral, NumberLiteral, StringLiteral, ArrayLiteral> val;
};

enum class ValueType : uint8_t
{
  // immediates
  ImmediateBool = 0,
  ImmediateDouble,

  // string literal
  Literal,

  // value, produced by instruction
  SSAValue,

  SSAIdentity,

  // id for block of instructions
  Block,

  Place,
  StaticFn,
  Self,

  // used for SSA slots
  Nothing,
};

enum class ValueSource
{
  Immediate,
  Literal,
  Computed,
  Static,
  Dynamic,

  // for getting parameters
  Parameter,
};

using SSAIdentityID = VarID;
using SSAVersion = uint32_t;

struct SSAIdentityState
{
  SSAIdentityID id;
  SSAVersion ver;

  bool operator==(SSAIdentityState const& other) const noexcept
  {
    return id == other.id && ver == other.ver;
  }
};

struct SSAIdentityStateHash
{
  size_t operator()(SSAIdentityState const& k) const noexcept
  {
    return std::hash<size_t>{}(size_t(k.id.val) << 32) ^ size_t(k.ver);
  }
};

struct TACValue
{
  std::variant<bool,
               double,
               LiteralID,
               ValueID,
               SSAIdentityState,
               VarID,
               FunctionID,
               BlockID>
    id;
  ValueType type = ValueType::SSAValue;
  ValueSource src = ValueSource::Computed;
};

struct Instruction
{
  OpCode op;
  // if opcode is phi, then operands stored as pairs of two tacvalues - first is
  // blockid for predecessor, and second is incoming value
  std::vector<TACValue> operands;
  ValueID res = {};
  struct Block* owner;
};

struct InstructionResult
{
  ValueID id = {};
  TACValue val = {};
};

struct Block
{
  BlockID id = {};
  std::vector<Instruction*> instructions;
};

struct ValueIDCounter
{
public:
  inline ValueID inc()
  {
    ValueID tmp = val;
    ++val;
    return tmp;
  }

  inline void reset() { val = { 0 }; }

private:
  ValueID val = { 0 };
};

struct MemberAccessContext
{
  TACValue base = {};
  bool place = false;
  bool enabled = false;
  bool ssaImmediate = false;
};

struct BranchContext
{
  Block* current;
  Block* alt;
};

struct PhiFunction
{
  SSAIdentityID id;
  SSAVersion ver;
  std::vector<SSAVersion> choices;
  ValueID res;
};

using IncomingVersionsMap =
  std::unordered_map<SSAIdentityID, std::vector<SSAVersion>>;

struct BlockContext
{
  std::unordered_map<SSAIdentityID, std::vector<Instruction*>> usedIdentities;
  IncomingVersionsMap inVersions;
  std::unordered_map<SSAIdentityID, SSAVersion> usedVersions;
  std::unordered_map<SSAIdentityID, SSAVersion> outVersions;
  std::unordered_map<SSAIdentityID, PhiFunction*> phiFunctions;
  std::unordered_set<SSAIdentityID> usedDefinitions;
  std::unordered_set<SSAIdentityID> propagatedIdentities;
  std::vector<Block*> outBlocks;
};

struct Optimized
{};
struct Unoptimized
{};

template<typename Tag>
struct TACBlocks
{
  Block* begin;
  std::vector<Block*>* blocks;
};

class LayoutRegistry;
class SSAState;
class TACBuilder;
class TACOptimizer;

class TAC
{
public:
  TAC(Diagnostics& diag, SymbolRegistry& symReg);
  ~TAC();

  TACBlocks<Unoptimized> build(HIR::TypedTree<HIR::Analyzed> root);

  // TODO: add null checks for blocks
  TACBlocks<Optimized> optimize(TACBlocks<Unoptimized> blocks);

  void print(std::vector<Block*>& blocks);

private:
  TAC(const TAC& other) = delete;
  TAC(TAC&& other) = delete;

  Diagnostics& _diag;
  SymbolRegistry& _symReg;
  ArenaAlloc _arena;

  std::unique_ptr<LayoutRegistry> _layReg;
  std::unique_ptr<SSAState> _ssa;

  std::unique_ptr<TACBuilder> _builder;
  std::unique_ptr<TACOptimizer> _optimizer;

  void printInstruction(Instruction& instr);
  const char* getOpCodeStr(OpCode op);
  std::string tacValueToStr(TACValue val);
};

};