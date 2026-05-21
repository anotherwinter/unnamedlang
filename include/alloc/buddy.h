#pragma once
#include <cstddef>
#include <map>
#include <vector>

class BuddyAlloc
{
public:
  BuddyAlloc(size_t cap);
  ~BuddyAlloc();
  void* alloc(size_t n);
  void release(void* ptr);

#ifdef DBG
  size_t getOffset(void* ptr);
#endif

private:
  // buffer
  char* _begin;

  // capacity of arena
  size_t _cap;

  // list of lists of free blocks for given size in power of 2 from 2^0 to
  // ceil(log2(capacity))
  std::vector<std::vector<char*>>* freeList;

  // map of currently used blocks
  std::map<char*, size_t>* busyList;
};