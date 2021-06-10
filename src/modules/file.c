#include <stdio.h>
#include <stdlib.h>

#include "file.h"

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

struct File *openFile(char *filename) {
  struct File *f = (struct File *)malloc(sizeof(struct File));

  f->identifier = fopen(filename, "r");
  f->pos = 0;

  if (f->identifier == NULL)
    return NULL;
  return f;
}

void closeFile(struct File *f) { fclose(f->identifier); }

char *getLine(struct File *f) {
  char *line = NULL;
  size_t len = 0;

  f->read = getline(&line, &len, f->identifier);
  f->pos += f->read;

  return line;
}

char *getAll(struct File *f) {
  fseek(f->identifier, 0, SEEK_END);
  long fsize = ftell(f->identifier);
  fseek(f->identifier, 0, SEEK_SET);

  char *buf = (char *) malloc(fsize + 1);
  fread(buf, 1, fsize, f->identifier);

  buf[fsize] = 0;

  f->read = fsize;
  f->pos = fsize;

  return buf;
}
