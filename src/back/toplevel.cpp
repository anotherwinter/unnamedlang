#include "toplevel.h"
#include "runtime.h"
#include <algorithm>
#include <vector>

TopLevel::TopLevel(Runtime& runtime)
  : _runtime(runtime)
  , _toplevelArena(1024 * 1024)
{
  _classes = new ClassesMap();
  _nameToTypeID = new NameToTypeIDMap();
  _functions = new FunctionsMap();
  _fnNameToID = new FunctionsNameToIDMap();
}

TopLevel::~TopLevel()
{
  delete _classes;
  delete _nameToTypeID;
  delete _functions;
  delete _fnNameToID;
}

bool
TopLevel::defineFunction(const char* name,
                         bool isBuiltin,
                         const std::vector<const char*>& paramNames,
                         const std::vector<TypeID>& paramTypes,
                         ASTNode* entry,
                         const BuiltinFunction builtin)
{
  FunctionInfo* fn =
    createFunctionInfo(name, isBuiltin, paramNames, paramTypes, entry, builtin);

  if (fn == nullptr) {
    _runtime.error("TopLevel::defineFunction: function is null");
    return false;
  }

  return defineFunction(fn, *_functions, *_fnNameToID);
}

bool
TopLevel::defineClass(
  const char* name,
  bool isPrimitive,
  bool isBuiltin,
  const std::vector<ClassMethod>& methods,
  const std::vector<ClassField>& fields,
  const std::map<std::string, Identifier>& staticFieldsValues)
{
  auto classIDIt = _nameToTypeID->find(name);
  if (classIDIt != _nameToTypeID->end() &&
      _classes->find(classIDIt->second) != _classes->end()) {
    _runtime.error("TopLevel::defineClass: class is already defined");
    return false;
  }

  ClassInfo* clsInfo = createClassInfo(
    name, isPrimitive, isBuiltin, methods, fields, staticFieldsValues);

  TypeID classID;
  // if class was fwd declared, use existing type id
  if (classIDIt != _nameToTypeID->end())
    classID = classIDIt->second;
  else {
    classID = _typeID;
    _nameToTypeID->try_emplace(name, _typeID);
    ++_typeID.val;
  }

  return _classes->try_emplace(classID, clsInfo).second;
}

bool
TopLevel::declareClass(const char* name)
{
  if (_nameToTypeID->find(name) != _nameToTypeID->end()) {
    _runtime.error("declareClass: class %s is already declared\n", name);
    return false;
  }

  _nameToTypeID->try_emplace(name, _typeID);
  ++_typeID.val;
  return true;
}

Identifier
TopLevel::lookup(const char* name)
{
  Identifier res = {};
  auto clsIt = _nameToTypeID->find(name);
  if (clsIt != _nameToTypeID->end()) {
    // return class typeID instead of classinfo*
    res.name = clsIt->second;

    return res;
  }

  auto nameIDIt = _fnNameToID->find(name);
  // if name is convertible to nameID, then function with given name exists
  if (nameIDIt != _fnNameToID->end())
    res.name = nameIDIt->second;

  return res;
}

Identifier
TopLevel::resolveFunction(FnNameID id,
                          std::vector<TypeID>& paramTypes,
                          TypeID classID)
{
  Identifier res = {};
  FunctionsMap* fnMap = nullptr;
  if (classID.val != dynamicTypeID.val) {
    auto clsIt = _classes->find(classID);
    if (clsIt == _classes->end())
      return res;

    fnMap = clsIt->second->methods;
  } else
    fnMap = _functions;

  // try to find closest overload
  auto fnIt = fnMap->find(id);
  if (fnIt == fnMap->end())
    return res;

  auto overloadsIt = fnIt->second.find(paramTypes.size());
  if (overloadsIt == fnIt->second.end())
    return res;

  FnParamBitmask bm = makeParamBitmask(paramTypes);
  auto overloads = overloadsIt->second;
  for (auto& overload : overloads) {
    if (isCompatibleOverload(
          bm, paramTypes, overload->bm, *overload->paramTypes)) {
      res.name = overload;
      break;
    }
  }

  return res;
}

