#include "bitset.h"

void bitset_set(struct bitset *bs, size_t ix) {
  // Which word in values to set
  size_t values_idx = BITSET_INDEX(ix);
  size_t values_bit = BITSET_OFFSET(ix);
  bs->values[values_idx] |= BITSET_MASK(values_bit);
}

void bitset_clear(struct bitset *bs, size_t ix) {
  // Which word in values to set
  size_t values_idx = BITSET_INDEX(ix);
  size_t values_bit = BITSET_OFFSET(ix);
  bs->values[values_idx] &= ~BITSET_MASK(values_bit);
}

bool bitset_test(struct bitset *bs, size_t ix) {
  // Which word in values to set
  size_t values_idx = BITSET_INDEX(ix);
  size_t values_bit = BITSET_OFFSET(ix);
  return bs->values[values_idx] & BITSET_MASK(values_bit);
}

struct bitset_find_free_result bitset_find_free(struct bitset *bs) {
  for (size_t i = 0; i < bs->size; i += 1) {
    if (!bitset_test(bs, i)) {
      return (struct bitset_find_free_result){.index = i, .success = true};
    }
  }
  return (struct bitset_find_free_result){.index = -1, .success = false};
}
