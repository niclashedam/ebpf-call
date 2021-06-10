#include "../src/exts.h"

int prog(void *mem, size_t mem_size) {
  char filename[64] = "examples/test.txt";

  struct File *file = openFile(filename);
  char *line = getLine(file);
  prints(line);

  closeFile(file);

  return 0;
}
