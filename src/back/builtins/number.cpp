#include "number.h"
#include "../../front/ast.h"
#include "../runtime.h"
#include <cmath>

ObjectHeader
numberCtor(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  double num = args.at(0).data.numValue;
  return std::get<ObjectHeader>(runtime.numberObj(num).name);
}

ObjectHeader
numberBinaryOp(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  double opVal = args.at(0).data.numValue;
  OpType op = runtime.doubleToOpType(opVal);
  double a = args.at(1).data.numValue;
  double b = args.at(2).data.numValue;
  Identifier id;

  switch (op) {
    case OP_BINARY_MUL: {
      id = runtime.numberObj(a * b);
      break;
    }
    case OP_BINARY_DIV: {
      id = runtime.numberObj(a / b);
      break;
    }
    case OP_BINARY_MOD: {
      id = runtime.numberObj(std::fmod(a, b));
      break;
    }
    case OP_BINARY_ADD: {
      id = runtime.numberObj(a + b);
      break;
    }
    case OP_BINARY_SUB: {
      id = runtime.numberObj(a - b);
      break;
    }
    case OP_BINARY_SHIFTL: {
      id =
        runtime.numberObj(static_cast<int64_t>(a) << static_cast<int64_t>(b));
      break;
    }
    case OP_BINARY_SHIFTR: {
      id =
        runtime.numberObj(static_cast<int64_t>(a) >> static_cast<int64_t>(b));
      break;
    }
    case OP_BINARY_AND: {
      id = runtime.numberObj(static_cast<int64_t>(a) & static_cast<int64_t>(b));
      break;
    }
    case OP_BINARY_OR: {
      id = runtime.numberObj(static_cast<int64_t>(a) | static_cast<int64_t>(b));
      break;
    }
    case OP_BINARY_XOR: {
      id = runtime.numberObj(static_cast<int64_t>(a) ^ static_cast<int64_t>(b));
      break;
    }
    case OP_BINARY_LT: {
      id = runtime.boolObj(a < b);
      break;
    }
    case OP_BINARY_GT: {
      id = runtime.boolObj(a > b);
      break;
    }
    case OP_BINARY_LEQ: {
      id = runtime.boolObj(a <= b);
      break;
    }
    case OP_BINARY_GEQ: {
      id = runtime.boolObj(a >= b);
      break;
    }
    case OP_BINARY_EQ: {
      id = runtime.boolObj(a == b);
      break;
    }
    case OP_BINARY_NEQ: {
      id = runtime.boolObj(a != b);
      break;
    }
    case OP_BINARY_LOGIC_AND: {
      id = runtime.boolObj(a && b);
      break;
    }
    case OP_BINARY_LOGIC_OR: {
      id = runtime.boolObj(a || b);
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

// ObjectHeader
// numberUnaryOp(Runtime& runtime, const std::vector<ObjectHeader>& args)
// {
//   double opVal = args.at(0).data.numValue;
//   OpType op = runtime.doubleToOpType(opVal);
//   double a = args.at(1).data.numValue;
//   Identifier id = { NameType::None };

//   switch (op) {
//     case OP_UNARY_INC: {
//       id = runtime.args->at(1);
//       ++obj->data.numValue;
//       break;
//     }
//     case OP_UNARY_DEC: {
//       id = runtime.args->at(1);
//       --obj->data.numValue;
//       break;
//     }
//     case OP_UNARY_NEG: {
//       id = runtime.boolObj(~static_cast<int64_t>(a));
//       break;
//     }
//     case OP_UNARY_ARITHM_NEG: {
//       id = runtime.numberObj(-a);
//       break;
//     }
//     default: {
//       // if operator passed to function is not a binary op defined for number
//       runtime.error("numberUnaryOp: Wrong operator\n");
//     }
//   }

//   contextSetLastEval({ NameType::Object, nullptr, obj });
// }

void
numberClassInit(Runtime& runtime)
{
  runtime.declareClass("Number");

  std::vector<const char*> bnCtorParamNames = { "a" };
  std::vector<TypeID> bnCtorParamTypes = { runtime.findClass("Number") };

  std::vector<const char*> bnBinaryParamNames = { "op", "a", "b" };
  std::vector<TypeID> bnBinaryParamTypes = { runtime.findClass("Number"),
                                             runtime.findClass("Number"),
                                             runtime.findClass("Number") };

  Modifier opMod = Modifier::None;
  Modifier ctorMod = Modifier::Static;

  std::vector<ClassMethod> methods;
  methods.push_back({ opMod,
                      "Number",
                      true,
                      &bnCtorParamNames,
                      &bnCtorParamTypes,
                      nullptr,
                      numberCtor });
  methods.push_back({ opMod,
                      "binary",
                      true,
                      &bnBinaryParamNames,
                      &bnBinaryParamTypes,
                      nullptr,
                      numberBinaryOp });

  runtime.defineClass("Number", true, true, methods, {}, {});
}