#pragma once
#include "../alloc/arena.h"
#include "symbolregistry.h"

class Scope
{
public:
  Scope(TypeID ownerID = {});

  // resolve name into varid
  VarInfo* resolve(const std::string name);

  inline VarID resolveID(const std::string name)
  {
    auto it = _nameToVarID.find(name);
    if (it != _nameToVarID.end())
      return it->second;

    return {};
  }

  // resolve by varid, returns pointer to original struct
  inline VarInfo* resolve(VarID id)
  {
    auto it = _variables.find(id);
    if (it != _variables.end())
      return &it->second;

    return nullptr;
  }

  // get typeid associated with this scope
  inline TypeID getOwnerID() { return _ownerID; }

  void reset();

  // declare variable
  VarID declare(const std::string& name, TypeID type, Modifier mod);

private:
  VarID _varID = { 0 };
  const TypeID _ownerID;

  VariablesMap _variables;
  VariableNametoVarIDMap _nameToVarID;
  ArenaAlloc _arena;
};