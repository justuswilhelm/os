// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kheap.h"

void kernel_end(void);

static uintptr_t placement_address = 0;

static void set_placement_address(uintptr_t val) { placement_address = val; }

static uintptr_t get_placement_address() {
  if (placement_address == 0) {
    placement_address = (uint32_t)&kernel_end;
  }
  return placement_address;
}

static void *kmalloc_internal(size_t size, bool align, uintptr_t *phys) {
  if (align && (get_placement_address() & 0xFFFFF000)) {
    set_placement_address((get_placement_address() & 0xFFFFF000) + 0x1000);
  }
  if (phys) {
    *phys = get_placement_address();
  }
  uintptr_t tmp = get_placement_address();
  set_placement_address(tmp + size);
  return (void *)tmp;
}

void *kmalloc(size_t size) { return kmalloc_internal(size, false, 0); }

void *kmalloc_a(size_t size) { return kmalloc_internal(size, true, 0); }

void *kmalloc_ap(size_t size, uintptr_t *phys) {
  return kmalloc_internal(size, true, phys);
}

void *kmalloc_p(size_t size, uintptr_t *phys) {
  return kmalloc_internal(size, false, phys);
}
