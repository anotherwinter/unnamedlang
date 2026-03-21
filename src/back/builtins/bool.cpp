#include "bool.h"
#include "../../front/ast.h"
#include "../runtime.h"

ObjectHeader
boolBinaryOp(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  double opVal = args.at(0).data.numValue;
  OpType op = runtime.doubleToOpType(opVal);
  bool a = args.at(1).data.boolValue;
  bool b = args.at(2).data.boolValue;
  Identifier id;

  switch (op) {
    case OP_BINARY_LOGIC_AND: {
      id = runtime.boolObj(a && b);
      break;
    }
    case OP_BINARY_LOGIC_OR: {
      id = runtime.boolObj(a || b);
      break;
    }
    case OP_BINARY_EQ: {
      id = runtime.boolObj(a == b);
      break;
    }
    default: {
      // if operator passed to function is not a binary op defined for number
      runtime.error("numberBinaryOp: Wrong operator\n");
      return runtime.invalidObjectHeader();
    }
  }

  return std::get<ObjectHeader>(id.name);
}

ObjectHeader
boolUnaryOp(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  double opVal = args.at(0).data.numValue;
  OpType op = runtime.doubleToOpType(opVal);
  bool a = args.at(1).data.boolValue;
  ObjectHeader hdr = runtime.invalidObjectHeader();

  if (op == OP_UNARY_NEG)
    hdr = std::get<ObjectHeader>(runtime.boolObj(!a).name);
  else
    runtime.error("boolUnaryOp: Unsupported operator %s\n", op2String(op));

  return hdr;
}

void
boolClassInit(Runtime& runtime)
{
  runtime.declareClass("Bool");

  // init operators
  std::vector<const char*> bnBinaryParamNames = { "op", "a", "b" };
  std::vector<TypeID> bnBinaryParamTypes = { runtime.findClass("Number"),
                                             runtime.findClass("Bool"),
                                             runtime.findClass("Bool") };

  std::vector<const char*> bnUnaryParamNames = { "op", "a" };
  std::vector<TypeID> bnUnaryParamTypes = { runtime.findClass("Number"),
                                            runtime.findClass("Bool") };

  Modifier opMod = Modifier::None;
  std::vector<ClassMethod> methods;
  methods.push_back({ opMod,
                      "binary",
                      true,
                      &bnBinaryParamNames,
                      &bnBinaryParamTypes,
                      nullptr,
                      boolBinaryOp });
  methods.push_back({ opMod,
                      "unary",
                      true,
                      &bnUnaryParamNames,
                      &bnUnaryParamTypes,
                      nullptr,
                      boolUnaryOp });

  runtime.defineClass("Bool", true, true, methods, {}, {});
}