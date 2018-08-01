/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */

#include "multiboot.h"
#include "screen.h"

void kernel_main(struct multiboot *mboot_ptr) {
  screen_clear();
  for (int i = 0; i < 20; i++) {
    screen_puts("This is a test!\n");
  }
  screen_printf("Hello, %s\n", "World");
  screen_printf("The answer is %4d\n", 42);
  screen_printf("And in hexadecimal, 255 + 1 : %x + %x = %x\n", 255, 1,
                255 + 1);
}
