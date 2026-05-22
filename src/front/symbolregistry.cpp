#include "front/symbolregistry.h"
#include "diagnostics.h"
#include "errorcode_bases.h"
#include "front/scope.h"
#include <algorithm>
#include <variant>

SymbolRegistry::SymbolRegistry(Diagnostics& diag)
  : _diag(diag)
{
  // push global scope
  pushScope();
}

SymbolRegistry::~SymbolRegistry() {}

FnDeclKey
SymbolRegistry::declareFunction(const std::string& name,
                                const std::vector<std::string>& paramNames,
                                const std::vector<TypeID>& paramTypes,
                                TypeID ownerID)
{
  auto it = _fnNameToID.find(name);
  FnNameID nameID;
  if (it == _fnNameToID.end()) {
    _fnNameToID.try_emplace(name, _nameID);
    ++_nameID.val;
    nameID = _nameID;
  } else
    nameID = it->second;

  auto fnRes = resolveFunction(nameID, paramTypes, ownerID);
  if (isValid(fnRes.id))
    return { fnRes.id, nameID };

  FunctionInfo fnInfo = { paramNames, paramTypes, _fnID, ownerID };
  auto inserted = _fnInfoStorage.emplace(_fnID, fnInfo);

  auto fnsIt = _functions.try_emplace(ownerID);
  auto overloadsIt = fnsIt.first->second.try_emplace(nameID);
  auto maskedOverloadsIt =
    overloadsIt.first->second.try_emplace(makeFnParamBitmask(paramTypes));
  insertFunctionOverload(inserted.first->second,
                         maskedOverloadsIt.first->second);

  ++_fnID;

  return { fnInfo.id, nameID };
}

FunctionInfo*
SymbolRegistry::resolveFunction(FunctionID id)
{
  auto it = _fnInfoStorage.find(id);
  if (it == _fnInfoStorage.end())
    return nullptr;

  return &it->second;
}

FnResolution
SymbolRegistry::resolveFunction(FnNameID nameID,
                                const std::vector<TypeID>& paramTypes,
                                TypeID ownerID)
{
  auto overloads = resolveFunctionOverloads(nameID, paramTypes, ownerID, true);
  if (overloads == nullptr)
    return {};

  auto fnID = overloads->back();
  delete overloads;

  return fnID;
}

std::vector<FnResolution>*
SymbolRegistry::resolveFunctionOverloads(FnNameID nameID,
                                         const std::vector<TypeID>& paramTypes,
                                         TypeID ownerID,
                                         bool exact,
                                         bool single)
{
  auto overloadsOuterIt = _functions.find(ownerID);
  if (overloadsOuterIt == _functions.end())
    return nullptr;

  auto overloadsInnerIt = overloadsOuterIt->second.find(nameID);
  if (overloadsInnerIt == overloadsOuterIt->second.end())
    return nullptr;

  auto overloadsByParamCount = overloadsInnerIt->second;
  auto overloadsIt = overloadsByParamCount.find(paramTypes.size());
  // if overloads list exists for given param count
  if (overloadsIt == overloadsByParamCount.end())
    return nullptr;

  auto overloads = overloadsIt->second;
  std::vector<FnResolution>* matches = new std::vector<FnResolution>();
  // TODO: optimize this
  for (auto& o : overloads) {
    bool match = true;
    for (size_t i = 0; i < paramTypes.size(); ++i) {
      if (!areTypesCompatible(paramTypes[i], o->paramTypes[i])) {
        match = false;
        break;
      }
    }

    if (match) {
      matches->push_back({ o->id, o->ownerID, o->returnType, o->mod });
      // if looking only for first matching overload
      if (single)
        return matches;
    }
  }

  if (matches->empty()) {
    delete matches;
    return nullptr;
  }

  return matches;
}

NameResolution
SymbolRegistry::resolveName(const std::string& name)
{
  for (auto& s : _scopes) {
    auto res = s->resolveName(name);
    if (!std::holds_alternative<std::monostate>(res.nameID))
      return res;
  }

  auto nameIt = _fnNameToID.find(name);
  if (nameIt == _fnNameToID.end())
    return {};

  auto globalIt = _functions.find({});
  if (globalIt == _functions.end()) {
    _diag.putMsg(STUB_ERR, 0, 0);
    return {};
  }

  auto globalFnNameIt = globalIt->second.find(nameIt->second);
  if (globalFnNameIt == globalIt->second.end())
    return {};

  return { TypeID{}, globalFnNameIt->first };
}

NameResolution
SymbolRegistry::resolveVarCurScope(const std::string& name)
{
  return _scopes.back()->resolveName(name);
}

