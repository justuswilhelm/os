// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
#include <stdint.h>

#include "cpu.h"
#include "irq.h"
#include "isr.h"
#include "screen.h"
#include "timer.h"

uint32_t tick = 0;

static void timer_callback(__attribute__((unused)) struct registers regs) {
  tick++;
  screen_printf("Tick: %d\n", tick);
}

void init_timer(uint32_t frequency) {
  // Firstly, register our timer callback.
  register_interrupt_handler(IRQ0, &timer_callback);

  // The value we send to the PIT is the value to divide it's input clock
  // (1193180 Hz) by, to get our required frequency. Important to note is
  // that the divisor must be small enough to fit into 16-bits.
  uint32_t divisor = TIMER_BASE_FREQ / frequency;

  // Send the command byte.
  // https://wiki.osdev.org/Programmable_Interval_Timer#I.2FO_Ports
  // Channel 0 | Access Mode lobyte+hibyte | Operating mode: Mode 3
  outb(TIMER_PORT_3, 0x36);
  outb(0x43, 0x36);

  // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

  // Send the frequency divisor.
  outb(0x40, l);
  outb(0x40, h);
}
