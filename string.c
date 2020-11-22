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

int strncmp(const char *lhs, const char *rhs, size_t num) {
  for (size_t i = 0; i < num; i++) {
    if (lhs[i] != rhs[i]) {
      return 1;
    }
  }
  return 0;
}

void *memcpy(void *dest, const void *src, size_t num) {
  char *_dest = dest;
  const char *_src = src;
  for (size_t i = 0; i < num; i++) {
    _dest[i] = _src[i];
  }
  return dest;
}
