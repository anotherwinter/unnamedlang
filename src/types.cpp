#include "types.h"
#include "ast.h"
#include "runtime.h"
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

static Object* nullSingleton = nullptr;

static Object*
allocObj()
{
  Object* obj = (Object*)allocOnArena(sizeof(Object));
  if (!obj)
    setError(true, "allocObj: Failed to alloc object\n");

  obj->refcount = 0;
  addTemporaryObject(obj);

  return obj;
}

Object*
nullObj()
{
  if (!nullSingleton) {
    nullSingleton = allocObj();
    nullSingleton->type = "";
    nullSingleton->isCompound = false;
    nullSingleton->isConst = true;
    nullSingleton->data.objectPtr = nullptr;
  }

  return nullSingleton;
}

Object*
boolObj(bool value)
{
  Bool* b = (Bool*)allocOnArena(sizeof(Bool));
  b->value = value;

  Object* obj = allocObj();
  obj->type = getInternedString("Bool");
  obj->isCompound = false;
  obj->data.objectPtr = b;

  return obj;
}

Object*
numberObj(double num)
{
  Number* n = (Number*)allocOnArena(sizeof(Number));
  n->value = num;

  Object* obj = allocObj();
  obj->isCompound = false;
  obj->type = getInternedString("Number");
  obj->data.objectPtr = n;

  return obj;
}

Object*
stringObj(const char* str, bool duplicate)
{
  String* s = (String*)allocOnArena(sizeof(String));
  s->value = duplicateString(str, duplicate);

  Object* obj = allocObj();
  obj->isCompound = false;
  obj->type = getInternedString("String");
  obj->data.objectPtr = s;

  return obj;
}

Object*
arrayObj(const std::vector<Object*>& elems)
{
  Array* a = (Array*)allocOnArena(sizeof(Array));
  a->objects = new std::vector<Object*>();
  a->objects->insert(a->objects->begin(), elems.begin(), elems.end());

  Object* obj = allocObj();
  obj->type = getInternedString("Array");
  obj->data.objectPtr = a;

  return obj;
}

Object*
functionObj(ASTNode* entry, const std::vector<ParameterInfo>& params)
{
  Function* fn = (Function*)allocOnArena(sizeof(Function));
  fn->isBuiltin = false;
  fn->code.entry = duplicateNode(entry);
  fn->params = new std::vector<ParameterInfo>();
  fn->params->insert(fn->params->end(), params.begin(), params.end());

  Object* obj = allocObj();
  obj->type = getInternedString("Function");
  obj->data.objectPtr = fn;

  return obj;
}

Object*
lambdaObj(ASTNode* entry,
          const std::map<std::string, Object*>& captureList,
          const std::vector<ParameterInfo>& params)
{
  Lambda* lambda = (Lambda*)allocOnArena(sizeof(Lambda));
  lambda->entry = duplicateNode(entry);
  lambda->captureList = new std::map<std::string, Object*>();
  lambda->captureList->insert(captureList.begin(), captureList.end());

  lambda->params = new std::vector<ParameterInfo>();
  lambda->params->insert(lambda->params->end(), params.begin(), params.end());

  // increase refcount for every captured object
  for (auto& o : captureList) {
    retainObject(o.second);
  }

  Object* obj = allocObj();
  obj->type = getInternedString("Lambda");
  obj->data.objectPtr = lambda;

  return obj;
}

Object*
classInstance(const char* className, const std::vector<Object*>& args)
{
  ClassInfo* info = contextLookupClass(className);
  std::string ctorName = "fn_ctor";
  // construct ctor name for lookup in table
  for (auto& a : args) {
    ctorName += a->type;
  }

  auto it = info->methods->at("ctor").find(ctorName);
  if (it == info->methods->at("ctor").end()) {
    setError(false,
             "Couldn't find %s class constructor %s\n",
             className,
             ctorName.c_str());
    return nullptr;
  }

  if (it->second.accessibility & MOD_HIDDEN) {
    setError(
      false, "Class %s, ctor %s is hidden\n", className, ctorName.c_str());
    return nullptr;
  }

  ClassMember ctor = it->second;
  Function* fn = (Function*)ctor.value->data.objectPtr;
  Object* instance = dynamicObj();
  instance->type = getInternedString(className);
  if (!info->isPrimitive) {
    instance->isCompound = true;
    instance->data.objectFields =
      new std::unordered_map<std::string, Object*>();

    for (auto& f : *info->fields) {
      // add only non-static and public fields
      if (!(f.second.accessibility & (MOD_HIDDEN | MOD_STATIC)))
        instance->data.objectFields->try_emplace(f.first, f.second.value);
    }
  } else {
    instance->isCompound = false;
  }

  callFunction(ctorName.c_str(), fn, args, NONE, instance);

  return instance;
}

Object*
builtinFnObj(const std::vector<ParameterInfo>& params, BuiltinFunction cb)
{
  Function* builtin = (Function*)allocOnArena(sizeof(Function));
  builtin->isBuiltin = true;
  builtin->code.cb = cb;

  builtin->params = new std::vector<ParameterInfo>();
  builtin->params->insert(builtin->params->end(), params.begin(), params.end());

  Object* obj = allocObj();
  obj->type = getInternedString("Function");
  obj->data.objectPtr = builtin;

  return obj;
}

Object*
dynamicObj()
{
  Object* obj = allocObj();
  obj->type = getInternedString("var");
  obj->data.objectFields = new std::unordered_map<std::string, Object*>();
  obj->data.objectPtr = nullptr;

  return obj;
}