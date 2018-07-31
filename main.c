/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */

#include "multiboot.h"
#include "screen.h"

void kernel_main(struct multiboot *mboot_ptr) {
  screen_clear();
  for (int i = 0; i < 20; i++) {
    screen_puts("This is a test!\n");
  }
  screen_printf("Hello, %s\n", "World");
  screen_printf("The answer is %d\n", 42);
}
