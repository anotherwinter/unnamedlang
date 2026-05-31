#pragma once
#include "alloc/arena.h"
#include "front/symbolregistry.h"

struct VarIDCounter
{
public:
  inline VarID inc()
  {
    VarID tmp = val;
    ++val;
    return tmp;
  }

  inline void reset() { val = { 0 }; }

private:
  VarID val = { 0 };
};

class Scope
{
public:
  Scope(SymbolRegistry& reg,
        TypeID ownerID = {},
        VarIDCounter* varCounter = nullptr);
  ~Scope();

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

  // TODO: change this
  inline VarIDCounter* getCounter() { return _varCounter; }

  void reset();

  // declare variable
  VarID declare(const std::string& name, TypeID type, Modifier mod);

  NameResolution resolveName(const std::string& name);

private:
  SymbolRegistry& _reg;
  const TypeID _ownerID;
  VarIDCounter* _varCounter;
  bool _ownsCounter;

  VariablesMap _variables;
  VariableNametoVarIDMap _nameToVarID;
  ArenaAlloc _arena;
};