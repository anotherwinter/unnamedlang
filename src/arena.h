#pragma once
#include <cstddef>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef struct
{
  char* buffer;
  size_t capacity;
  size_t offset;
  // map of blocks that are free, with size of block as key
  std::unordered_map<size_t, std::unordered_set<void*>> freeBlocks;
} Arena;

void
arenaInit(Arena** ptr, size_t cap);
void*
arenaAlloc(Arena* a, size_t blockSize);
void
arenaRelease(Arena* a, void* ptr, size_t blockSize);
void
arenaResetFreeBlock(Arena* a, size_t blockSize, void* ptr);
void
arenaReset(Arena* a);
void
arenaDestroy(Arena* a);
size_t
arenaGetOffset(Arena* a);
void
arenaSetOffset(Arena* a, size_t newOffset);