#include "runtime.h"
#include "arena.h"
#include "ast.h"
#include "bnf.tab.h"
#include "builtin_fns.h"
#include "builtin_types.h"
#include "eval_ast.h"
#include "types.h"
#include <csetjmp>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

// struct for storing reference count delta for given object in current context
typedef struct alignas(8)
{
  Object* obj;
  int delta;
} _RefDelta;

typedef struct alignas(8)
{
  // pointer to pointer holding object data
  Object** objPointer;
  // pointer to old object data so it can be restored
  Object* old;
} _ObjectChange;

typedef struct alignas(8) _Context
{
  // symbolic table of current context named objects
  std::map<std::string, Object*>* table;
  // table for every object that is in upper context and changed its value, so
  // you can recover it when exception occurs
  std::map<Object**, Object*>* changedObjects;
  // vector of referenced deltas for upper context objects so ARC can be
  // recovered from exception
  std::vector<_RefDelta>* rcDelta;
  // set of temporary objects that were created but haven't been claimed by
  // anyone
  std::unordered_set<Object*>* temporaryObjects;
  Object* exHandler;
  int flags;
  Object* lastEval;
  // class instance which created context
  Object* currentInstance;
  _Context* prev;
  size_t arenaOffset;
  // list of strings that were allocated inside this context, useful for
  // recovering
  std::vector<const char*>* allocatedStrings;
  // jmp buffer for returning from function
  jmp_buf env;
} _Context;

typedef struct alignas(8) StringInfo
{
  const char* str;
  size_t refcount;
} StringInfo;

static _Context* _current = nullptr;
static _Context* _global = nullptr;
static std::map<std::string, ClassInfo> _classesMap;
static Arena* _arena = nullptr;
static std::unordered_map<std::string, StringInfo> _internedStrings;
static jmp_buf _globalCheckpoint;
static bool _globalRecoveryEnabled = false;
static bool _strictMode = false;

static Object*
createClassCtor(ClassInfo* classInfo);

static Object*
createClassCtor(ClassInfo* classInfo)
{
  ASTNode* nodeList = nullptr;

  // if has fields then create linked list of assign statements each assigning
  // to "null"
  if (!classInfo->fields->empty()) {
    auto begin = classInfo->fields->begin();
    nodeList = newNodeList(nullptr);

    while (begin != classInfo->fields->end()) {
      ASTNode* assign =
        newVarAssign(EQUALS, newName(begin->second.name), newName("null"));
      astNodeLLAppend(nodeList->data.nodeList.list, assign);

      ++begin;
    }
  }

  return functionObj(nodeList, {});
}

void
runtimeInit()
{
  arenaInit(&_arena, 1024 * 1024);

  contextPush({}, IN_GLOBAL, nullptr);
  _global = _current;

  _internedStrings.clear();
  _classesMap.clear();

  builtinTypesInit();
  builtinFunctionsInit();

  registerObject("null", nullObj());
}

void*
allocOnArena(size_t n)
{
  void* ptr = arenaAlloc(_arena, n);
  if (!ptr)
    setError(true, "allocOnArena: Failed to alloc block with size %lu\n", n);

  return ptr;
}

// add temporary object to list for freeing
void
addTemporaryObject(Object* ptr)
{
  _current->temporaryObjects->insert(ptr);
}

void
cleanTemporaryObjects()
{
  for (auto& o : *_current->temporaryObjects)
    releaseObject(o);

  _current->temporaryObjects->clear();
}

const char*
duplicateString(const char* str, bool duplicate)
{
  if (duplicate) {
    const char* ptr = strdup(str);
    if (!ptr)
      setError(true, "duplicateString: Failed to duplicate string\n");

    _current->allocatedStrings->push_back(ptr);

    return ptr;
  }

  return str;
}

