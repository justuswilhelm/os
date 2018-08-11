// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
#pragma once
#include <stdint.h>

// A struct describing an interrupt gate.
struct idt_entry {
  // The lower 16 bits of the address to jump to when this interrupt fires.
  uint16_t base_lo;
  // Kernel segment selector.
  uint16_t sel;
  // This must always be zero.
  uint8_t always0;
  // More flags. See documentation.
  uint8_t flags;
  // The upper 16 bits of the address to jump to.
  uint16_t base_hi;
} __attribute__((packed));

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr {
  uint16_t limit;
  // The address of the first element in our idt_entry_t array.
  uint32_t base;
} __attribute__((packed));

void init_idt();
