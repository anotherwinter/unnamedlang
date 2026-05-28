#include "middle/ssastate.h"
#include "middle/tacbuilder.h"

using namespace MIR;

SSAState::SSAState() {}

SSAState::~SSAState() {}

SSAIdentityState
SSAState::getIdentityState(SSAIdentityID id, const char* name)
{
  // TODO: change this
  SSAIdentity identity = { 0, 0, 0 };
  auto it = _identities.find(id);
  if (it != _identities.end())
    return { it->second.id, it->second.ver };

  if (name)
    identity.nameID = internalize(name);

  identity.id = id;
  _identities.emplace(identity.id, identity);
  return { identity.id, identity.ver };
}

SSAVersion
SSAState::assign(SSAIdentityID id, TACValue val)
{
  // TODO: change this
  auto identityIt = _identities.find(id);
  if (identityIt == _identities.end())
    return {};

  auto identityVersions = _identityVals.try_emplace(id);

  identityVersions.first->second.push_back(val);
  SSAVersion newVer = identityVersions.first->second.size() - 1;
  identityIt->second.ver = newVer;
  _scopes.back()._bindings.emplace(id, newVer);

  return newVer;
}

const char*
SSAState::getIdentityStr(SSAIdentityID id)
{
  auto it = _identities.find(id);
  if (it == _identities.end())
    return nullptr;

  return _stringStorage[it->second.nameID.val];
}

LiteralID
SSAState::internalize(const char* val)
{
  auto it = _stringsMap.find(val);
  if (it != _stringsMap.end())
    return it->second;

  LiteralID id = _literalID;
  val = strdup(val);
  _stringsMap.emplace(val, id);
  _stringStorage.push_back(val);
  ++_literalID;

  return id;
}

void
SSAState::pushScope()
{
  _scopes.push_back({});
}

void
SSAState::popScope()
{
  if (_scopes.empty())
    return;

  for (auto& b : _scopes.back()._bindings) {
    auto prevIt = _identities.find(b.first);
    for (auto it = std::next(_scopes.rbegin()); it != _scopes.rend(); ++it) {
      auto bindingIt = it->_bindings.find(b.first);
      if (bindingIt != it->_bindings.end()) {
        prevIt->second.ver = bindingIt->second;
        break;
      }
    }
  }

  _scopes.pop_back();
}