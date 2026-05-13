#include "string.h"
#include "../../front/ast.h"
#include "../runtime.h"
#include <cstring>
#include <vector>

ObjectHeader
stringConcat(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  double opVal = args.at(0).data.numValue;
  OpType op = runtime.doubleToOpType(opVal);
  const char* strA = args.at(1).data.stringValue;
  const char* strB = args.at(2).data.stringValue;

  if (op != OP_BINARY_ADD) {
    // if operator passed to function is not a binary op defined for number
    runtime.error("stringConcat: Wrong operator\n");
    return runtime.invalidObjectHeader();
  }

  int lenA = strlen(strA);
  int lenB = strlen(strB);
  char* strCat = (char*)malloc(lenA + lenB + 1);

  memcpy(strCat, strA, lenA);
  memcpy(strCat + lenA, strB, lenB);

  Identifier id = runtime.stringObj(strCat, false);
  free(strCat);

  return std::get<ObjectHeader>(id.name);
}

void
stringClassInit(Runtime& runtime)
{
  runtime.declareClass("String");

  // init operators
  std::vector<const char*> bnConcatParamNames = { "op", "strA", "strB" };
  std::vector<TypeID> bnConcatParamTypes = { runtime.findClass("Number"),
                                             runtime.findClass("String"),
                                             runtime.findClass("String") };

  Modifier opMod = Modifier::None;

  std::vector<ClassMethod> methods;
  methods.push_back({ opMod,
                      "binary",
                      true,
                      &bnConcatParamNames,
                      &bnConcatParamTypes,
                      nullptr,
                      stringConcat });

  // TODO: implement static fields, and fix this for each builtin
  runtime.defineClass("String", false, true, methods, {}, {});
}