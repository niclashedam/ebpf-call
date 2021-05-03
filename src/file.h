#include <stdio.h>
#include <stdlib.h>

inline char *read_program(char *filename) {
  int i = 0;
  char num;

  FILE *f = fopen(filename, "rb");

  if (f == 0) {
    return 0;
  }

  fseek(f, 0, SEEK_END);
  size_t fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  char *prog = malloc(fsize + 1);
  fread(prog, 1, fsize, f);
  fclose(f);

  prog[fsize] = 0;

  return prog;
}