const char*
getInternedString(const char* str)
{
  std::string key = str;
  auto it = _internedStrings.find(key);
  if (it != _internedStrings.end()) {
    ++it->second.refcount;
    return it->second.str;
  }
  // cache string if its not present in map
  const char* copy = strdup(str);
  _internedStrings[str] = { copy, 1 };

  return copy;
}

void
releaseInternedString(const char* str)
{
  std::string key = str;
  auto it = _internedStrings.find(key);
  if (it != _internedStrings.end()) {
    --it->second.refcount;
    // free string
    if (it->second.refcount == 0) {
      _internedStrings.erase(it);
      free((void*)it->second.str);
    }
  } else {
    setError(
      false,
      "releaseInternedString: Trying to release string that was not cached\n");
  }
}

void
contextPop()
{
  contextSetLastEval(nullObj());

  for (auto& o : *_current->table) {
    releaseObject(o.second);
  }

  cleanTemporaryObjects();

  _Context* prevCtx = _current->prev;
  // if popping out with unhandled exception, revert all allocations and
  // modifications
  if (contextGetFlags() & UNHANDLED_EX) {
    if (_current == _global)
      exit(EXIT_FAILURE);

    // revert to old values of variables if they were changed
    for (auto& o : *_current->changedObjects)
      *(o.first) = o.second;

    // recover old refcounts and erase info about new free blocks
    for (auto& rc : *_current->rcDelta) {
      // unmark blocks as free if they were freed inside context with unhandled
      // exception
      if (rc.obj->refcount == 0)
        arenaResetFreeBlock(_arena, sizeof(Object), (void*)rc.obj);

      rc.obj->refcount -= rc.delta;
    }

    // free every allocated string in this context
    for (auto& s : *_current->allocatedStrings) {
      free((void*)s);
    }

    delete _current->table;
    delete _current->changedObjects;
    delete _current->rcDelta;
    delete _current->temporaryObjects;

    // no need to release this context because it creates new free block that
    // shouldnt have existed if we reverted arena state
    // arenaRelease(_arena, _current, sizeof(_Context));
    _current = prevCtx;
    arenaSetOffset(_arena, _current->arenaOffset);
  } else {
    delete _current->table;
    delete _current->changedObjects;
    delete _current->rcDelta;
    delete _current->temporaryObjects;

    arenaRelease(_arena, _current, sizeof(_Context));
    _current = prevCtx;
  }
}

void
contextPush(std::unordered_map<std::string, Object*> table,
            int flags,
            Object* instance)
{
  size_t offset = arenaGetOffset(_arena);
  _Context* ctx = (_Context*)arenaAlloc(_arena, sizeof(_Context));
  if (!ctx)
    setError(true, "contextPush: Failed to alloc context\n");

  ctx->table = new std::map<std::string, Object*>;
  ctx->changedObjects = new std::map<Object**, Object*>();
  ctx->rcDelta = new std::vector<_RefDelta>();
  ctx->temporaryObjects = new std::unordered_set<Object*>();
  ctx->exHandler = nullptr;
  ctx->flags = flags;
  ctx->lastEval = nullptr;
  ctx->currentInstance = instance;
  ctx->prev = _current;
  ctx->arenaOffset = arenaGetOffset(_arena);
  ctx->allocatedStrings = new std::vector<const char*>();

  if (_current)
    _current->arenaOffset = offset;

  for (auto& o : table) {
    Object* obj = o.second;
    retainObject(obj);
    ctx->table->try_emplace(o.first, obj);
  }

  _current = ctx;
}

void
contextSetExHandler(Object* handler)
{
  if (_current->exHandler) {
    releaseObject(_current->exHandler);
  }

  _current->exHandler = handler;
  // retain handler so it will survive context destruction
  retainObject(handler);
}

