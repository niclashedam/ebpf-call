#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ubpf.h>
#include <unistd.h>

#include "exts.h"

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

#define UBPF_ENGINES 1
#define UBPF_DATA_BUF 32 * MB
#define UBPF_PROG_BUF 32 * MB


int main(int argc, char *argv[]) {
  struct ubpf_vm *engines[UBPF_ENGINES];
  void *dataz = calloc(UBPF_ENGINES, UBPF_DATA_BUF);
  void *progz = calloc(UBPF_ENGINES, UBPF_PROG_BUF);

  for (int i = 0; i < UBPF_ENGINES; i++) {
    engines[i] = ubpf_create();

    for (int i = 0; i < sizeof(functions) / sizeof(struct ext_func); i++) {
      ubpf_register(engines[i], functions[i].idx, functions[i].name,
                    functions[i].ref);
    }
  }

  return 0;
}
