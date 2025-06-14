#include "arena.h"
#include <cstdlib>

void
arenaInit(Arena** ptr, size_t cap)
{
  *ptr = (Arena*)malloc(sizeof(Arena));
  Arena* a = *ptr;

  if (a->buffer)
    free(a->buffer);

  a->buffer = (char*)malloc(cap);
  a->capacity = cap;
  a->offset = 0;
  a->freeBlocks = std::unordered_map<size_t, std::unordered_set<void*>>();
}

void*
arenaAlloc(Arena* a, size_t blockSize)
{
  if (a->offset + blockSize > a->capacity) {
    // either grow, or error out
    return NULL;
  }

  void* ptr = a->buffer + a->offset;
  // align to 8 bytes
  a->offset += (blockSize + 7) & ~7;
  return ptr;
}

void
arenaRelease(Arena* a, void* ptr, size_t blockSize)
{
  int size = (blockSize + 7) & ~7;
  auto inner = a->freeBlocks.try_emplace(size);
  inner.first->second.insert(ptr);
}

void
arenaResetFreeBlock(Arena* a, size_t blockSize, void* ptr)
{
  auto it = a->freeBlocks.find(blockSize);
  if (it != a->freeBlocks.end()) {
    auto blockIt = it->second.find(ptr);
    it->second.erase(blockIt);
  }
}

void
arenaReset(Arena* a)
{
  a->offset = 0;
}

void
arenaDestroy(Arena* a)
{
  free(a->buffer);
  a->buffer = NULL;
}

size_t
arenaGetOffset(Arena* a)
{
  return a->offset;
}

void
arenaSetOffset(Arena* a, size_t newOffset)
{
  a->offset = newOffset;
}