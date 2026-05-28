#pragma once
#include "middle/tacbuilder.h"
#include <unordered_map>

namespace MIR {
struct ScopedBindingInfo
{
  SSAIdentityID id;
  SSAVersion lastVersion;
};

struct SSAScope
{
  std::unordered_map<SSAIdentityID, SSAVersion> _bindings;
};

struct SSAIdentity
{
  SSAIdentityID id;
  SSAVersion ver;
  LiteralID nameID;
};

using IdentityMap = std::unordered_map<SSAIdentityID, SSAIdentity>;
using IdentityValuesMap =
  std::unordered_map<SSAIdentityID, std::vector<TACValue>>;
using ValueStorage = std::vector<TACValue>;
using StringStorage = std::vector<const char*>;
using StringsMap = std::unordered_map<std::string, LiteralID>;

class SSAState
{
public:
  SSAState();
  ~SSAState();

  inline ValueID makeValueID() { return _valCounter.inc(); }

  inline bool isIdentityDefined(SSAIdentityID id, bool curScope = false)
  {
    auto it = _identities.find(id);
    if (it == _identities.end())
      return false;

    if (curScope) {
      if (_scopes.empty())
        return false;

      auto& curBindings = _scopes.back()._bindings;

      return curBindings.find(it->second.id) != curBindings.end();
    }

    return true;
  }

  inline SSAVersion getCurVersion(SSAIdentityID id)
  {
    auto it = _identities.find(id);
    if (it == _identities.end())
      return {};

    return it->second.ver;
  }

  // add new identity into ssa returns id for new or existing identity
  SSAIdentityState getIdentityState(SSAIdentityID id, const char* name = nullptr);
  SSAVersion assign(SSAIdentityID id, TACValue val);

  const char* getIdentityStr(SSAIdentityID id);

  LiteralID internalize(const char* val);

  void pushScope();

  void popScope();

private:
  SSAState(const SSAState& other) = delete;
  SSAState(SSAState&& other) = delete;

  LiteralID _literalID = { 0 };

  IdentityMap _identities;
  IdentityValuesMap _identityVals;
  ValueIDCounter _valCounter;
  StringStorage _stringStorage;
  StringsMap _stringsMap;

  std::vector<SSAScope> _scopes;
};

};