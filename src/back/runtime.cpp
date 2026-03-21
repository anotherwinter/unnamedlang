#include "runtime.h"
#include "../front/ast.h"
#include "builtins/builtins_init.h"
#include "context.h"
#include "toplevel.h"
#include "treewalk.h"
#include <cmath>
#include <cstring>
#include <variant>

Runtime::Runtime()
  : _topLevel(new TopLevel(*this))
  , _ctxArena(1024 * 1024)
  , _stringsArena(1024 * 1024)
  , _lookupHandleID(0)
{
  contextPush({}, ContextFlags::Global, nullptr);
  _global = _current;
  _internedStrings.clear();

  builtinsInit(*this);
}

void*
Runtime::alloc(size_t n)
{
  return _current->alloc(n, 1);
}

bool
Runtime::compareObjects(ObjectHeader& A, ObjectHeader& B)
{
  // TODO: implement compare
  if (A.valueType == ValueType::Instance || B.valueType == ValueType::Instance)
    return A.data.instancePayload == B.data.instancePayload;

  return true;
}

bool
Runtime::defineFunction(const char* name,
                        bool isBuiltin,
                        const std::vector<const char*>& paramNames,
                        const std::vector<TypeID>& paramTypes,
                        ASTNode* entry,
                        const BuiltinFunction builtin)
{
  return _topLevel->defineFunction(
    name, isBuiltin, paramNames, paramTypes, entry, builtin);
}

bool
Runtime::defineClass(
  const char* name,
  bool isPrimitive,
  bool isBuiltin,
  const std::vector<ClassMethod>& methods,
  const std::vector<ClassField>& fields,
  const std::map<std::string, Identifier>& staticFieldsValues)
{
  return _topLevel->defineClass(
    name, isPrimitive, isBuiltin, methods, fields, staticFieldsValues);
}

bool
Runtime::declareClass(const char* name)
{
  return _topLevel->declareClass(name);
}

bool
Runtime::defineVariable(const char* name, Identifier id)
{
  if (name && std::holds_alternative<ObjectHeader>(id.name)) {
    if (!std::holds_alternative<std::monostate>(lookup(name).name))
      error("define: %s is already defined\n", name);

    return _current->define(name, id);
  }

  return false;
}

const char*
Runtime::getInternedString(const char* str)
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
Runtime::releaseInternedString(const char* str)
{
  std::string key = str;
  auto it = _internedStrings.find(key);
  if (it != _internedStrings.end()) {
    --it->second.refcount;

    // free string
    if (it->second.refcount == 0) {
      _internedStrings.erase(it);
      _stringsArena.release((void*)it->second.str);
    }
  } else {
    error(
      "releaseInternedString: Trying to release string that was not cached\n");
  }
}

void
Runtime::setLastEval(Identifier id)
{
  _current->setLastEval(id);
}

Identifier
Runtime::getLastEval()
{
  return _current->getLastEval();
}

Identifier
Runtime::lookup(const char* name)
{
  // 1. search in top-level
  Identifier res = _topLevel->lookup(name);
  if (!std::holds_alternative<std::monostate>(res.name))
    return res;

  // 2. search for variable in contexts
  Context* ctx = _current;
  while (ctx) {
    Identifier* id = ctx->lookup(name);
    // if found name, cache its location
    if (id != nullptr) {
      cacheLookup(id);
      res = *id;
      break;
    }

    ctx = ctx->getPrev();
  }

  return res;
}

Identifier
Runtime::resolveFunction(FnNameID id, std::vector<TypeID>& paramTypes)
{
  return _topLevel->resolveFunction(id, paramTypes);
}

Identifier
Runtime::lookupInClass(const char* name, Identifier id)
{
  return _topLevel->lookupInClass(name, id);
}

void
Runtime::cacheLookup(Identifier* id)
{
  if (!id)
    return;

  if (id->locationHandle == 0) {
    id->locationHandle = ++_lookupHandleID;
    _lookupCache[id->locationHandle] = id;
  }
}

void
Runtime::printObject(const ObjectHeader& hdr)
{
  switch (hdr.valueType) {
    case ValueType::None: {
      printf("no value");
      break;
    }
    case ValueType::Bool: {
      bool b = hdr.data.boolValue;
      printf(b ? "true" : "false");
      break;
    }
    case ValueType::Number: {
      double num = hdr.data.numValue;
      printf("%g", num);
      break;
    }
    case ValueType::String: {
      const char* str = hdr.data.stringValue;
      printf("\"%s\"", str);
      break;
    }
    case ValueType::Null: {
      printf("null (obj)");
      break;
    }
    case ValueType::Array: {
      printf("array");
      break;
    }
    case ValueType::Instance: {
      printf("%s", _topLevel->getClassInfo(hdr.objClass)->name);
      break;
    }
  }

  printf("\n");
}

