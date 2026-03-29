#pragma once
#include <cstddef>

class BumpAlloc
{
public:
  BumpAlloc(size_t cap);
  ~BumpAlloc();
  void* alloc(size_t n, size_t align = 8);
  void reset();

#ifdef DBG
  size_t getOffset(BumpAlloc* a, void* ptr);
#endif

private:
// buffer
  char* _begin;
  // current offset into buffer
  size_t _offset;
  
  // capacity of arena
  size_t _cap;
};