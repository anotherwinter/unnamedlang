#include "context.h"

Context::Context(Runtime& runtime,
                 const std::unordered_map<std::string, Identifier>& table,
                 ContextFlags flags,
                 Context* prev)
  : _runtime(runtime)
  , _prev(prev)
  , _bump(1024 * 1024)
{
  _table = new std::map<std::string, Identifier>();
  _table->insert(table.begin(), table.end());
}

Context::~Context()
{
  delete _table;
}

void
Context::setExHandler(ObjectHeader handler)
{
}

Identifier*
Context::lookup(const char* name)
{
  Identifier* res = nullptr;
  auto objIt = _table->find(name);
  if (objIt != _table->end())
    res = &objIt->second;

  return res;
}

bool
Context::define(const char* name, Identifier id)
{
  id.locationHandle = 0;
  return _table->try_emplace(name, id).second;
}

void
Context::returnFromFn()
{
  longjmp(_env, 1);
}