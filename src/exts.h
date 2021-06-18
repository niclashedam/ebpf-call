#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "modules/file.h"
#include "modules/memory.h"
#include "modules/print.h"
#include "modules/test.h"
#include "modules/strings.h"

struct ext_func {
  uint8_t idx;
  char name[64];
  void *ref;
};

static struct ext_func functions[] = {
    /* Test Module */
    {.idx = 0, .name = "test", &test},

    /* Print Module */
    {.idx = 10, .name = "prints", &prints},
    {.idx = 11, .name = "printi", &printi},
    {.idx = 12, .name = "printc", &printc},
    {.idx = 13, .name = "printp", &printp},

    /* File Module */
    {.idx = 20, .name = "file_read", &file_read},

    /* Strings */
    {.idx = 30, .name = "strtok", &strtok},
    {.idx = 31, .name = "strtok_r", &strtok_r},
    {.idx = 32, .name = "atoi", &atoi},
    {.idx = 33, .name = "atol", &atol},

    /* Memory */
    {.idx = 40, .name = "copy", &copy},
};