bool
Runtime::hasUnhandledException() const
{
  return to_underlying(_current->getFlags()) &
         to_underlying(ContextFlags::Exception);
}

ContextFlags
Runtime::getContextFlags() const
{
  return _current->getFlags();
}

void
Runtime::setContextFlag(ContextFlags flag)
{
  _current->setFlag(flag);
}

void
Runtime::resetContextFlag(ContextFlags flag)
{
  _current->resetFlag(flag);
}

Identifier
Runtime::call(FnNameID fnNameID,
              const std::vector<ObjectHeader>& args,
              Identifier id)
{
  std::vector<TypeID> types;
  for (auto a : args)
    types.push_back(a.objClass);

  TypeID classID;
  // if calling on class instance or static method
  if (auto cls = std::get_if<ClassInfo*>(&id.name)) {
    classID = (*cls)->typeID;
  } else if (auto obj = std::get_if<ObjectHeader>(&id.name)) {
    classID = (*obj).objClass;
    if (classID == dynamicTypeID) {
      error("call: calling on class instance with no class ID\n");
      return {};
    }
  }

  Identifier resolvedID = _topLevel->resolveFunction(fnNameID, types, classID);
  if (!std::holds_alternative<FunctionInfo*>(resolvedID.name)) {
    // TODO: more verbose error messages
    error("call: No overload");
    return {};
  }

  FunctionInfo* fn = std::get<FunctionInfo*>(resolvedID.name);
  if (classID.val != dynamicTypeID.val &&
      !isCurrentlyVisible(resolvedID, classID)) {
    error("call: Method %s is not accessible in given context");
    return {};
  }

  return callFunction(fn, args, _topLevel->getClassInfo(classID));
}

void
Runtime::returnFromFunction()
{
  if (!(to_underlying(_current->getFlags()) &
        to_underlying(ContextFlags::Function)))
    error("returnFromFn: Trying to return from a non-callable context\n");

  _current->returnFromFn();
}

bool
Runtime::tryAssign(uint32_t handle, Identifier value)
{
  if (!std::holds_alternative<ObjectHeader>(value.name))
    return false;

  auto it = _lookupCache.find(handle);
  if (it == _lookupCache.end())
    return false;

  ObjectHeader hdr = std::get<ObjectHeader>(value.name);

  // if only moving value
  if ((!hdr.isConst && isMovable(hdr)) || value.locationHandle == 0) {
    *it->second = value;
  }
  // if copying value
  else {
    Identifier duplicate = duplicateObject(value);
    if (!std::holds_alternative<ObjectHeader>(duplicate.name)) {
      error("tryAssign: failed to duplicate object");
      return false;
    }

    *it->second = duplicate;
  }

  return true;
}

Identifier
Runtime::accessArray(Identifier array, size_t idx)
{
  auto obj = std::get_if<ObjectHeader>(&array.name);
  if (!obj || obj->valueType != ValueType::Array) {
    error("accessArray: not an array\n");
    return {};
  }

  Array* a = reinterpret_cast<Array*>(obj->data.builtinPayload);
  if (idx > (a->size - 1)) {
    error("accessArray: index %zu is out of bounds\n", idx);
    return {};
  }

  Identifier* id = a->objects + idx;
  cacheLookup(id);

  return *id;
}

ObjectHeader
Runtime::invalidObjectHeader() const
{
  ObjectHeader hdr;
  hdr.objClass = {};
  hdr.valueType = ValueType::None;

  return hdr;
}

bool
Runtime::isPositiveInteger(double x) const
{
  return (std::fabs(x - std::round(x)) < 1e-9 && x > 0);
}

OpType
Runtime::doubleToOpType(double x)
{
  if (!isPositiveInteger(x)) {
    error("boolBinaryOp: op type is not positive integer");
    return OpType::_COUNT;
  }

  return static_cast<OpType>(x);
}

bool
Runtime::isPrimitive(TypeID classID)
{
  ClassInfo* cls = _topLevel->getClassInfo(classID);
  if (!cls) {
    error("isPrimitive: trying to check primitivity for undefined class ID\n");
    return false;
  }

  return cls->isPrimitive;
}

ClassInfo*
Runtime::getClassInfo(TypeID classID)
{
  return _topLevel->getClassInfo(classID);
}

Identifier
Runtime::nullObj()
{
  Identifier* id = _global->lookup("null");

  // this check only succeeds when initializing global symbolic table, so when
  // allocating header current == global
  if (id == nullptr) {
    ObjectHeader hdr;
    hdr.objClass = {};
    hdr.isConst = true;
    hdr.valueType = ValueType::Null;

    return { 0, hdr };
  }

  return *id;
}