void
runtimeRecover()
{
  if (!(contextGetFlags() & UNHANDLED_EX))
    setError(true,
             "runtimeRecover: Trying to recover with no UNHANDLED_EX flag set "
             "(recover call without setting error)");

  while (!_current->exHandler && _current != _global) {
    contextSetFlag(UNHANDLED_EX);
    contextPop();
  }

  // if has exception handler, handle it and restore execution
  if (_current->exHandler) {
    Lambda* lambdaObj = (Lambda*)_current->exHandler->data.objectPtr;
    int usedRet = setjmp(_current->env);
    if (usedRet == 0) {
      evalNode(lambdaObj->entry);
      contextSetLastEval(nullObj());
    }
    releaseObject(_current->exHandler);

    // if handling from global context, just exit
    if (_current == _global)
      exit(EXIT_SUCCESS);

    contextResetFlag(UNHANDLED_EX);
    contextPop();
  }
  // if no exception handler was set, try to recover to last global checkpoint
  else if (_globalRecoveryEnabled) {
    _globalRecoveryEnabled = false;
    longjmp(_globalCheckpoint, 1);
  } else {
    exit(EXIT_FAILURE);
  }
}

int
setGlobalCheckpoint()
{
  _globalRecoveryEnabled = true;

  int value = setjmp(_globalCheckpoint);
  return value;
}

void
returnFromFunction()
{
  if (!(contextGetFlags() & IN_FUNCTION))
    setError(false,
             "returnFromFn: Trying to return from a non-callable context\n");

  longjmp(_current->env, 1);
}

void
contextSetLastEval(Object* obj)
{
  _current->lastEval = obj;
}

Object*
contextGetLastEval()
{
  return _current->lastEval;
}

int
contextGetFlags()
{
  return _current->flags;
}

void
contextSetFlag(int flag)
{
  _current->flags |= flag;
}

void
contextResetFlag(int flag)
{
  _current->flags &= ~flag;
}

Object*
contextGetCurrentInstance()
{
  return _current->currentInstance;
}

Object**
contextLookup(const char* name, bool supressErrors)
{
  // 1. lookup in current context table
  auto it = _current->table->find(name);
  if (it != _current->table->end())
    return &it->second;

  // 2. lookup in instances fields
  if (_current->currentInstance) {
    auto& fields = _current->currentInstance->data.objectFields;
    auto it = fields->find(name);
    if (it != fields->end())
      return &it->second;

    // 3. lookup in class static fields & methods
    Object** obj = contextLookupClassMember(
      _current->currentInstance->type, name, nullptr, true);
    if (obj)
      return obj;
  }

  // 4. lookup in global table
  it = _global->table->find(name);
  if (it != _global->table->end())
    return &it->second;

  return nullptr;
}

ClassInfo*
contextLookupClass(const char* className)
{
  ClassInfo* info = nullptr;
  auto it = _classesMap.find(className);
  if (it != _classesMap.end())
    info = &it->second;

  return info;
}

