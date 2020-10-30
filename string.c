#include <stddef.h>

#include "string.h"

void *memset(void *dest, int ch, size_t count) {
  unsigned char *dest_cp = dest;

  while (count > 0) {
    *dest_cp = (unsigned char)ch;
    dest_cp++;
    count--;
  }

  return dest;
}
