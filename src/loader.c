#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ubpf.h>
#include <unistd.h>

#include "exts.h"
#include "file.h"

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

#define UBPF_ENGINES 1
#define UBPF_DATA_BUF 32 * MB
#define UBPF_PROG_BUF 32 * MB

int load_buf(void *prog, size_t prog_size, struct ubpf_vm *engine) {
  char *errmsg;
  int rv = ubpf_load_elf(engine, prog, prog_size, &errmsg);

  if (rv < 0) {
    printf("%s\n", errmsg);
    return 2;
  }

  return rv;
}

int load_file(char *filename, struct ubpf_vm *engine) {
  char *prog = read_program(filename);
  if (prog == 0) {
    printf("file not found\n");
    return 2;
  }

  return load_buf(prog, UBPF_PROG_BUF, engine);
}

int main(int argc, char *argv[]) {
  int interactive = 0;
  char *singular = NULL;
  int arg;

  while ((arg = getopt(argc, argv, "if:")) != -1)
    switch (arg) {
    case 'i':
      interactive = 1;
      break;
    case 'f':
      singular = optarg;
      break;
    case '?':
      if (optopt == 'f')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }

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

  if (singular) {
    if (load_file(singular, engines[0]) > 0) {
      return 2;
    }
    int ret = ubpf_exec(engines[0], dataz, UBPF_DATA_BUF);
    if (ret == UINT64_MAX) {
      printf("Execution failed.\n");
      return 3;
    }

    printf("Return code: %i\n", ret);
    return 0;
  }

  if (interactive) {
    while (1) {
      char *line = malloc(1024);
      printf("> ");
      scanf("%s", line);

      if (strcmp(line, "exit") == 0)
        break;

      if (load_file(line, engines[0]) > 0) {
        return 2;
      }
      int ret = ubpf_exec(engines[0], dataz, UBPF_DATA_BUF);
      if (ret == UINT64_MAX) {
        printf("Execution failed.\n");
        return 3;
      }

      printf("Return code: %i\n", ret);
    }

    for (int i = 0; i < UBPF_ENGINES; i++) {
      ubpf_destroy(engines[i]);
    }
  }

  return 0;
}
