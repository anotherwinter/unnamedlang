#pragma once
#include "alloc/arena.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include <vector>

namespace MIR {
enum class OpCode : uint16_t
{
  Store,
  RelOp,
  Load,
  LoadRef,
  LoadSubs,
  LoadSubsRef,
  Jmp,
  Call,
  DynCall,
  If,
  Else,
  IfRel,
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
  Neg,
  Inc,
  Dec,
  Get,
  GetRef,
  Set,
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

struct TACValue
{
  std::variant<ValueID, VarID, FnResolution, BlockID, LiteralID> id;
};

struct Instruction
{
  OpCode op;
  std::vector<TACValue> operands;
  ValueID res = {};
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

class LayoutRegistry;
class SSAState;

class TACBuilder
{
public:
  TACBuilder(Diagnostics& diag, LayoutRegistry& layReg);

  inline Block* build(HIR::TypedTree<HIR::Analyzed> root)
  {
    // TODO: fix memleaks related to _last
    _last = allocBlock();
    buildFromAST(root.root);

    return _last;
  }

private:
  TACBuilder(const TACBuilder& other) = delete;
  TACBuilder(TACBuilder&& other) = delete;

  Diagnostics& _diag;
  ArenaAlloc _arena;
  Block* _last;
  std::unique_ptr<SSAState> _ssa;

  Block* allocBlock();

  ValueID buildFromAST(const HIR::TypedNode* node);

  ValueID buildNodeList(const HIR::NodeList& list);
  ValueID buildCallExpr(const HIR::CallExpr& callExpr);
  ValueID buildVarDecl(const HIR::VarDecl& varDecl);
  ValueID buildVarAssign(const HIR::VarAssign& varAssign);
  ValueID buildLoopWhl(const HIR::LoopWhl& loopWhl);
  ValueID buildLoopFor(const HIR::LoopFor& loopFor);
  ValueID buildIf(const HIR::StmtIf& stmtIf);
  ValueID buildSwitch(const HIR::StmtSwitch& stmtSwitch);
  ValueID buildRet(const HIR::StmtRet& stmtRet);
  ValueID buildBrk(const HIR::StmtBrk& stmtBrk);
  ValueID buildBinary(const HIR::BinaryExpr& binary);
  ValueID buildUnary(const HIR::UnaryExpr& unary);
  ValueID buildMemberAccess(const HIR::MemberAccess& membAccess);
  ValueID buildArrayAccess(const HIR::ArrayAccess& arrAccess);
  ValueID buildBool(const HIR::BoolVal& val);
  ValueID buildNumber(const HIR::NumVal& val);
  ValueID buildString(const HIR::StringVal& val);
  ValueID buildArray(const HIR::ArrayVal& val);
  ValueID buildName(const HIR::NameExpr& name);

  ValueID buildExpr(const HIR::TypedNode* expr, bool ref = false);
  ValueID buildMemberAccess(const HIR::MemberAccess& membAccess,
                            bool ref = false);
  ValueID buildArrayAccess(const HIR::ArrayAccess& arrAccess, bool ref = false);
  ValueID buildName(const HIR::NameExpr& name, bool ref = false);

  ValueID buildCallArgument(const HIR::TypedNode* arg);
};

};