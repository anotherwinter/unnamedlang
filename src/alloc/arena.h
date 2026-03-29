#pragma once
#include <cstddef>
#include <vector>

class ArenaAlloc
{
  struct Block
  {
    // buffer
    char* _begin;
    // current offset into buffer
    size_t _offset;

    // capacity of block
    size_t _cap;
  };

public:
  ArenaAlloc(size_t cap = 4096);
  ~ArenaAlloc();
  void* alloc(size_t n, size_t align = 8);
  void reset();

private:
  std::vector<Block> _blocks;
  size_t _blockSize;
};