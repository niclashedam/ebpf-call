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
    {.idx = 00, .name = "test", &test},

    /* Print Module */
    {.idx = 10, .name = "prints", &prints},
    {.idx = 11, .name = "printi", &printi},
    {.idx = 12, .name = "printc", &printc},

    /* File Module */
    {.idx = 20, .name = "file_read", &file_read}};
