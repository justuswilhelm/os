// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
#include "isr.h"
#include "screen.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(struct registers regs) {
  screen_printf("recieved interrupt: %x\n", regs.int_no);
}
