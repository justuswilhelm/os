// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kheap.h"
#include "screen.h"

static uintptr_t placement_address = 0;

void set_heap_start(uintptr_t val) {
  if (placement_address) {
    PANIC("Heap start already set");
  }
  placement_address = val;
}

static void set_placement_address(uintptr_t val) { placement_address = val; }

static uintptr_t get_placement_address() {
  if (placement_address == 0) {
    PANIC("Did not set placement address");
  }
  return placement_address;
}

static void *kmalloc_internal(size_t size, uintptr_t align) {
  uintptr_t tmp = get_placement_address();
  // Example calculation
  // align = 4
  // placement = 15
  // then
  // align_rem = 3
  // adjustment = align - align_rem
  // thanks to
  // https://stackoverflow.com/a/12721849/6261168
  if (align) {
    uintptr_t align_rem = tmp % align;
    if (align_rem != 0) {
      uintptr_t adjustment = align - align_rem;
      tmp += adjustment;
    }
  }
  set_placement_address(tmp + size);
  return (void *)tmp;
}

void *kmalloc(size_t size) { return kmalloc_internal(size, false); }

void *kmalloc_a(size_t size, uintptr_t align) {
  return kmalloc_internal(size, align);
}