Object**
contextLookupClassMember(const char* className,
                         const char* rawMemberName,
                         const char* overloadedMemberName,
                         bool lookForStatic,
                         bool supressErrors)
{
  ClassInfo* classInfo = contextLookupClass(className);
  if (!supressErrors && !classInfo)
    setError(false, "lookupClassMember: Can't find class %s\n", className);

  bool sameClassContext =
    (_current->currentInstance &&
     strcmp(_current->currentInstance->type, className) == 0);
  auto fieldIt = classInfo->fields->find(std::string{ rawMemberName });
  if (fieldIt != classInfo->fields->end()) {
    bool isHidden = fieldIt->second.accessibility & MOD_HIDDEN;
    if (!supressErrors && isHidden && !sameClassContext) {
      setError(false,
               "lookupClassMember: Trying to access hidden field %s outside of "
               "class %s\n",
               rawMemberName,
               className);
      return nullptr;
    }

    bool isStatic = fieldIt->second.accessibility & MOD_STATIC;
    if (!supressErrors && !isStatic && lookForStatic) {
      setError(false,
               "lookupClassMember: No static field named %s in class %s\n",
               rawMemberName,
               className);
      return nullptr;
    }

    return &fieldIt->second.value;
  }

  if (!supressErrors && overloadedMemberName == nullptr) {
    setError(false,
             "lookupClassMember: Can't find field %s in class %s\n",
             rawMemberName,
             className);
    return nullptr;
  }

  auto rawMethodNameIt = classInfo->methods->find(rawMemberName);
  if (!supressErrors && rawMethodNameIt == classInfo->methods->end()) {
    setError(false,
             "lookupClassMember: Can't find method %s in class %s\n",
             rawMemberName,
             className);
    return nullptr;
  }

  auto overloadedNames = rawMethodNameIt->second;
  auto overloadedIt = overloadedNames.find(std::string{ overloadedMemberName });
  if (!supressErrors && overloadedIt == overloadedNames.end()) {
    setError(
      false,
      "lookupClassMember: Can't find overload %s for member %s in class %s\n",
      overloadedMemberName,
      rawMemberName,
      className);
    return nullptr;
  }

  ClassMember member = overloadedIt->second;
  bool isHidden = (member.accessibility & MOD_HIDDEN);
  if (!supressErrors && isHidden && !sameClassContext) {
    setError(false,
             "lookupClassMember: Trying to access hidden method %s outside of "
             "class %s\n",
             overloadedMemberName,
             className);
    return nullptr;
  }

  bool isStatic = member.accessibility & MOD_STATIC;
  if (!supressErrors && !isStatic && lookForStatic) {
    setError(
      false,
      "lookupClassMember: No static method overload named %s in class %s\n",
      overloadedMemberName,
      className);
    return nullptr;
  }

  return &overloadedIt->second.value;
}

void
retainObject(Object* obj)
{
  if (obj)
    obj->refcount++;

  // if it has been in temporary objects, remove it from list cause now someone
  // claimed this object
  auto it = _current->temporaryObjects->find(obj);
  if (it != _current->temporaryObjects->end())
    _current->temporaryObjects->erase(it);
}

void
releaseObject(Object* obj)
{
  if (!obj)
    return;

  if (obj->refcount < 0)
    setError(true, "releaseObject: Already released object\n");

  // deleting global null object is not allowed
  if (obj == nullObj())
    return;

  obj->refcount--;
  size_t objSize = sizeof(Number);
  // checking lower than or equal zero because this can release temporary
  // objects, that have not been linked to anything
  if (obj->refcount == 0) {
    if (obj == _current->lastEval)
      _current->lastEval = nullObj();

    if (obj->isCompound) {
      // if its a compound type, then try to unref every field
      for (auto& field : *obj->data.objectFields) {
        releaseObject(field.second);
      }

      delete obj->data.objectFields;
      arenaRelease(_arena, obj, sizeof(Object));

      return;
    } else {
      // if its an array, try to unref every element
      if (strcmp(obj->type, "Array") == 0) {
        Array* arr = (Array*)obj->data.objectPtr;
        for (auto& o : *arr->objects)
          releaseObject(o);

        delete arr->objects;
        objSize = sizeof(Array);
      }

      // if its a string, free its pointer
      else if (strcmp(obj->type, "String") == 0) {
        String* str = (String*)obj->data.objectPtr;
        free((void*)str->value);

        objSize = sizeof(String);
      }

      // if function, free paramsinfo, return type and AST nodes
      else if (strcmp(obj->type, "Function") == 0) {
        Function* fn = (Function*)obj->data.objectPtr;
        if (!fn->isBuiltin) {
          freeNode(fn->code.entry);
          for (auto& p : *fn->params) {
            free((void*)p.name);
          }
        }

        delete fn->params;
        objSize = sizeof(Function);
      }

      // if lambda, free params and AST nodes and decrease refcount of captured
      // names
      else if (strcmp(obj->type, "Lambda") == 0) {
        Lambda* lambda = (Lambda*)obj->data.objectPtr;
        freeNode(lambda->entry);
        for (auto& p : *lambda->params) {
          free((void*)p.name);
        }

        delete lambda->params;

        for (auto& c : *lambda->captureList) {
          releaseObject(c.second);
        }

        delete lambda->captureList;
        objSize = sizeof(Lambda);
      }

      else if (strcmp(obj->type, "Bool") == 0)
        objSize = sizeof(Bool);
    }

    arenaRelease(_arena, obj->data.objectPtr, objSize);
    arenaRelease(_arena, obj, sizeof(Object));
  }
}

