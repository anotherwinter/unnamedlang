#include "string.h"
#include "../runtime.h"
#include <cstring>
#include <vector>

void
stringConcat(std::vector<Object*>* args)
{
  OperatorType opType =
    (OperatorType)((Number*)(args->at(0)->data.objectPtr))->value;
  const char* strA = ((String*)(args->at(1)->data.objectPtr))->value;
  const char* strB = ((String*)(args->at(2)->data.objectPtr))->value;

  if (opType != OperatorType::BINARY_ADD) {
    // if operator passed to function is not a binary op defined for number
    setError(false, "stringConcat: Wrong operator\n");
  }

  int lenA = strlen(strA);
  int lenB = strlen(strB);
  char* strCat = (char*)malloc(lenA + lenB + 1);

  memcpy(strCat, strA, lenA);
  memcpy(strCat + lenA, strB, lenB);

  Object* obj = stringObj(strCat, false);
  contextSetLastEval(obj);
}

void
stringClassInit()
{
  // init operators
  Object* bnStringConcat = builtinFnObj(
    { { "op", "Number" }, { "strA", "String" }, { "strB", "String" } },
    stringConcat);

  Modifier opMod = (Modifier)(MOD_PUBLIC | MOD_STATIC);
  ClassMember builtinStringConcat = { "binary", bnStringConcat, opMod };

  registerClass("String", true, { builtinStringConcat });
}