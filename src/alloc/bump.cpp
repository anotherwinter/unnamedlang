#include "bump.h"
#include <cstdio>
#include <cstdlib>

BumpAlloc::BumpAlloc(size_t cap)
{
  _begin = (char*)malloc(cap);
  _offset = 0;
  _cap = cap;
}

BumpAlloc::~BumpAlloc()
{
  free(_begin);
}

void*
BumpAlloc::alloc(size_t n, size_t align)
{
  if (align > alignof(std::max_align_t)) {
    fprintf(stderr, "bumpAlloc: unsupported alignment of %zu\n", align);
    return nullptr;
  }

#ifdef DBG
  printf("bumpAlloc: Allocating block for %lu bytes, offset before %lu\n",
         n,
         a->offset);
#endif

  void* ptr = _begin + _offset;
  if (ptr > _begin + _cap) {
    fprintf(stderr, "bumpAlloc: capacity exceeded\n");
    return nullptr;
  }

  // align
  _offset += (n + align - 1) & ~(align - 1);

#ifdef DBG
  printf("bumpAlloc: offset after allocating: %lu\n", a->offset);
#endif

  return ptr;
}

void
BumpAlloc::reset()
{
  _offset = 0;
}

#ifdef DBG
size_t
BumpAlloc::getOffset(void* ptr)
{
  return (char*)ptr - _begin;
}
#endif