bool
registerObject(const char* objName, Object* obj)
{
  if (contextLookup(objName))
    setError(false,
             "registerObject: Can't register %s since its already registered\n",
             objName);

  retainObject(obj);
  // if we are not in function then assume global context
  if (!(contextGetFlags() & IN_FUNCTION))
    _global->table->try_emplace(objName, obj);
  else
    _current->table->try_emplace(objName, obj);

  return true;
}

void
printObject(Object* obj)
{
  if (!obj) {
    printf("null\n");
    return;
  }

  if (strcmp(obj->type, "Number") == 0) {
    Number* n = (Number*)obj->data.objectPtr;
    printf("%g", n->value);
  }

  else if (strcmp(obj->type, "String") == 0) {
    String* s = (String*)obj->data.objectPtr;
    printf("\"%s\"", s->value);
  }

  else if (strcmp(obj->type, "Bool") == 0) {
    Bool* b = (Bool*)obj->data.objectPtr;
    printf(b->value ? "true" : "false");
  }

  else if (strcmp(obj->type, "Array") == 0) {
    Array* a = (Array*)obj->data.objectPtr;
    printf("[");
    for (size_t i = 0; i < a->objects->size(); ++i) {
      printObject(a->objects->at(i));
      if (i + 1 < a->objects->size())
        printf(", ");
    }
    printf("]");
  }

  else if (strcmp(obj->type, "") == 0) {
    printf("null (obj)");
  }

  else if (obj->isCompound) {
    printf("<%s>:", obj->type);
    auto objectFields = obj->data.objectFields;
    int i = 0;

    for (auto begin = objectFields->begin(); begin != objectFields->end();
         ++begin) {
      printObject(begin->second);
      if (i + 1 < obj->data.objectFields->size())
        printf(", ");

      ++i;
    }
  }

  printf("\n");
}

