#pragma once
#include "front/symbolregistry.h"
#include "tacbuilder.h"
#include <string>
#include <unordered_map>

namespace MIR {
struct Binding
{
  std::string name;
  std::vector<ValueID> values;
};

struct ScopedBindingInfo
{
  VarID id;
  size_t firstIndex;
};

using VariablesMap = std::unordered_map<std::string, Binding>;
using ValueStorage = std::vector<TACValue>;

struct SSAScope
{
  std::vector<ScopedBindingInfo> _bindings;
};

class SSAState
{
public:
  SSAState();

  inline ValueID makeValueID() { return _valCounter.inc(); }

  void declare(VarID id, Binding bind);
  void assign(VarID id, ValueID val);

private:
  SSAState(const SSAState& other) = delete;
  SSAState(SSAState&& other) = delete;

  VariablesMap _varMap;
  ValueStorage _valStorage;
  ValueIDCounter _valCounter;
};

};