TypeID
SymbolRegistry::beginDeclareClass(const std::string& name)
{
  auto id = _typeID;
  _nameToTypeID.emplace(name, id);
  _classes.emplace(id, ClassInfo{});
  ++_typeID;

  return id;
}

TypeID
SymbolRegistry::finishDeclareClass(TypeID classID,
                                   bool isPrimitive,
                                   bool isValueImmutable,
                                   const std::vector<MethodDeclInfo>& methods,
                                   const std::vector<FieldDeclInfo>& fields)
{
  // TODO: differentiate between errors which can be produced
  auto classIt = _classes.find(classID);
  if (classIt == _classes.end())
    return {};

  classIt->second.isPrimitive = isPrimitive;
  classIt->second.isValueImmutable = isValueImmutable;

  auto fieldsMapPair = _fields.try_emplace(classID);
  if (!fieldsMapPair.second)
    return {};

  auto fieldsIt = &fieldsMapPair.first->second;
  for (auto& f : fields)
    fieldsIt->try_emplace(f.name, f.info);

  auto methodsMapPair = _methods.try_emplace(classID);
  if (!methodsMapPair.second)
    return {};

  auto methodsIt = &methodsMapPair.first->second;
  for (auto& m : methods) {
    auto outer = methodsIt->try_emplace(m.nameID);
    auto inner = outer.first->second;
    inner.push_back(m.id);
  }

  return classID;
}

TypeID
SymbolRegistry::resolveClass(const std::string& name)
{
  auto it = _nameToTypeID.find(name);
  if (it != _nameToTypeID.end())
    return it->second;

  return {};
}

VarID
SymbolRegistry::declareVariable(const std::string& name,
                                TypeID type,
                                Modifier mod)
{
  return _scopes.back()->declare(name, type, mod);
}

FnNameID
SymbolRegistry::resolveMethodName(TypeID ownerID, const std::string& name)
{
  auto nameIt = _fnNameToID.find(name);
  if (nameIt == _fnNameToID.end())
    return {};

  auto methodsIt = _methods.find(ownerID);
  if (methodsIt == _methods.end())
    return {};

  auto methodNameIt = methodsIt->second.find(nameIt->second);
  if (methodNameIt == methodsIt->second.end())
    return {};

  return methodNameIt->first;
}

VarInfo
SymbolRegistry::resolveField(TypeID ownerID, const std::string& name)
{
  auto fieldsIt = _fields.find(ownerID);
  if (fieldsIt == _fields.end())
    return {};

  auto nameIt = fieldsIt->second.find(name);
  if (nameIt == fieldsIt->second.end())
    return {};

  return nameIt->second;
}

VarInfo*
SymbolRegistry::resolveVariable(VarID id)
{
  return _scopes.back()->resolve(id);
}

VarInfo*
SymbolRegistry::resolveField(VarID id, TypeID ownerID)
{
  auto fieldsIt = _fields.find(ownerID);
  if (fieldsIt != _fields.end()) {
    auto fieldsMap = fieldsIt->second;
    // TODO: optimize this
    for (auto& f : fieldsMap) {
      if (f.second.id == id)
        return &f.second;
    }
  }

  return nullptr;
}

void
SymbolRegistry::pushScope(TypeID ownerID, FunctionID fnID)
{
  _scopes.emplace_back(std::make_unique<Scope>(*this, ownerID));
  if (isValid(fnID)) {
    auto fnInfo = resolveFunction(fnID);
    if (fnInfo) {
      for (size_t i = 0; i < fnInfo->paramNames.size(); ++i) {
        auto varID = _scopes.back()->declare(
          fnInfo->paramNames[i], fnInfo->paramTypes[i], {});
      }
    }
  }
}

void
SymbolRegistry::popScope()
{
  _scopes.pop_back();
}

bool
SymbolRegistry::isGlobalScope()
{
  return _scopes.back()->getOwnerID() == TypeID{};
}

TypeID
SymbolRegistry::getCurScopeOwnerID()
{
  return _scopes.back()->getOwnerID();
}

void
SymbolRegistry::insertFunctionOverload(FunctionInfo& fnInfo,
                                       std::vector<FunctionInfo*>& overloads)
{
  auto cmp = [](const FunctionInfo* a, const FunctionInfo* b) {
    auto pSizeA = a->paramTypes.size();
    auto pSizeB = b->paramTypes.size();

    if (pSizeA != pSizeB)
      return pSizeA < pSizeB;

    // builtin popcount has parameter type of unsigned int, consider that when
    // changing bitmask type
    return __builtin_popcount(a->bitMask) < __builtin_popcount(b->bitMask);
  };

  auto it = std::lower_bound(overloads.begin(), overloads.end(), &fnInfo, cmp);
  overloads.insert(it, &fnInfo);
}