void
registerClass(const char* className,
              bool isPrimitive,
              std::vector<ClassMember> members)
{
  // check if given class exists
  if (contextLookupClass(className))
    setError(false, "registerClass: Class %s is already defined\n", className);

  bool hasDefaultCtor = false;
  bool hasDefaultDtor = false;
  ClassInfo info;
  info.name = getInternedString(className);
  info.isPrimitive = isPrimitive;
  info.methods =
    new std::map<std::string, std::map<std::string, ClassMember>>();
  info.fields = new std::map<std::string, ClassMember>();

  for (auto& m : members) {
    Object* obj = m.value;
    bool isFunction = strcmp(obj->type, "Function") == 0;
    bool isPublic = m.accessibility & MOD_PUBLIC;

    // check if current member is a ctor with 0 parameters (default ctor)
    if (strcmp(m.name, "ctor") == 0 && isPublic && isFunction) {
      Function* fn = (Function*)obj->data.objectPtr;
      if (fn->params->empty())
        hasDefaultCtor = true;

    }

    else if (strcmp(m.name, "dtor") == 0 && isPublic && isFunction) {
      Function* fn = (Function*)obj->data.objectPtr;
      if (fn->params->empty()) {
        hasDefaultDtor = true;
      } else {
        // print error if dtor has any params
        setError(false,
                 "Error when registering class %s: class can't have "
                 "destructor with parameters\n",
                 className);
      }
    }

    std::string memberStr;
    // construct member name according to its type
    if (isFunction) {
      // construct method name
      memberStr = std::string{ "fn_" } + m.name;
      Function* fn = (Function*)obj->data.objectPtr;
      for (auto& p : *fn->params) {
        memberStr += p.type;
      }
    }
    // if member is not of a Function, then register it as
    // ordinary field
    else {
      memberStr = std::string{ "f_" } + m.name;
    }

    // check if name was already defined but with different type (function vs
    // field)
    auto methodsIt = info.methods->find(std::string{ m.name });
    if (isFunction) {
      bool alreadyDefined =
        info.fields->find(std::string{ m.name }) != info.fields->end();
      if (alreadyDefined)
        setError(false,
                 "registerClass: Member %s of class was already defined\n",
                 memberStr.c_str());

      // check if overload was already defined
      if (methodsIt != info.methods->end() &&
          methodsIt->second.find(memberStr) != methodsIt->second.end())
        setError(false,
                 "registerClass: Member %s of class was already defined\n",
                 memberStr.c_str());

      auto& inner =
        info.methods->try_emplace(std::string{ m.name }).first->second;
      inner.try_emplace(memberStr, m);
    }

    else {
      bool alreadyDefined = !isFunction && methodsIt != info.methods->end();
      if (alreadyDefined)
        setError(false,
                 "registerClass: Member %s of class was already defined\n",
                 memberStr.c_str());
    }

    info.fields->try_emplace(std::string{ m.name }, m);
  }

  // if class has no default ctor, add it
  if (!hasDefaultCtor) {
    ClassMember ctor = { "ctor", createClassCtor(&info), MOD_PUBLIC };
    auto& inner = info.methods->try_emplace("ctor").first->second;
    inner.try_emplace(std::string{ "fn_ctor" }, ctor);
  }

  // if class has no default dtor, add it
  if (!hasDefaultDtor) {
    ClassMember dtor = { "dtor", nullptr, MOD_PUBLIC };
    auto& inner = info.methods->try_emplace("dtor").first->second;
    inner.try_emplace(std::string{ "fn_dtor" }, dtor);
  }

  // increment refcount for every member
  for (auto& rawName : *info.methods) {
    for (auto& m : rawName.second)
      retainObject(m.second.value);
  }

  for (auto& field : *info.fields) {
    retainObject(field.second.value);
  }

  _classesMap[std::string{ className }] = info;
}

void
registerFunction(const char* fnName, Object* fnObj)
{
  // check if function with given name already exists
  Object** existingFnObj = contextLookup(fnName);
  if (existingFnObj)
    setError(
      false, "registerFunction: Function %s is already defined\n", fnName);

  retainObject(fnObj);

  _global->table->try_emplace(fnName, fnObj);
}

void
setError(bool errorExit, const char* str, ...)
{
  va_list args;

  va_start(args, str);
  vfprintf(stderr, str, args);

  va_end(args);

  if (errorExit || _strictMode)
    exit(EXIT_FAILURE);

  contextSetFlag(UNHANDLED_EX);
  runtimeRecover();
}

void
runtimeSetStrictMode(bool strict)
{
  _strictMode = strict;
}

bool
runtimeGetStrictMode()
{
  return _strictMode;
}