Identifier
TopLevel::lookupInClass(const char* name, Identifier id)
{
  Identifier res = {};
  // looking for class members
  if (auto classID = std::get_if<TypeID>(&id.name)) {
    auto clsIt = _classes->find(*classID);
    if (clsIt == _classes->end())
      return {};

    ClassInfo* clsInfo = clsIt->second;

    // 1. search in methods
    auto nameIDIt = clsInfo->mhtdNameToID->find(name);
    // if name is convertible to nameID, then function with given name exists
    if (nameIDIt != clsInfo->mhtdNameToID->end()) {
      res.name = nameIDIt->second;

      return res;
    }

    // 2. search in static fields
    auto fldIt = clsInfo->fields->find(name);
    if (fldIt != clsInfo->fields->end() &&
        (to_underlying(fldIt->second.accessibility) &
         to_underlying(Modifier::Static))) {
      auto valIt = clsInfo->staticFieldsValues->find(name);
      if (valIt != clsInfo->staticFieldsValues->end()) {
        _runtime.cacheLookup(&valIt->second);
        res = valIt->second;

        return res;
      }
    }
  }

  // looking for object values for instance field
  else if (auto hdr_ = std::get_if<ObjectHeader>(&id.name)) {
    ObjectHeader hdr = *hdr_;
    ClassInfo* clsInfo = getClassInfo(hdr.objClass);
    if (clsInfo == nullptr)
      return res;

    auto fldIt = clsInfo->fields->find(name);

    // found class field with matching name, extract object header from object's
    // payload knowing offset
    if (fldIt != clsInfo->fields->end()) {
      ClassField field = fldIt->second;
      size_t fieldOffset = field.fieldOffset;
      Identifier* fieldValue = hdr.data.instancePayload + fieldOffset;
      _runtime.cacheLookup(fieldValue);
      res = *fieldValue;
    }
  }

  return res;
}

ClassInfo*
TopLevel::getClassInfo(TypeID classID)
{
  ClassInfo* clsInfo = nullptr;
  auto clsIt = _classes->find(classID);
  if (clsIt != _classes->end())
    clsInfo = clsIt->second;

  return clsInfo;
}

Modifier
TopLevel::getMethodModifier(TypeID classID, FunctionID mthdID)
{
  auto clsIt = _classes->find(classID);
  if (clsIt == _classes->end()) {
    _runtime.error("TopLevel::getMethodModifier: unknown class ID");
    return {};
  }

  ClassInfo* clsInfo = clsIt->second;
  auto modIt = clsInfo->mthdIDToModifier->find(mthdID);
  if (modIt == clsInfo->mthdIDToModifier->end()) {
    _runtime.error("TopLevel::getMethodModifier: unknown method ID");
    return {};
  }

  return modIt->second;
}

FnParamBitmask
TopLevel::makeParamBitmask(std::vector<TypeID>& paramTypes)
{
  FnParamBitmask bitmask = 0;
  uint16_t pos = 0;

  for (auto& id : paramTypes) {
    if (id.val != dynamicTypeID.val)
      bitmask |= (1 << pos);

    pos <<= 1;
  }

  return bitmask;
}

FunctionInfo*
TopLevel::createFunctionInfo(const char* name,
                             bool isBuiltin,
                             const std::vector<const char*>& paramNames,
                             const std::vector<TypeID>& paramTypes,
                             const ASTNode* entry,
                             const BuiltinFunction builtin)
{
  FunctionInfo* fn = (FunctionInfo*)_runtime.alloc(sizeof(FunctionInfo));
  if (!fn)
    _runtime.error("createFunctionInfo: Failed to alloc object\n");

  fn->name = _runtime.getInternedString(name);
  fn->code.entry = entry;
  fn->isBuiltin = isBuiltin;
  fn->code.cb = builtin;

  fn->paramNames = new std::vector<const char*>();
  for (auto n : paramNames) {
    fn->paramNames->push_back(_runtime.getInternedString(n));
  }

  // TODO: add some safety checks

  fn->paramTypes = new std::vector<TypeID>();
  fn->paramTypes->insert(
    fn->paramTypes->begin(), paramTypes.begin(), paramTypes.end());

  return fn;
}

