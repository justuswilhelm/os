#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BITSET_INDEX(a) ((a) / (sizeof(uint32_t) * 8))
#define BITSET_OFFSET(a) ((a) % (sizeof(uint32_t) * 8))
#define BITSET_MASK(offset) (0x1 << (offset))
#define BITSET_SIZE(num_values) ((num_values) / (sizeof(uint32_t) * 8))

struct bitset {
  uint32_t *values;
  size_t size;
};

struct bitset_find_free_result {
  size_t index;
  bool success;
};

void bitset_set(struct bitset *, size_t);
void bitset_clear(struct bitset *, size_t);
bool bitset_test(struct bitset *, size_t);
struct bitset_find_free_result bitset_find_free(struct bitset *);
