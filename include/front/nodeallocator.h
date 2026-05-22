#pragma once
#include "alloc/arena.h"
#include "front/typedast.h"
#include <variant>

struct ASTNode;
class Diagnostics;

namespace HIR {

struct TypedNode;

class NodeAllocator
{
public:
  NodeAllocator(Diagnostics& diag);

  inline void reset() { _arena.reset(); }

  inline TypedNode* allocTypedNode()
  {
    return allocTypedNode<std::monostate>(nullptr);
  }

  template<typename T>
  inline TypedNode* allocTypedNode(size_t line, size_t col)
  {
    void* raw = _arena.alloc(sizeof(TypedNode), alignof(TypedNode));
    if (!raw) {
      _diag.putMsg(STUB_ERR, line, col);
      return nullptr;
    }

    TypedNode* n = new (raw) TypedNode{ T{}, line, col };

    return n;
  }

  template<typename T>
  inline TypedNode* allocTypedNode(const ASTNode* node)
  {
    if (!node)
      return allocTypedNode<std::monostate>(nullptr);

    return allocTypedNode<T>(node->line, node->col);
  }

private:
  NodeAllocator(const NodeAllocator& other) = delete;
  NodeAllocator(NodeAllocator&& other) = delete;

  Diagnostics& _diag;
  ArenaAlloc _arena;
};

};