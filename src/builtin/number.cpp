#include "number.h"
#include "../runtime.h"
#include <cmath>

void
numberBinaryOp(std::vector<Object*>* args)
{
  OperatorType opType =
    (OperatorType)((Number*)(args->at(0)->data.objectPtr))->value;
  double a = ((Number*)(args->at(1)->data.objectPtr))->value;
  double b = ((Number*)(args->at(2)->data.objectPtr))->value;
  Object* obj = nullptr;
  switch (opType) {
    case OperatorType::BINARY_MUL: {
      obj = numberObj(a * b);
      break;
    }
    case OperatorType::BINARY_DIV: {
      obj = numberObj(a / b);
      break;
    }
    case OperatorType::BINARY_MOD: {
      obj = numberObj(fmod(a, b));
      break;
    }
    case OperatorType::BINARY_ADD: {
      obj = numberObj(a + b);
      break;
    }
    case OperatorType::BINARY_SUB: {
      obj = numberObj(a - b);
      break;
    }
    case OperatorType::BINARY_SHIFTL: {
      obj = numberObj((int)a << (int)b);
      break;
    }
    case OperatorType::BINARY_SHIFTR: {
      obj = numberObj((int)a >> (int)b);
      break;
    }
    case OperatorType::BINARY_AND: {
      obj = numberObj((int)a & (int)b);
      break;
    }
    case OperatorType::BINARY_OR: {
      obj = numberObj((int)a | (int)b);
      break;
    }
    case OperatorType::BINARY_XOR: {
      obj = numberObj((int)a ^ (int)b);
      break;
    }
    case OperatorType::BINARY_LT: {
      obj = boolObj(a < b);
      break;
    }
    case OperatorType::BINARY_GT: {
      obj = boolObj(a > b);
      break;
    }
    case OperatorType::BINARY_LEQ: {
      obj = boolObj(a <= b);
      break;
    }
    case OperatorType::BINARY_GEQ: {
      obj = boolObj(a >= b);
      break;
    }
    case OperatorType::BINARY_EQ: {
      obj = boolObj(a == b);
      break;
    }
    case OperatorType::BINARY_NEQ: {
      obj = boolObj(a != b);
      break;
    }
    case OperatorType::BINARY_LOGIC_AND: {
      obj = boolObj(a && b);
      break;
    }
    case OperatorType::BINARY_LOGIC_OR: {
      obj = boolObj(a || b);
      break;
    }
    case OperatorType::EQUALS: {
      obj = boolObj(a == b);
      break;
    }
    default: {
      // if operator passed to function is not a binary op defined for number
      setError(false, "numberBinaryOp: Wrong operator\n");
      return;
    }
  }

  contextSetLastEval(obj);
}

void
numberUnaryOp(std::vector<Object*>* args)
{
  OperatorType opType =
    (OperatorType)((Number*)(args->at(0)->data.objectPtr))->value;
  double* a = &((Number*)(args->at(1)->data.objectPtr))->value;
  Object* obj = nullptr;
  switch (opType) {
    case OperatorType::UNARY_INC: {
      ++(*a);
      obj = args->at(1);
      break;
    }
    case OperatorType::UNARY_DEC: {
      --(*a);
      obj = args->at(1);
      break;
    }
    case OperatorType::UNARY_NEG: {
      obj = numberObj(~(int)(*a));
      break;
    }
    case OperatorType::UNARY_ARITHM_NEG: {
      obj = numberObj(-(*a));
      break;
    }
    default: {
      // if operator passed to function is not a binary op defined for number
      setError(false, "numberUnaryOp: Wrong operator\n");
    }
  }

  contextSetLastEval(obj);
}

void
numberClassInit()
{
  // init operators
  Object* bnBinary =
    builtinFnObj({ { "op", "Number" }, { "a", "Number" }, { "b", "Number" } },
                 numberBinaryOp);
  Object* bnUnary =
    builtinFnObj({ { "op", "Number" }, { "a", "Number" } }, numberUnaryOp);

  Modifier opMod = (Modifier)(MOD_PUBLIC | MOD_STATIC);
  ClassMember builtinBinary = { "binary", bnBinary, opMod };
  ClassMember builtinUnary = { "unary", bnUnary, opMod };

  registerClass("Number", true, { builtinBinary, builtinUnary });
}