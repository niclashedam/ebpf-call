#include <stdio.h>
#include <stdlib.h>

/* Read a whole file to a buffer */
int file_read(void *buffer, int size, char *filename);

struct File {
  FILE *identifier;
  unsigned long pos;
  unsigned long read;
};

struct File *openFile(char *filename);
void closeFile(struct File *f);
char *getLine(struct File *f);
char *getAll(struct File *f);
