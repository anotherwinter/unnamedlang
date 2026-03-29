#include "arena.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <new>

ArenaAlloc::ArenaAlloc(size_t cap)
  : _blockSize(cap)
{
  _blocks.reserve(8);
  reset();
}

ArenaAlloc::~ArenaAlloc()
{
  for (auto& b : _blocks) {
    std::free(b._begin);
    b._begin = nullptr;
    b._offset = 0;
    b._cap = 0;
  }
}

inline size_t
alignUp(size_t n, size_t align)
{
  return (n + (align - 1)) & ~(align - 1);
}

void*
ArenaAlloc::alloc(size_t n, size_t align)
{
  Block* b = &_blocks.back();

  size_t alignedOffset = alignUp(b->_offset, align);

  if (alignedOffset + n > b->_cap) {
    size_t cap = std::max(_blockSize, alignUp(n, align));
    char* mem = static_cast<char*>(std::malloc(cap));

    if (!mem)
      throw std::bad_alloc();

    _blocks.push_back({ mem, 0, cap });
    b = &_blocks.back();
    alignedOffset = 0;
  }

  void* ptr = b->_begin + alignedOffset;
  b->_offset = alignedOffset + n;

  return ptr;
}

void
ArenaAlloc::reset()
{
  for (auto& b : _blocks)
    std::free(b._begin);

  _blocks.clear();
  char* mem = static_cast<char*>(std::malloc(_blockSize));

  if (!mem)
    throw std::bad_alloc();

  _blocks.push_back({ mem, 0, _blockSize });
}