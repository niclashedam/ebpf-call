#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

void prints(char *buf) { printf("%s\n", buf); }
void printi(int buf) { printf("%i\n", buf); }
void printc(char buf) { printf("%c\n", buf); }
void printp(void *buf) { printf("0x%" PRIXPTR "\n", (uintptr_t) buf); }