void
callFunction(const char* fnName,
             Function* fn,
             std::vector<Object*> args,
             int flags,
             Object* instance)
{
  // check if arguments count is the same as parameters count
  if (args.size() != fn->params->size())
    setError(false,
             "callFunction: Wrong arguments count, expected %d, got %d\n",
             fn->params->size(),
             args.size());

  // build up symbolic table for function's context
  std::unordered_map<std::string, Object*> table;
  for (int i = 0; i < args.size(); ++i) {
    Object* arg = args[i];
    // if parameter type is not dynamic and types are not equal, report error
    if (strcmp(fn->params->at(i).type, "var") &&
        strcmp(arg->type, fn->params->at(i).type))
      setError(false,
               "callFunction: Type mismatch when calling %s: expected %s, "
               "got %s as argument #%d\n",
               fnName,
               fn->params->at(i).type,
               arg->type,
               i);

    // building table only for ordinary function, not builtin
    if (!fn->isBuiltin)
      table[fn->params->at(i).name] = arg;
  }

  // refcount of every arg is incremented while pushing context
  contextPush(table, IN_FUNCTION | flags, instance);

  Object* obj = nullptr;
  int usedRet = setjmp(_current->env);
  if (usedRet == 0) {
    if (!fn->isBuiltin)
      evalNode(fn->code.entry);
    else {
      fn->code.cb(&args);
      for (auto& a : args)
        releaseObject(a);

      contextPop();
      return;
    }
    // if there was no longjmp, then there was no ret, return null object

    obj = nullObj();
  } else {
    obj = contextGetLastEval();
  }

  contextPop();
  contextSetLastEval(obj);
}

void
callLambda(Lambda* lambda, std::vector<Object*> args)
{
  // build up symbolic table for function's context
  std::unordered_map<std::string, Object*> table;
  for (int i = 0; i < args.size(); ++i) {
    Object* arg = args[i];
    // if parameter type is not dynamic and types are not equal, report error
    if (strcmp(lambda->params->at(i).type, "var") &&
        strcmp(arg->type, lambda->params->at(i).type))
      setError(false,
               "callLambda: Type mismatch when calling lambda: expected %s, "
               "got %s as argument #%d\n",
               lambda->params->at(i).type,
               arg->type,
               i);

    table[lambda->params->at(i).name] = arg;
  }

  // refcount of every arg is incremented while pushing context
  contextPush(table, IN_FUNCTION, nullptr);

  // but we do not want to increment refcount for captured values, since they
  // are already retained in lambda object, so we insert them after pushing
  // context
  _current->table->insert(lambda->captureList->begin(),
                          lambda->captureList->end());

  Object* obj = nullptr;
  int usedRet = setjmp(_current->env);
  if (usedRet == 0) {
    evalNode(lambda->entry);

    // if there was no longjmp, then there was no ret, return null object
    obj = nullObj();
  } else {
    obj = contextGetLastEval();
  }
  contextPop();
  contextSetLastEval(obj);
}

void
callClassMember(const char* className,
                const char* memberName,
                Object* instance,
                std::vector<Object*> args)
{
  std::string methodStr = std::string{ "fn_" } + memberName;
  for (auto& a : args) {
    methodStr += a->type;
  }

  Object** memberPtr =
    contextLookupClassMember(className, memberName, methodStr.c_str(), false);
  if (!memberPtr)
    setError(false, "callClasMember: Failed to lookup %s\n", memberName);

  Object* member = *memberPtr;
  bool isLambda = strcmp(member->type, "Lambda") == 0;
  if (!isLambda && strcmp(member->type, "Function"))
    setError(false,
             "callClassMember: Member %s is not a callable object\n",
             memberName);

  if (isLambda)
    callLambda((Lambda*)member->data.objectPtr, args);
  else
    callFunction(methodStr.c_str(),
                 (Function*)member->data.objectPtr,
                 args,
                 IN_METHOD,
                 instance);
}

void
callBinaryOp(OperatorType opType, Object* lhs, Object* rhs)
{
  if (!lhs)
    setError(false, "callBinaryOp: lhs is nullptr\n");
  else if (!rhs)
    setError(false, "callBinaryOp: rhs is nullptr\n");

  // generate operator function name
  std::string opFnName =
    "fn_binaryNumber" + std::string{ lhs->type } + std::string{ rhs->type };
  Modifier mod = (Modifier)(MOD_PUBLIC | MOD_STATIC);

  // try to find operator function defined in lhs
  Object** fnPtr =
    contextLookupClassMember(lhs->type, "binary", opFnName.c_str(), false);
  if (!fnPtr)
    setError(false,
             "callBinaryOp: Cannot find binary operator function for "
             "types %s, %s\n",
             lhs->type,
             rhs->type);

  Object* fnObj = *fnPtr;
  Object* typeWrapper = numberObj((double)opType);
  callFunction(opFnName.c_str(),
               (Function*)fnObj->data.objectPtr,
               { typeWrapper, lhs, rhs },
               NONE,
               nullptr);
}

