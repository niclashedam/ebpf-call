#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ubpf.h>
#include <unistd.h>

#include "exts.h"
#include "loader.h"

static struct bpf_slot slots[UBPF_ENGINES];

void exit_handler(void) {
  for (int i = 0; i < UBPF_ENGINES; i++) {
    unexpose(&slots[i]);
  }
}

int main(int argc, char *argv[]) {
  atexit(exit_handler);
  for (int i = 0; i < UBPF_ENGINES; i++) {
    slots[i].engine = ubpf_create();
    slots[i].data = calloc(1, UBPF_DATA_BUF);
    slots[i].data = calloc(1, UBPF_PROG_BUF);

    for (int j = 0; j < sizeof(functions) / sizeof(struct ext_func); j++) {
      ubpf_register(slots[i].engine, functions[j].idx, functions[j].name,
                    functions[j].ref);
    }

    expose(&slots[i]);
  }

  return 0;
}
