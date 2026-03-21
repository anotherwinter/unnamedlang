#include "../../src/alloc/buddy.h"
#include "../../src/alloc/bump.h"
#include <cstdio>
#include <random>

#define BLOCK_SIZE 64
#define UNALIGNED_BLOCK_SIZE 50

int
main()
{

  // buddy alloc test
  printf("\n--Testing buddy alloc--\n");
  BuddyArena* buddy = buddyInit(1024);

  // allocate some blocks
  printf("1. Allocate some blocks\n");
  void* buddyBlocks[10];
  for (size_t i = 0; i < 10; ++i) {
    buddyBlocks[i] = buddyAlloc(buddy, BLOCK_SIZE);
    printf("Allocated block at offset %lu\n",
           buddyGetOffset(buddy, buddyBlocks[i]));
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, 9);
  printf("2. Release some blocks\n");
  // release random 5 blocks
  for (size_t i = 0; i < 5; ++i) {
    void* ptr = buddyBlocks[dist(gen)];
    printf("Releasing block at offset %lu with size %lu\n",
           buddyGetOffset(buddy, ptr),
           BLOCK_SIZE);

    buddyRelease(buddy, ptr);
  }

  // allocate some blocks again
  printf("3. Allocate again\n");
  for (size_t i = 0; i < 5; ++i) {
    buddyBlocks[i] = buddyAlloc(buddy, BLOCK_SIZE);
    printf("Allocated block at offset %lu\n",
           buddyGetOffset(buddy, buddyBlocks[i]));
  }

  // bump alloc test

  printf("\n--Testing bump alloc--\n");
  BumpArena* bump = bumpInit(128);

  // allocate some blocks
  printf("1. Allocate some blocks until hit capacity\n");
  void* bumpBlocks[10];
  for (size_t i = 0; i < 10; ++i) {
    buddyBlocks[i] = bumpAlloc(bump, UNALIGNED_BLOCK_SIZE);
    if (buddyBlocks[i]) {
      printf("Allocated block at offset %lu\n",
             bumpGetOffset(bump, buddyBlocks[i]));
    } else {
      printf("Max capacity reached, stopping bump alloc\n");
      break;
    }
  }
}
