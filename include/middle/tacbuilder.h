#pragma once
#include "alloc/arena.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include <vector>

namespace MIR {
enum class OpCode : uint16_t
{
  // get operand without deref
  Move,

  // deref operand and get value
  Load,

  // get by val for primitives, and by ref for non-primitives
  DynRead,

  Store,
  Jmp,
  Call,
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

enum class ValueType
{
  // immediates
  ImmediateBool,
  ImmediateDouble,

  // string literal
  Literal,

  // value, produced by instruction
  SSAValue,

  // id for block of instructions
  Block,

  Place,
  StaticFn,
  Self,
};

enum class ValueSource
{
  Immediate,
  Literal,
  Computed,
  Static,
  Dynamic,
  Self,
};

struct TACValue
{
  std::variant<ValueID, VarID, FunctionID, BlockID, LiteralID, bool, double> id;
  ValueType type = ValueType::SSAValue;
  ValueSource src = ValueSource::Computed;
};

struct Instruction
{
  OpCode op;
  std::vector<TACValue> operands;
  ValueID res = {};
};

struct InstructionResult
{
  ValueID id = {};
  TACValue val = {};
};

struct Block
{
  BlockID id = {};
  std::vector<Instruction> instructions;
  Block* next = nullptr;
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

private:
  ValueID val = { 0 };
};

struct MemberAccessContext
{
  TACValue base = {};
  bool place = false;
  bool enabled = false;
};

struct BranchContext
{
  Block* current;
  Block* alt;
};

class LayoutRegistry;
class SSAState;

class TACBuilder
{
public:
  TACBuilder(Diagnostics& diag);
  ~TACBuilder();

  inline Block* build(HIR::TypedTree<HIR::Analyzed> root)
  {
    // TODO: fix memleaks related to _last
    Block* begin = allocBlock();
    pushBranchCtx(begin);
    buildFromAST(root.root);
    popBranchCtx();

    return begin;
  }

  void print();

private:
  TACBuilder(const TACBuilder& other) = delete;
  TACBuilder(TACBuilder&& other) = delete;

  Diagnostics& _diag;
  ArenaAlloc _arena;

  // currently active branches
  std::vector<BranchContext> _branches;

  std::unique_ptr<LayoutRegistry> _layReg;
  std::unique_ptr<SSAState> _ssa;

  std::vector<Block*> _blocks;

  BlockID _blockID = { 0 };

  inline void addInstruction(Instruction i)
  {
    _branches.back().current->instructions.push_back(i);
  }

  inline BranchContext currentBranchCtx() { return _branches.back(); }

  inline void pushBranchCtx(Block* current, Block* alt = nullptr)
  {
    _branches.push_back({ current, alt });
  }

  inline void popBranchCtx() { _branches.pop_back(); }

  InstructionResult addAccessInstruction(TACValue expr,
                                         MemberAccessContext ctx);

  Block* allocBlock();

  void buildFromAST(const HIR::TypedNode* node);

  InstructionResult buildNodeList(const HIR::NodeList& list);
  InstructionResult buildFnDef(const HIR::FnDef& fnDef);
  InstructionResult buildClassDef(const HIR::ClassDef& classDef);

  InstructionResult buildVarDecl(const HIR::VarDecl& varDecl);
  InstructionResult buildVarAssign(const HIR::VarAssign& varAssign);
  InstructionResult buildLoopWhl(const HIR::LoopWhl& loopWhl);
  InstructionResult buildLoopFor(const HIR::LoopFor& loopFor);
  InstructionResult buildIf(const HIR::StmtIf& stmtIf);
  InstructionResult buildSwitch(const HIR::StmtSwitch& stmtSwitch);
  InstructionResult buildRet(const HIR::StmtRet& stmtRet);
  InstructionResult buildBrk(const HIR::StmtBrk& stmtBrk);

  InstructionResult buildPlace(const HIR::TypedNode* expr,
                               MemberAccessContext ctx = {});

  // value only
  InstructionResult buildCallExpr(const HIR::CallExpr& callExpr);
  InstructionResult buildBinary(const HIR::BinaryExpr& binary);
  InstructionResult buildUnary(const HIR::UnaryExpr& unary);
  InstructionResult buildBool(const HIR::BoolVal& val);
  InstructionResult buildNumber(const HIR::NumVal& val);
  InstructionResult buildString(const HIR::StringVal& val);
  InstructionResult buildArray(const HIR::ArrayVal& val);

  // value / ref
  InstructionResult buildMemberAccess(const HIR::MemberAccess& membAccess,
                                      MemberAccessContext ctx = {});
  InstructionResult buildArrayAccess(const HIR::ArrayAccess& arrAccess,
                                     MemberAccessContext ctx = {});
  InstructionResult buildName(const HIR::NameExpr& name,
                              MemberAccessContext ctx = {});
  InstructionResult buildSelf(const HIR::SelfExpr& self,
                              MemberAccessContext ctx);

  InstructionResult buildCallArgument(const HIR::TypedNode* arg);

  void printInstruction(Instruction& instr);

  const char* getOpCodeStr(OpCode op);

  std::string tacValueToStr(TACValue val);
};

};