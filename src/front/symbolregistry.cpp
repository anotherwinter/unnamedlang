#include "front/symbolregistry.h"
#include "diagnostics.h"
#include "front/scope.h"
#include <algorithm>

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
  ++_fnID;
  auto inserted = _fnInfoStorage.emplace(_fnID, fnInfo);

  auto fnsIt = _functions.try_emplace(ownerID);
  auto overloadsIt = fnsIt.first->second.try_emplace(nameID);
  auto maskedOverloadsIt =
    overloadsIt.first->second.try_emplace(makeFnParamBitmask(paramTypes));
  insertFunctionOverload(inserted.first->second,
                         maskedOverloadsIt.first->second);

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

FnNameResolution
SymbolRegistry::resolveFunctionName(const std::string& name)
{
  auto it = _fnNameToID.find(name);
  if (it == _fnNameToID.end())
    return {};

  auto fnNameID = it->second;

  // TODO: add map that will contain fnnameid as key and vector of FnNameResolution as value
  // that way we can avoid iterating over scopes and just try to get last resolution
  // from vector if given fnnameid exists in map

  // resolve in owner scopes
  for (auto& s : _scopes) {
    auto ownerID = s->getOwnerID();
    if (!isValid(ownerID))
      continue;

    auto methodsIt = _methods.find(ownerID);
    if (methodsIt != _methods.end()) {
      auto methodIt = methodsIt->second.find(fnNameID);
      if (methodIt != methodsIt->second.end())
        return { fnNameID, ownerID };
    }
  }

  return { fnNameID, TypeID{} };
}

TypeID
SymbolRegistry::beginDeclareClass(const std::string& name)
{
  auto id = _typeID;
  _nameToTypeID.emplace(name, _typeID);
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

VarResolution
SymbolRegistry::resolveVariable(const std::string& name)
{
  size_t idx = _scopes.size() - 1;
  while (true) {
    auto varInfo = _scopes.at(idx)->resolve(name);
    if (varInfo != nullptr)
      return { *varInfo, _scopes.at(idx)->getOwnerID() };

    if (idx == 0)
      break;

    --idx;
  }

  return {};
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

VarID
SymbolRegistry::resolveVariableCurScope(const std::string& name)
{
  return _scopes.back()->resolveID(name);
}

void
SymbolRegistry::pushScope(TypeID ownerID)
{
  _scopes.emplace_back(std::make_unique<Scope>());
  if (isValid(ownerID)) {
    auto fieldsIt = _fields.find(ownerID);
    if (fieldsIt != _fields.end()) {
      for (auto& f : fieldsIt->second)
        auto varID = declareVariable(f.first, f.second.type, f.second.mod);
    }
  }
}

void
SymbolRegistry::popScope()
{
  _scopes.pop_back();
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