#include "front/scope.h"

Scope::Scope(SymbolRegistry& reg, TypeID ownerID, VarIDCounter* varCounter)
  : _reg(reg)
  , _ownerID(ownerID)
  , _arena()
{
  if (varCounter) {
    _varCounter = varCounter;
    _ownsCounter = false;
  } else {
    _varCounter = new VarIDCounter();
    _ownsCounter = true;
  }
}

Scope::~Scope()
{
  if (_ownsCounter)
    delete _varCounter;
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
  VarInfo info = { _varCounter->inc(), type, mod };
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