#include "foldfunctions.h"
#include "middle/tac.h"

using namespace MIR;

TACValue
foldBoolBool(OpCode op, const TACValue* lhs, const TACValue* rhs)
{
  bool lhsVal = std::get<bool>(lhs->id);
  bool rhsVal = std::get<bool>(rhs->id);
  TACValue val;
  val.type = ValueType::ImmediateBool;
  val.src = ValueSource::Immediate;
  switch (op) {
    case MIR::OpCode::Lt: {
      val.id = lhsVal < rhsVal;
      break;
    }
    case MIR::OpCode::Gt: {
      val.id = lhsVal > rhsVal;
      break;
    }
    case MIR::OpCode::Leq: {
      val.id = lhsVal <= rhsVal;
      break;
    }
    case MIR::OpCode::Geq: {
      val.id = lhsVal >= rhsVal;
      break;
    }
    case MIR::OpCode::Eq: {
      val.id = lhsVal == rhsVal;
      break;
    }
    case MIR::OpCode::Neq: {
      val.id = lhsVal != rhsVal;
      break;
    }
    case MIR::OpCode::LogicAnd: {
      val.id = lhsVal && rhsVal;
      break;
    }
    case MIR::OpCode::LogicOr: {
      val.id = lhsVal || rhsVal;
      break;
    }
    case MIR::OpCode::LogicNeg: {
      val.id = !lhsVal;
      break;
    }
    default:
      return {};
  }

  return val;
}

TACValue
foldNumberNumber(OpCode op, const TACValue* lhs, const TACValue* rhs)
{
  double lhsVal = std::get<double>(lhs->id);
  double rhsVal = std::get<double>(rhs->id);
  TACValue val;
  val.type = ValueType::ImmediateDouble;
  val.src = ValueSource::Immediate;
  switch (op) {
    case MIR::OpCode::Add: {
      val.id = lhsVal + rhsVal;
      break;
    }
    case MIR::OpCode::Sub: {
      val.id = lhsVal - rhsVal;
      break;
    }
    case MIR::OpCode::Mul: {
      val.id = lhsVal * rhsVal;
      break;
    }
    case MIR::OpCode::Div: {
      val.id = lhsVal / rhsVal;
      break;
    }
    case MIR::OpCode::Mod: {
      val.id = (double)((uint64_t)lhsVal % (uint64_t)rhsVal);
      break;
    }
    case MIR::OpCode::Shiftl: {
      val.id = (double)((uint64_t)lhsVal << (uint64_t)rhsVal);
      break;
    }
    case MIR::OpCode::Shiftr: {
      val.id = (double)((uint64_t)lhsVal >> (uint64_t)rhsVal);
      break;
    }
    case MIR::OpCode::And: {
      val.id = (double)((uint64_t)lhsVal & (uint64_t)rhsVal);
      break;
    }
    case MIR::OpCode::Or: {
      val.id = (double)((uint64_t)lhsVal | (uint64_t)rhsVal);
      break;
    }
    case MIR::OpCode::Xor: {
      val.id = (double)((uint64_t)lhsVal ^ (uint64_t)rhsVal);
      break;
    }
    case MIR::OpCode::Lt: {
      val.id = lhsVal < rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::Gt: {
      val.id = lhsVal > rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::Leq: {
      val.id = lhsVal <= rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::Geq: {
      val.id = lhsVal >= rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::Eq: {
      val.id = lhsVal == rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::Neq: {
      val.id = lhsVal != rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::LogicAnd: {
      val.id = lhsVal && rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::LogicOr: {
      val.id = lhsVal || rhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::LogicNeg: {
      val.id = !lhsVal;
      val.type = ValueType::ImmediateBool;
      break;
    }
    case MIR::OpCode::ArithmNeg: {
      val.id = -lhsVal;
      break;
    }
    default:
      return {};
  }

  return val;
}