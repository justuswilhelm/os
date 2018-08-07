// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
#pragma once
#include <stdint.h>

struct gdt_entry {
  // The lower 16 bits of the limit.
  uint16_t limit_low;
  // The lower 16 bits of the base.
  uint16_t base_low;
  // The next 8 bits of the base.
  uint8_t base_middle;
  // Access flags, determine what ring this segment can be used in.
  uint8_t access;
  uint8_t granularity;
  // The last 8 bits of the base.
  uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
  // The upper 16 bits of all selector limits.
  uint16_t limit;
  // The address of the first gdt_entry_t struct.
  uint32_t base;
} __attribute__((packed));

void init_gdt();
