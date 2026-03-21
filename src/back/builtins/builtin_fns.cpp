#include "builtin_fns.h"
#include "../runtime.h"
#include <cstdio>

ObjectHeader
print(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  ObjectHeader obj = args.at(0);
  runtime.printObject(obj);

  return runtime.invalidObjectHeader();
}

ObjectHeader
typeofFunction(Runtime& runtime, const std::vector<ObjectHeader>& args)
{
  ObjectHeader obj = args.at(0);
  printf("%s\n", runtime.getClassInfo(obj.objClass)->name);

  return runtime.invalidObjectHeader();
}

void
builtinFunctionsInit(Runtime& runtime)
{
  std::vector<const char*> bnPrintParamNames = { "expr" };
  std::vector<TypeID> bnPrintParamTypes = { runtime.findClass("var") };

  std::vector<const char*> bnTypeofParamNames = { "expr" };
  std::vector<TypeID> bnTypeofParamTypes = { runtime.findClass("var") };

  runtime.defineFunction(
    "print", true, bnPrintParamNames, bnPrintParamTypes, nullptr, print);
  runtime.defineFunction("typeof",
                         true,
                         bnTypeofParamNames,
                         bnTypeofParamTypes,
                         nullptr,
                         typeofFunction);
}