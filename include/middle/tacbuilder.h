#pragma once
#include "alloc/arena.h"
#include "front/symbolregistry.h"
#include "front/typedast.h"
#include "middle/layoutregistry.h"
#include "middle/tac.h"

namespace MIR {

class TACBuilder
{
public:
  TACBuilder(Diagnostics& diag,
             SymbolRegistry& symReg,
             LayoutRegistry& layReg,
             SSAState& ssa);
  ~TACBuilder();

  TACBlocks<Unoptimized> build(HIR::TypedTree<HIR::Analyzed> root);

private:
  TACBuilder(const TACBuilder& other) = delete;
  TACBuilder(TACBuilder&& other) = delete;

  Diagnostics& _diag;
  SymbolRegistry& _symReg;
  ArenaAlloc _arena;

  LayoutRegistry& _layReg;
  SSAState& _ssa;

  // currently active branches
  std::vector<BranchContext> _branches;

  std::vector<Block*> _blocks;

  BlockID _blockID = { 0 };

  std::unordered_map<BlockID::__BlockID, BlockContext> _ssaBlocksMap;
  std::unordered_map<SSAIdentityID, std::unordered_map<SSAVersion, PhiFunction>>
    _phiFunctions;

  inline void addInstruction(Instruction i)
  {
    i.owner = _branches.back().current;
    _branches.back().current->instructions.push_back(allocInstruction(i));
  }

  inline BranchContext currentBranchCtx() { return _branches.back(); }

  void pushBranchCtx(Block* current, Block* alt = nullptr);

  void popBranchCtx();

  InstructionResult addAccessInstruction(TACValue expr,
                                         MemberAccessContext ctx);

  Block* allocBlock();

  Instruction* allocInstruction(Instruction i);

  // pass1 - build TAC with SSA symbols
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

  void injectParameter(VarID id, const char* name, TACValue val);

  // pass2 - SSA construction
  void resolveSSA();

  void resolveIdentities();

  void propagateVersions(BlockContext& ctx, std::vector<Block*>& worklist);

  void ensurePhi(BlockContext& ctx,
                 SSAIdentityID id,
                 std::vector<SSAVersion>& versions);

  void makePhi(BlockContext& ctx,
               SSAIdentityID id,
               const std::vector<SSAVersion>& versions);

  std::unordered_set<SSAVersion> unfoldVersions(
    SSAIdentityID id,
    const std::vector<SSAVersion>& versions);

  void insertPhi(Block* b, PhiFunction& phi);

  void printInstruction(Instruction& instr);
  const char* getOpCodeStr(OpCode op);
  std::string tacValueToStr(TACValue val);
};

};