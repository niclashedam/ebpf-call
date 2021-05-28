#include <pthread.h>
#include <stdint.h>

#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

#define UBPF_ENGINES 4
#define UBPF_DATA_BUF 1 * MB
#define UBPF_PROG_BUF 1 * MB

struct bpf_slot {
  int i;

  struct ubpf_vm *engine;
  void *data;
  void *prog;

  char *trigger;
  char *status;
  char *load;

  int64_t ret_load;
  int64_t ret_exec;

  pthread_t tid;
  pthread_cond_t *cond;
  pthread_mutex_t *lock;
};

int bpf_start();
int bpf_expose(struct bpf_slot *slot);
void bpf_unexpose(struct bpf_slot *slot);
void bpf_end();
