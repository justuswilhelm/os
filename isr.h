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

void isr0();
void isr1();
void isr2();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// Enables registration of callbacks for interrupts or IRQs.
typedef void (*isr_t)(struct registers);
extern isr_t interrupt_handlers[256];
