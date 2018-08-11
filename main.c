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
  for (int i = 0; i < 20; i++) {
    screen_puts("This is a test!\n");
  }
  screen_printf("Hello, %s\n", "World");
  screen_printf("The answer is %4d\n", 42);
  screen_printf("And in hexadecimal, 255 + 1 : %02x + %02x = %04x\n", 255, 1,
                255 + 1);
  asm volatile("int $0x3");
  asm volatile("int $0x4");

  init_timer(1);
}
