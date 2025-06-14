#include "builtin_fns.h"
#include "runtime.h"
#include "types.h"
#include <cstdio>

void
print(std::vector<Object*>* args)
{
  Object* obj = args->at(0);
  printObject(obj);

  contextSetLastEval(nullObj());
}

void
strict(std::vector<Object*>* args)
{
  Object* obj = args->at(0);
  bool strict = ((Bool*)obj->data.objectPtr)->value;
  if (strict)
    printf("Strict mode is enabled\n");
  else
    printf("Strict mode is disabled\n");

  runtimeSetStrictMode((strict));
  contextSetLastEval(nullObj());
}

void
typeofFunction(std::vector<Object*>* args)
{
  Object* obj = args->at(0);
  printf("%s\n", obj->type);
  contextSetLastEval(nullObj());
}

void
builtinFunctionsInit()
{
  Object* bnPrint = builtinFnObj({ { "expr", "var" } }, print);
  Object* bnStrict = builtinFnObj({ { "enable", "Bool" } }, strict);
  Object* bnTypeof = builtinFnObj({ { "expr", "var" } }, typeofFunction);

  registerFunction("print", bnPrint);
  registerFunction("strict", bnStrict);
  registerFunction("typeof", bnTypeof);
}