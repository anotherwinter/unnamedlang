#pragma once
#include "front/symbolregistry.h"
#include "middle/tacbuilder.h"
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
using StringStorage = std::vector<const char*>;
using StringsMap = std::unordered_map<const char*, LiteralID>;

struct SSAScope
{
  std::vector<ScopedBindingInfo> _bindings;
};

class SSAState
{
public:
  SSAState();
  ~SSAState();

  inline ValueID makeValueID() { return _valCounter.inc(); }

  void declare(VarID id, Binding bind);
  void assign(VarID id, ValueID val);

  LiteralID internalize(const char* val);

private:
  SSAState(const SSAState& other) = delete;
  SSAState(SSAState&& other) = delete;

  LiteralID _literalID = { 0 };

  VariablesMap _varMap;
  ValueStorage _valStorage;
  ValueIDCounter _valCounter;
  StringStorage _stringStorage;
  StringsMap _stringsMap;
};

};