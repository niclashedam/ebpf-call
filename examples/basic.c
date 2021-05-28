#include "../src/exts.h"

int prog(void *mem, size_t mem_size) {
  char *state = mem;
  state[0] = '1';
  state[1] = '\0';

  return test();
}