ClassInfo*
TopLevel::createClassInfo(
  const char* name,
  bool isPrimitive,
  bool isBuiltin,
  const std::vector<ClassMethod>& methods,
  const std::vector<ClassField>& fields,
  const std::map<std::string, Identifier>& staticFieldsValues)
{
  ClassInfo* cls = (ClassInfo*)_runtime.alloc(sizeof(ClassInfo));
  if (!cls)
    _runtime.error("createClassInfo: Failed to alloc object\n");

  cls->name = _runtime.getInternedString(name);
  cls->isPrimitive = isPrimitive;
  cls->isBuiltin = isBuiltin;
  cls->methods = new FunctionsMap();
  cls->mhtdNameToID = new FunctionsNameToIDMap();
  cls->mthdIDToModifier = new MethodsModifiersMap();
  cls->fields = new FieldsMap();
  cls->staticFieldsValues = new ObjectsMap();

  FunctionID mthdID = 0;

  for (auto& m : methods) {
    auto mthdFn = createFunctionInfo(
      m.name, m.isBuiltin, *m.paramNames, *m.paramTypes, m.entry, m.builtin);
    defineFunction(mthdFn, *cls->methods, *cls->mhtdNameToID);
    mthdFn->id = mthdID++;
    cls->mthdIDToModifier->try_emplace(mthdFn->id, m.accessibility);
  }

  for (auto& f : fields)
    cls->fields->emplace(f.name, f);

  // TODO: add uniqueness check for each member

  cls->staticFieldsValues->insert(staticFieldsValues.begin(),
                                  staticFieldsValues.end());

  return cls;
}

bool
TopLevel::defineFunction(FunctionInfo* fn,
                         FunctionsMap& fnMap,
                         FunctionsNameToIDMap& fnNameToIDMap)
{
  if (fn == nullptr)
    return false;

  auto fnIt = fnNameToIDMap.find(fn->name);
  FnParamBitmask bm = makeParamBitmask(*fn->paramTypes);

  // if trying to define overload
  if (fnIt != fnNameToIDMap.end()) {
    auto inner = fnMap.at(fnIt->second);
    auto paramTypes = fn->paramTypes;
    auto overloadsIt = inner.find(paramTypes->size());

    // if overloads list exists for given bitmask
    if (overloadsIt != inner.end()) {
      auto overloads = overloadsIt->second;

      // check if given overload is already defined
      for (auto& overload : overloads) {
        // if param types and count are equal, then that signature already
        // exists
        if (paramTypes == overload->paramTypes) {
          _runtime.error(
            "TopLevel::define: function overload is already defined");
          return false;
        }
      }

      // if no duplicate overload found, then emplace
      insertFunctionOverload(overloadsIt->second, fn);
    }
    // if there no overloads for bitmask, just emplace
    else
      inner.try_emplace(paramTypes->size(), std::vector<FunctionInfo*>{ fn });
  }
  // if function name is not defined
  else {
    FnNameID nameID = _fnNameID;
    ++_fnNameID.val;
    fnNameToIDMap.try_emplace(fn->name, nameID);
    fnMap.try_emplace(nameID).first->second.try_emplace(
      fn->paramTypes->size(), std::vector<FunctionInfo*>{ fn });
  }

  fn->id = _fnID++;

  return true;
}

void
TopLevel::insertFunctionOverload(std::vector<FunctionInfo*>& overloads,
                                 FunctionInfo* fn)
{
  auto cmp = [](const FunctionInfo* a, const FunctionInfo* b) {
    auto pSizeA = a->paramTypes->size();
    auto pSizeB = b->paramTypes->size();

    if (pSizeA != pSizeB)
      return pSizeA < pSizeB;

    // builtin popcount has parameter type of unsigned int, consider that when
    // changing bitmask type
    return __builtin_popcount(a->bm) < __builtin_popcount(b->bm);
  };

  auto it = std::lower_bound(overloads.begin(), overloads.end(), fn, cmp);
  overloads.insert(it, fn);
}

bool
TopLevel::isCompatibleOverload(FnParamBitmask bmA,
                               std::vector<TypeID>& typesA,
                               FnParamBitmask bmB,
                               std::vector<TypeID>& typesB)
{
  // if B has more fixed types, then A not compatible with B, rel. to A
  if (bmB > bmA || typesA.size() != typesB.size())
    return false;

  auto cmp = [](const TypeID& a, const TypeID& b) {
    if (a == dynamicTypeID || b == dynamicTypeID)
      return true;

    return a == b;
  };

  return std::equal(typesA.begin(), typesA.end(), typesB.begin(), cmp);
}