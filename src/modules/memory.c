#include <string.h>

void *copy(void *dest, const void * src, size_t n){
  memcpy(dest, src, n);
}
