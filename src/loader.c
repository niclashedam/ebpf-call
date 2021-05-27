#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ubpf.h>
#include <unistd.h>
#include <pthread.h>

#include "exts.h"
#include "loader.h"

static struct bpf_slot slots[UBPF_ENGINES];

void exit_handler(void) {
  for (int i = 0; i < UBPF_ENGINES; i++) {
    pthread_join(slots[i].tid, NULL);

    bpf_unexpose(&slots[i]);
    ubpf_destroy(slots[i].engine);
  }

  bpf_end();
}

void *bpf_exec(void *arg) {
  char *errmsg;
  struct bpf_slot *slot = (struct bpf_slot *) arg;

  while(1){
    pthread_cond_wait(slot->cond, slot->lock);
    slot->status = 1;

    if (slot->load) {
      ubpf_unload_code(slot->engine);
      slot->ret_load = ubpf_load_elf(slot->engine, slot->prog, UBPF_PROG_BUF, &errmsg);

      if (slot->ret_load < 0) printf("BPF load error on slot %i, ret = %i", slot->i, slot->ret_load);
    }

    slot->ret_exec = ubpf_exec(slot->engine, slot->data, UBPF_DATA_BUF);

    slot->status = 0;
  }
}


int main(int argc, char *argv[]) {
  atexit(exit_handler);
  bpf_start();

  for (int i = 0; i < UBPF_ENGINES; i++) {
    slots[i].i = i;
    slots[i].engine = ubpf_create();
    slots[i].data = calloc(1, UBPF_DATA_BUF);
    slots[i].prog = calloc(1, UBPF_PROG_BUF);
    slots[i].load = 1; // Trigger a load of any new code

    pthread_mutex_init(slots[i].lock, NULL);
    pthread_cond_init(slots[i].cond, NULL);

    for (int j = 0; j < sizeof(functions) / sizeof(struct ext_func); j++) {
      ubpf_register(slots[i].engine, functions[j].idx, functions[j].name,
                    functions[j].ref);
    }

    pthread_create(&slots[i].tid, NULL, &bpf_exec, &slots[i]);

    bpf_expose(&slots[i]);
  }

  while(1){
    for (int i = 0; i < UBPF_ENGINES; i++) {
      if(!slots[i].status && slots[i].trigger) {
        slots[i].trigger = 0;
        pthread_cond_signal(slots[i].cond);
      }
    }

    usleep(1000);
  }

  return 0;
}
