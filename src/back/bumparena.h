#pragma once
#include <cstddef>

class BumpArena
{
public:
  BumpArena(size_t cap);
  ~BumpArena();
  void* alloc(size_t n, size_t align);
  void reset();

#ifdef DBG
  size_t getOffset(BumpArena* a, void* ptr);
#endif

private:
// buffer
  char* _begin;
  // current offset into buffer
  size_t _offset;
  
  // capacity of arena
  size_t _cap;
};