Identifier
Runtime::boolObj(bool value)
{
  ObjectHeader hdr;
  hdr.isConst = false;
  hdr.objClass = std::get<TypeID>(_topLevel->lookup("Bool").name);
  hdr.valueType = ValueType::Bool;
  hdr.data.boolValue = value;

  return { 0, hdr };
}

Identifier
Runtime::numberObj(double num)
{
  ObjectHeader hdr;
  hdr.isConst = false;
  hdr.objClass = std::get<TypeID>(_topLevel->lookup("Number").name);
  hdr.valueType = ValueType::Number;
  hdr.data.numValue = num;

  return { 0, hdr };
}

Identifier
Runtime::stringObj(const char* str, bool duplicate)
{
  ObjectHeader hdr;
  hdr.isConst = false;
  hdr.objClass = std::get<TypeID>(_topLevel->lookup("String").name);
  hdr.valueType = ValueType::String;
  hdr.data.stringValue =
    duplicate ? duplicateString(str) : getInternedString(str);

  return { 0, hdr };
}

Identifier
Runtime::arrayObj(const std::vector<ObjectHeader>& elems)
{
  ObjectHeader hdr;
  hdr.isConst = false;
  hdr.objClass = std::get<TypeID>(_topLevel->lookup("Array").name);
  hdr.valueType = ValueType::Array;

  // allocate bytes for size field and object headers
  hdr.data.builtinPayload = static_cast<std::byte*>(_current->alloc(
    sizeof(uint64_t) + sizeof(ObjectHeader) * elems.size(), alignof(Array)));

  Array* a = reinterpret_cast<Array*>(hdr.data.builtinPayload);
  a->size = elems.size();
  memcpy(a->objects, elems.data(), sizeof(ObjectHeader) * elems.size());

  return { 0, hdr };
}

Identifier
Runtime::classInstance(TypeID classID,
                       const std::vector<ObjectHeader>& ctorArgs)
{
  ClassInfo* clsInfo = _topLevel->getClassInfo(classID);
  if (!clsInfo) {
    error("classInstance: undefined class ID");
    return {};
  }

  Identifier res = {};
  // if creating a non-builtin class instance, then allocate objectheader
  // otherwise treat call to ctor as a call to static member since
  // builtin classes allocate objects themselves
  // TODO: this has to be changed to unified behavior for both builtins and
  // non-builtins
  if (!clsInfo->isBuiltin) {
    ObjectHeader hdr;
    hdr.isConst = false;
    hdr.objClass = clsInfo->typeID;
    hdr.valueType = ValueType::Instance;
    hdr.data.instancePayload =
      static_cast<Identifier*>(alloc(clsInfo->payloadSize));

    res = { 0, hdr };
  }

  Identifier ctorNameID_ =
    _topLevel->lookupInClass(clsInfo->name, { 0, classID });
  auto ctorNameID = std::get_if<FnNameID>(&ctorNameID_.name);
  if (!ctorNameID) {
    error("classInstance: no ctor");
    return {};
  }

  call(*ctorNameID, ctorArgs, res);

  return res;
}

const char*
Runtime::duplicateString(const char* str)
{
  const char* ptr = strdup(str);
  if (!ptr)
    error("duplicateString: Failed to duplicate string\n");

  return ptr;
}

void
Runtime::contextPush(const std::unordered_map<std::string, Identifier>& table,
                     ContextFlags flags,
                     ClassInfo* clsInfo)
{
  void* raw = _ctxArena.alloc(sizeof(Context));
  if (!raw)
    error("contextPush: Failed to alloc context\n");

  Context* ctx = new (raw) Context(*this, table, flags, _current);
  _current = ctx;
}

void
Runtime::contextPop()
{
  Context* prevCtx = _current->getPrev();
  // if popping out with unhandled exception, revert all allocations and
  // modifications
  if (to_underlying(_current->getFlags()) &
      to_underlying(ContextFlags::Exception)) {
    if (_current == _global)
      exit(EXIT_FAILURE);
  }

  std::destroy_at(_current);
  _ctxArena.release(_current);

  _current = prevCtx;
}

void
Runtime::runtimeRecover()
{
  // TODO: implement actual recovery
  if (!(to_underlying(_current->getFlags()) &
        to_underlying(ContextFlags::Exception)))
    error("runtimeRecover: Trying to recover with no UNHANDLED_EX flag set "
          "(recover call without setting error)");

  while (_current != _global) {
    contextPop();
    _current->setFlag(ContextFlags::Exception);
  }

  // TODO: implement per-scope stack of exception handlers
}

