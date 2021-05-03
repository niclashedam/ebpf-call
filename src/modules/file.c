#include <stdio.h>
#include <stdlib.h>

int file_read(void *buffer, int size, char *filename) {
  int i = 0;
  int fsize;
  char num;
  char *buf = (char *)buffer;

  FILE *f = fopen(filename, "rb");

  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  fread(buf, 1, (fsize < size - 1) ? fsize : size - 1, f);
  fclose(f);

  buf[(fsize < size - 1) ? fsize : size] = 0;

  return (fsize < size - 1) ? fsize : size;
}