void
callUnaryOp(OperatorType opType, Object* expr)
{
  // generate operator function name
  std::string opFnName = "fn_unaryNumber" + std::string{ expr->type };
  Modifier mod = (Modifier)(MOD_PUBLIC | MOD_STATIC);

  // try to find operator function defined in obj
  Object** fnPtr =
    contextLookupClassMember(expr->type, "unary", opFnName.c_str(), false);
  if (!fnPtr)
    setError(false,
             "evalUnaryOp: Cannot find unary operator function for type %s\n",
             expr->type);

  Object* fnObj = *fnPtr;
  Object* typeWrapper = numberObj((double)opType);
  callFunction(opFnName.c_str(),
               (Function*)fnObj->data.objectPtr,
               { typeWrapper, expr },
               NONE,
               nullptr);
}

Object**
resolveLHS(ASTNode* lhsNode, bool suppressErrors)
{
  switch (lhsNode->type) {
    case NODE_NAME: {
      const char* name = lhsNode->data.stringValue;
      Object** ptr = contextLookup(name);
      if (!ptr && !suppressErrors)
        setError(false, "evalVarAssign: Variable %s is not declared\n", name);

      return ptr;
    }

    case NODE_MEMBER_ACCESS: {
      // lookup in static
      ASTNode* left = lhsNode->data.memberAccess.left;
      const char* field = lhsNode->data.memberAccess.right->data.stringValue;

      if (left->type == NODE_NAME &&
          contextLookupClass(left->data.stringValue)) {
        return contextLookupClassMember(
          left->data.stringValue, field, nullptr, true, suppressErrors);
      }

      // lookup in instance
      evalNode(left);
      Object* inst = contextGetLastEval();
      if (!inst || !inst->isCompound) {
        if (!suppressErrors)
          setError(false, "evalVarAssign: Left side is not an object\n");
      }

      auto& fields = inst->data.objectFields;
      auto it = fields->find(field);
      if (it == fields->end()) {
        if (!suppressErrors)
          setError(false,
                   "evalVarAssign: Field %s not found in %s\n",
                   field,
                   inst->type);
      }

      return &it->second;
    }

    case NODE_ARR_ACCESS: {
      // lookup in array
      evalNode(lhsNode->data.arrayAccess.left);
      Object* arrObj = contextGetLastEval();
      if (!arrObj || strcmp(arrObj->type, "Array") != 0) {
        if (!suppressErrors)
          setError(false, "evalVarAssign: Left side is not an Array\n");
      }

      Array* arr = (Array*)arrObj->data.objectPtr;
      evalNode(lhsNode->data.arrayAccess.subscript);
      Object* indexObj = contextGetLastEval();
      if (!indexObj || strcmp(indexObj->type, "Number") != 0) {
        if (!suppressErrors)
          setError(false, "evalVarAssign: Array index is not a Number\n");
      }

      int index = (int)((Number*)indexObj->data.objectPtr)->value;
      if (index < 0 || index >= (int)arr->objects->size()) {
        if (!suppressErrors)
          setError(
            false, "evalVarAssign: Array index %d out of range\n", index);
      }

      return &((*arr->objects)[index]);
    }

    default:
      if (!suppressErrors)
        setError(
          false, "evalVarAssign: Unsupported LHS type %d\n", lhsNode->type);
  }

  return nullptr;
}
