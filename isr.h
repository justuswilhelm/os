// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
#pragma once
#include <stdint.h>

struct registers {
  // Data segment selector
  uint32_t ds;
  // Pushed by pusha.
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  // Interrupt number and error code (if applicable)
  uint32_t int_no, err_code;
  // Pushed by the processor automatically.
  uint32_t eip, cs, eflags, useresp, ss;
};

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
