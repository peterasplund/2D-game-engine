#include "memory.h"

size_t padByte(size_t size) { return 8 - (size % 8); }

void *FrameAllocator::allocate(std::size_t size) {
  size = padByte(size);

  uint8_t *old_ptr = ptr;

  if (ptr - memory + size > FRAME_ALLOC_MAX_MEM) {
    return nullptr;
  }

  ptr += size;
  return old_ptr;
}
