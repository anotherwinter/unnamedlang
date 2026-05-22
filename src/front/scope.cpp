#include "front/scope.h"
#include "front/symbolregistry.h"

Scope::Scope(SymbolRegistry& reg, TypeID ownerID)
  : _reg(reg)
  , _ownerID(ownerID)
  , _arena()
{
}

void
Scope::reset()
{
  _variables.clear();
  _nameToVarID.clear();
  _arena.reset();
}

VarID
Scope::declare(const std::string& name, TypeID type, Modifier mod)
{
  VarInfo info = { _varID, type, mod };
  ++_varID;
  _variables.insert({ info.id, info });
  _nameToVarID.insert({ name, info.id });

  return info.id;
}

NameResolution
Scope::resolveName(const std::string& name)
{
  // 1. search in locals
  auto nameIt = _nameToVarID.find(name);
  if (nameIt != _nameToVarID.end()) {
    auto varIt = _variables.find(nameIt->second);
    if (varIt != _variables.end())
      return { {}, varIt->second };
  }

  // 2. search in class members
  if (isValid(_ownerID)) {
    auto field = _reg.resolveField(_ownerID, name);
    if (isValid(field.id))
      return { _ownerID, field };

    auto methodName = _reg.resolveMethodName(_ownerID, name);
    if (isValid(methodName))
      return { _ownerID, methodName };
  }

  return {};
}