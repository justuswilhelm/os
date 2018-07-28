/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */

#include "multiboot.h"
#include "screen.h"

void kernel_main(struct multiboot *mboot_ptr) {
  for (int i = 0; i < 25; i++) {
    screen_puts("This is a test!\n");
  }
  screen_puts("End---asd\n");
  screen_puts("End---asd\n");
}
