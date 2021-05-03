#include "../src/exts.h"

int prog(void *mem, size_t mem_size) {
  int *state = (int *) mem;
  if (*state == 0)
    *state = 1;
  else
    *state *= 2;

  return *state;
}
