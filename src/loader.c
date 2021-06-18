#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ubpf.h>
#include <unistd.h>

#include "exts.h"
#include "loader.h"

static struct bpf_slot slots[UBPF_ENGINES];
static int terminating = 0;

void exit_handler() { bpf_end(); }

void sig_term_handler(int signum, siginfo_t *info, void *ptr) {
  exit_handler();
}

void *bpf_exec(void *arg) {
  char *errmsg;
  struct bpf_slot *slot = (struct bpf_slot *)arg;

  while (1) {
    pthread_cond_wait(slot->cond, slot->lock);
    *slot->status = '1';

    if (terminating) {
      bpf_unexpose(slot);
      ubpf_destroy(slot->engine);
    }

    if (*slot->load == '1') {
      *slot->load = '0';
      ubpf_unload_code(slot->engine);
      slot->ret_load =
          ubpf_load_elf(slot->engine, slot->prog, UBPF_PROG_BUF, &errmsg);

      if (slot->ret_load < 0) {
        fprintf(stderr, "BPF load error on slot %i, ret = %li, error = %s\n", slot->i,
                slot->ret_load, errmsg);
      }
    }

    slot->ret_exec = ubpf_exec(slot->engine, slot->data, UBPF_DATA_BUF);

    printf("Executed program on slot %i, ret = %li\n", slot->i, slot->ret_exec);

    *slot->status = '0';
  }
}

int main(int argc, char *argv[]) {
  atexit(exit_handler);

  static struct sigaction _sigact;

  memset(&_sigact, 0, sizeof(_sigact));
  _sigact.sa_sigaction = sig_term_handler;
  _sigact.sa_flags = SA_NODEFER;

  sigaction(SIGTERM, &_sigact, NULL);

  int start = bpf_start();

  if (start != 0) {
    fprintf(stderr, "Pre-handler exited with error code %i\n", start);
    exit(start);
  }

  for (int i = 0; i < UBPF_ENGINES; i++) {
    slots[i].i = i;
    slots[i].engine = ubpf_create();

    slots[i].lock = malloc(sizeof(pthread_mutex_t));
    slots[i].cond = malloc(sizeof(pthread_cond_t));

    pthread_mutex_init(slots[i].lock, NULL);
    pthread_cond_init(slots[i].cond, NULL);

    for (int j = 0; j < sizeof(functions) / sizeof(struct ext_func); j++) {
      ubpf_register(slots[i].engine, functions[j].idx, functions[j].name,
                    functions[j].ref);
    }

    int exposed = bpf_expose(&slots[i]);
    if (exposed != 0) {
      fprintf(stderr, "Exposer exited with error code %i\n", exposed);
      exit(exposed);
    }

    memset(slots[i].prog, '\0', UBPF_PROG_BUF);
    memset(slots[i].data, '\0', UBPF_DATA_BUF);
    *slots[i].trigger = '0';
    *slots[i].status = '0';
    *slots[i].load = '1'; // Trigger a load of any new code

    pthread_create(&slots[i].tid, NULL, &bpf_exec, &slots[i]);
  }

  while (1) {
    for (int i = 0; i < UBPF_ENGINES; i++) {
      if (*slots[i].status == '0' && *slots[i].trigger == '1') {
        *slots[i].trigger = '0';
        pthread_cond_signal(slots[i].cond);
      }
    }

    usleep(10000); // 10 ms
  }

  return 0;
}
