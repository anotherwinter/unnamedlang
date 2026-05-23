#include "middle/ssastate.h"
#include "middle/tacbuilder.h"

using namespace MIR;

SSAState::SSAState() {}

SSAState::~SSAState() {}

void
SSAState::declare(VarID id, Binding bind)
{
}

void
SSAState::assign(VarID id, ValueID val)
{
}

LiteralID
SSAState::internalize(const char* val)
{
  auto it = _stringsMap.find(val);
  if (it != _stringsMap.end())
    return it->second;

  LiteralID id = _literalID;
  _stringsMap.emplace(val, id);
  ++_literalID;

  return id;
}