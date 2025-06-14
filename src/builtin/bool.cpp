#include "bool.h"
#include "../runtime.h"

void
boolBinaryOp(std::vector<Object*>* args)
{
  OperatorType opType =
    (OperatorType)((Number*)(args->at(0)->data.objectPtr))->value;
  bool a = ((Bool*)(args->at(1)->data.objectPtr))->value;
  bool b = ((Bool*)(args->at(2)->data.objectPtr))->value;
  Object* obj = nullptr;

  switch (opType) {
    case OperatorType::BINARY_LOGIC_AND: {
      obj = boolObj(a && b);
      break;
    }
    case OperatorType::BINARY_LOGIC_OR: {
      obj = boolObj(a || b);
      break;
    }
    case OperatorType::BINARY_EQ: {
      obj = boolObj(a == b);
      break;
    }
    default: {
      // if operator passed to function is not a binary op defined for number
      setError(false, "numberBinaryOp: Wrong operator\n");
    }
  }

  contextSetLastEval(obj);
}

void
boolUnaryOp(std::vector<Object*>* args)
{
  OperatorType opType =
    (OperatorType)((Number*)(args->at(0)->data.objectPtr))->value;
  bool a = ((Bool*)(args->at(1)->data.objectPtr))->value;

  if (opType == OperatorType::UNARY_NEG)
    contextSetLastEval(boolObj(!a));
  else
    setError(false, "boolUnaryOp: Unsupported operator %s\n", op2String);
}

void
boolClassInit()
{
  Object* bnBoolUnary =
    builtinFnObj({ { "op", "Number" }, { "a", "Bool" } }, boolUnaryOp);
  Object* bnBoolBinary = builtinFnObj(
    { { "op", "Number" }, { "a", "Bool" }, { "b", "Bool" } }, boolBinaryOp);

  Modifier opMod = (Modifier)(MOD_PUBLIC | MOD_STATIC);
  ClassMember builtinUnary = { "unary", bnBoolUnary, opMod };
  ClassMember builtinBinary = { "binary", bnBoolBinary, opMod };

  registerClass("Bool", true, { builtinUnary, builtinBinary });
}