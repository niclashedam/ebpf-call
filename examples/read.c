#include "../src/exts.h"

int prog(void *mem, size_t mem_size) {
  char file[64] = "examples/test.txt";
  int bytes_read = file_read(mem, mem_size, file);

  prints(mem);

  return bytes_read;
}
