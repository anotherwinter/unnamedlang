#include "array.h"
#include "../runtime.h"

void
arrayCtor(std::vector<Object*>* args)
{
  double count = ((Number*)(args->at(0)->data.objectPtr))->value;
  Object* instance = contextGetCurrentInstance();
  instance->data.objectPtr = allocOnArena(sizeof(Array));

  Array* arr = (Array*)instance->data.objectPtr;
  arr->objects = new std::vector<Object*>(count);
  arr->objects->resize(count);

  contextSetLastEval(nullObj());
}

void
arrayClassInit()
{
  Object* bnCtor = builtinFnObj({ { "count", "Number" } }, arrayCtor);

  Modifier ctorMod = (Modifier)(MOD_PUBLIC | MOD_STATIC);
  ClassMember builtinBinary = { "ctor", bnCtor, ctorMod };

  registerClass("Array", true, { builtinBinary });
}