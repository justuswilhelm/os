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
