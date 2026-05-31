#include "middle/tac.h"
#include "front/symbolregistry.h"
#include "middle/ssastate.h"
#include "middle/tacbuilder.h"
#include "middle/tacoptimizer.h"

using namespace MIR;

TAC::TAC(Diagnostics& diag, SymbolRegistry& symReg)
  : _diag(diag)
  , _symReg(symReg)
{
  _layReg = std::make_unique<LayoutRegistry>();
  _ssa = std::make_unique<SSAState>();
  _builder =
    std::make_unique<TACBuilder>(diag, symReg, *_layReg.get(), *_ssa.get());
  _optimizer = std::make_unique<TACOptimizer>();
}

TAC::~TAC() {}

TACBlocks<Unoptimized>
TAC::build(HIR::TypedTree<HIR::Analyzed> root)
{
  _layReg->reset();
  _ssa->reset();

  return _builder->build(root);
}

TACBlocks<Optimized>
TAC::optimize(TACBlocks<Unoptimized> blocks)
{
  return _optimizer->optimize(blocks);
}

void
TAC::print(std::vector<Block*>& blocks)
{
  for (auto& b : blocks) {
    printf("\nblock %u:\n", b->id.val);
    for (auto& i : b->instructions)
      printInstruction(*i);
  }
}

void
TAC::printInstruction(Instruction& instr)
{
  std::string str;
  if (instr.op != OpCode::Dead) {
    str = isValid(instr.res) ? "@" + std::to_string(instr.res.val) + " = " : "";
    str += getOpCodeStr(instr.op);
    if (!instr.operands.empty()) {
      str += " " + tacValueToStr(instr.operands.front());

      for (size_t i = 1; i < instr.operands.size(); ++i)
        str += ", " + tacValueToStr(instr.operands[i]);
    }
  } else {
    str = getOpCodeStr(instr.op);
  }

  printf("%s\n", str.c_str());
}

const char*
TAC::getOpCodeStr(OpCode op)
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
    case OpCode::Exit:
      return "exit";
    case OpCode::Phi:
      return "phi";
    case OpCode::Dead:
      return "dead";
    default:
      return "unknown";
  }
}

std::string
TAC::tacValueToStr(TACValue val)
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
    if (isValid(*blockID))
      str += "blk" + std::to_string(blockID->val);
    else
      str += "blkInv";
  }

  return str;
}