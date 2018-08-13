/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */

#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "screen.h"
#include "timer.h"

// void kernel_main(struct multiboot *mboot_ptr) {
void kernel_main() {
  init_gdt();
  init_idt();
  screen_clear();
  screen_printf("Hello, %s\n", "World");
  screen_printf("The answer is %4d\n", 42);
  screen_printf("And in hexadecimal, 255 + 1 : %02x + %02x = %04x\n", 255, 1,
                255 + 1);

  init_timer(1);
  for (;;) {
    asm volatile("hlt");
  }
}