bool
Runtime::isCurrentlyVisible(Identifier memb, TypeID receiverClassID)
{
  // TODO: fix accessibility/visibility
  return true;

  // Context* ctx = _current;
  // std::string name = {};

  // // build name if checking for method
  // if (member.nameType == NameType::Method) {
  //   FunctionInfo* mthdFn = member.name.mthd.mthd.mthdFn;
  //   name = buildCallableSignature(mthdFn->name, mthdFn->params);
  // }
  // // build name if checking for field
  // else {
  //   name = member.name.field.name;
  // }

  // Identifier clsId = { NameType::Class };
  // // 1. check visibility from nested contexts
  // while (ctx != _global) {
  //   ClassInfo* clsInfo = ctx->getClassInfo();
  //   clsId.name.classInfo = clsInfo;
  //   Identifier id = lookupInClass(name.c_str(), clsId);
  //   if (id.nameType != NameType::None)
  //     return true;
  // }

  // // 2. if not found in nested contexts, check if can be found in receiver's
  // // class and hidden modifier is disabled
  // if (receiverClsInfo) {
  //   clsId.name.classInfo = receiverClsInfo;
  //   Identifier id = lookupInClass(name.c_str(), clsId, fnID);
  //   if (id.nameType != NameType::None) {
  //     Modifier mod = Modifier::None;
  //     if (id.nameType == NameType::Method)
  //       mod = id.name.mthd.mthd.accessibility;
  //     else
  //       mod = id.name.field.field.accessibility;

  //     if (~to_underlying(mod) & to_underlying(Modifier::Hidden))
  //       return true;
  //   }
  // }

  // return false;
}

bool
Runtime::isMovable(ObjectHeader hdr) const
{
  auto cls = _topLevel->getClassInfo(hdr.objClass);
  return hdr.valueType == ValueType::Null || (cls && !cls->isPrimitive);
}

Identifier
Runtime::duplicateObject(Identifier id)
{
  if (!std::holds_alternative<ObjectHeader>(id.name))
    return {};

  ObjectHeader res = {};
  ObjectHeader hdr = std::get<ObjectHeader>(id.name);
  res.valueType = hdr.valueType;
  res.objClass = hdr.objClass;

  switch (hdr.valueType) {
    case ValueType::None: {
      error("duplicateObject: trying to duplicate none");
      break;
    }
    case ValueType::Bool: {
      res.data.boolValue = hdr.data.boolValue;
      break;
    }
    case ValueType::Number: {
      res.data.numValue = hdr.data.numValue;
      break;
    }
    case ValueType::String: {
      res.data.stringValue = duplicateString(hdr.data.stringValue);
      break;
    }
    case ValueType::Null: {
      error("duplicateObject: trying to duplicate null");
      break;
    }
    case ValueType::Array: {
      error("duplicateObject: cannot duplicate arrays");
      break;
    }
    case ValueType::Instance: {
      error("duplicateObject: cannot duplicate instances");
      break;
    }
  }

  return { 0, res };
}

void
Runtime::setExceptionFlag()
{
  _current->setFlag(ContextFlags::Exception);
}

Identifier
Runtime::callFunction(FunctionInfo* fn,
                      const std::vector<ObjectHeader>& args,
                      ClassInfo* clsInfo)
{
  // check if arguments count is the same as parameters count
  if (args.size() != fn->paramTypes->size())
    error("callFunction: Wrong arguments count, expected %d, got %d\n",
          fn->paramTypes->size(),
          args.size());

  // check types and build symbolic table
  std::unordered_map<std::string, Identifier> table;
  for (size_t i = 0; i < args.size(); ++i) {
    const ObjectHeader& arg = args[i];

    // actually building table only for ordinary function, not builtin, because
    // arguments are passed to builtin directly
    if (!fn->isBuiltin)
      table[fn->paramNames->at(i)] = { 0, arg };
  }

  contextPush(table, ContextFlags::Function, clsInfo);
  Identifier res = {};
  int jmpResult = setjmp(_env);
  if (jmpResult == 0) {
    if (!fn->isBuiltin) {
      _treewalk->eval(fn->code.entry);
      // if there was no longjmp, then there was no ret, return none
    } else {
      res = { 0, fn->code.cb(*this, args) };
    }
  }
  // if called return
  else if (jmpResult == 1) {
    res = getLastEval();
  }

  // pop context only if current context is not flagged as with exception (if it
  // is flagged, then recovery unwinding already popped context)
  if (!(to_underlying(_current->getFlags()) &
        to_underlying(ContextFlags::Exception)))
    contextPop();

  return res;
}