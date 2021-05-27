#include <stddef.h>
#include <stdint.h>

#include "modules/file.h"
#include "modules/print.h"
#include "modules/test.h"

struct ext_func {
  uint8_t idx;
  char name[64];
  void *ref;
};

static struct ext_func functions[] = {
    /* Test Module */
    {.idx = 0, .name = "test", &test},

    /* Print Module */
    {.idx = 1, .name = "prints", &prints},
    {.idx = 2, .name = "printi", &printi},
    {.idx = 3, .name = "printc", &printc},

    /* File Module */
    {.idx = 4, .name = "file_read", &file_read}};
