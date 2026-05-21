#include "front/scope.h"
#include "front/symbolregistry.h"

Scope::Scope(TypeID ownerID)
  : _ownerID(ownerID)
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

VarInfo*
Scope::resolve(const std::string name)
{
  auto varID = resolveID(name);
  if (isValid(varID)) {
    auto varInfoIt = _variables.find(varID);
    if (varInfoIt != _variables.end())
      return &varInfoIt->second;
  }

  return {};
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