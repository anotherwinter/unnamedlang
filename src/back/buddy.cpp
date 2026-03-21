#include "buddy.h"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>

BuddyArena::BuddyArena(size_t cap)
{
  // adjust capacity parameter to be power of 2
  size_t size = ceil(log2(cap));

  _cap = cap;
  char* ptr = (char*)malloc(cap);
  _begin = ptr;
  freeList = new std::vector<std::vector<char*>>();
  busyList = new std::map<char*, size_t>();

  // resize to power + 1 to align vector indices with power
  freeList->resize(size + 1);
  freeList->at(size).push_back(ptr);
}

BuddyArena::~BuddyArena()
{
  free(_begin);

  delete freeList;
  delete busyList;
}

void*
BuddyArena::alloc(size_t n)
{
  size_t size = ceil(log2(n));
  size_t current = size;
  size_t limit = freeList->size();
  if (size > limit) {
    fprintf(stderr,
            "buddy: trying to allocate chunk that is bigger than capacity\n");
    return nullptr;
  }

  while (freeList->at(size).empty()) {
    // if we ended up hitting the limit then there is no free blocks available
    if (current == limit) {
      fprintf(stderr, "buddy: no free blocks\n");
      return nullptr;
    }

    if (!freeList->at(current).empty()) {
      // get pointer from bigger block
      char* leftblock = freeList->at(current).back();
      // remove bigger block from freelist since its getting splitted in two
      // smaller blocks
      freeList->at(current).pop_back();
      --current;

      char* rightblock = leftblock + (1 << current);
      // push both smaller blocks into freelist
      freeList->at(current).push_back(leftblock);
      freeList->at(current).push_back(rightblock);
    }

    else {
      ++current;
    }
  }

  // if we are here then function managed to successfully split/find appropriate
  // block
  char* ptr = freeList->at(size).back();
  freeList->at(size).pop_back();

  // add allocated block to busylist
  busyList->try_emplace(ptr, size);

  return ptr;
}

void
BuddyArena::release(void* ptr)
{
  auto it = busyList->find((char*)ptr);
  if (it == busyList->end()) {
    fprintf(stderr,
            "arenaRelease: ptr %p wasn't allocated (possible double release)\n",
            ptr);
    return;
  }

  size_t size = it->second;
  size_t offset = (size_t)ptr - (size_t)_begin;
  char* buddy = _begin + (offset ^ (1 << size));
  std::vector<char*>& list = freeList->at(size);
  // try to find buddy for merging
  for (size_t i = 0; i < list.size(); ++i) {
    // if found buddy, erase it from freelist and merge into bigger block, put
    // it into freelist
    if (list[i] == buddy) {
      list.erase(list.begin() + i);
      char* biggerBlock = buddy < ptr ? buddy : (char*)ptr;
      freeList->at(size + 1).push_back(biggerBlock);

#ifdef DBG
      printf("Found buddy at offset %lu, merging into bigger block with offset "
             "%lu, size %lu\n",
             buddyGetOffset(a, buddy),
             buddyGetOffset(a, biggerBlock),
             (1 << (size + 1)));
#endif

      return;
    }
  }

  freeList->at(size).push_back((char*)ptr);
}

#ifdef DBG
size_t
buddyGetOffset(BuddyArena* a, void* ptr)
{
  return (size_t)ptr - (size_t)begin;
}
#endif