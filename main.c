/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#define VGA_ADDR 0xB8000

// Location of VGA buffer
volatile uint16_t *vga_buffer = (uint16_t *)VGA_ADDR;

int term_col = 0;

void print(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    char c = str[i];
    switch (c) {
    default: {
      const size_t index = term_col;
      vga_buffer[index] = ((uint16_t)0x0F << 8) | c;
      term_col++;
      break;
    }
    }
  }
}

void kernel_main(struct multiboot *mboot_ptr) { print("This is a test!"); }
