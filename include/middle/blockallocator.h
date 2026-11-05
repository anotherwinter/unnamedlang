#pragma once
#include "alloc/arena.h"
#include "diagnostics.h"
#include "middle/tac.h"
#include <vector>

namespace MIR {

class BlockAllocator
{
public:
  BlockAllocator(Diagnostics& diag)
    : _diag(diag)
  {
  }

  inline void reset()
  {
    for (auto& b : _blocks)
      b->~Block();

    _arena.reset();
    _blockID = { 0 };
  }

  inline Block* alloc()
  {
    void* raw = _arena.alloc(sizeof(Block), alignof(Block));
    if (!raw) {
      _diag.putMsg(STUB_ERR, 0, 0);
      return nullptr;
    }

    Block* b = new (raw) Block();
    b->id = _blockID;
    ++_blockID;

    _blocks.push_back(b);

    return b;
  }

  inline Block* at(size_t idx) { return _blocks.at(idx); }

  inline std::vector<Block*>& getBlocks() { return _blocks; }

  inline Instruction* allocInstruction(Instruction& instr)
  {
    void* raw = _arena.alloc(sizeof(Instruction), alignof(Instruction));
    if (!raw) {
      _diag.putMsg(STUB_ERR, 0, 0);
      return nullptr;
    }

    Instruction* i = new (raw) Instruction();
    *i = instr;

    return i;
  }

private:
  BlockAllocator(const BlockAllocator& other) = delete;
  BlockAllocator(BlockAllocator&& other) = delete;

  Diagnostics& _diag;
  ArenaAlloc _arena;
  BlockID _blockID = { 0 };
  std::vector<Block*> _blocks;
};

}