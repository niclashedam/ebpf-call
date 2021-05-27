#include <stdint.h>

#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

#define UBPF_ENGINES 4
#define UBPF_DATA_BUF 1 * MB
#define UBPF_PROG_BUF 1 * MB

struct bpf_slot {
  struct ubpf_vm *engine;
  void *data;
  void *prog;
  uint8_t trigger;
  uint8_t status;
};

int expose(struct bpf_slot *slot);
int unexpose(struct bpf_slot *slot);
