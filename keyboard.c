#include <stdint.h>

#include "cpu.h"
#include "irq.h"
#include "screen.h"

static void keyboard_callback(__attribute__((unused)) struct registers regs) {
  uint8_t c = inb(0x60);
  screen_printf("Keyboard: %d\n", c);
}
void init_keyboard() { register_interrupt_handler(IRQ1, &keyboard_callback); }
