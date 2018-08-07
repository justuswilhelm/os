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

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void init_idt();