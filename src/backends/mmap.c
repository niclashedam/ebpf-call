#include "../loader.h"
#include <stdio.h>
#include <stdlib.h>

int expose(struct bpf_slot *slot) {
  printf("Exposing slot to filesystem .. \n");
  return 0;
}

int unexpose(struct bpf_slot *slot) {
  printf("Unexposing slot to filesystem .. \n");
  return 0;
}
