#include <cstdint>
#include <cstdio>

#define FRAME_ALLOC_MAX_MEM 1024

struct FrameAllocator {
  uint8_t memory[FRAME_ALLOC_MAX_MEM];
  uint8_t* ptr;

  FrameAllocator() {
    freeAll();
  }

  void* allocate(std::size_t size);

  void freeAll() {
    for (int i = 0; i < FRAME_ALLOC_MAX_MEM; i++) {
      memory[i] = 0;
    }

    ptr = memory;
  }
};

