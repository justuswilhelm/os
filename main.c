/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */

#include "bitset.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "screen.h"
#include "timer.h"

void test_print() {
  screen_printf("Hello, %s\n", "World");
  screen_printf("The answer is %4d\n", 42);
  screen_printf("Printing negative numbers! %04d\n", -42);
  screen_printf("And in hexadecimal, 255 + 1 : %02x + %02x = %04x\n", 255, 1,
                255 + 1);
}

void test_bitset() {
  // 2 * 4 * 8 bits = 64 values
  uint32_t values[2] = {0};
  struct bitset bs = {.values = values, .size = 64};
  screen_printf("Test 0: %d\n", bitset_test(&bs, 0));
  screen_printf("Set 0...\n");
  bitset_set(&bs, 0);
  screen_printf("Test 0: %d\n", bitset_test(&bs, 0));
  screen_printf("Set 1...\n");
  bitset_set(&bs, 1);
  screen_printf("Next free: %d\n", bitset_find_free(&bs).index);
  screen_printf("Clear 0...\n");
  bitset_clear(&bs, 0);
  screen_printf("Test 0: %d\n", bitset_test(&bs, 0));

  screen_printf("Test 63: %d\n", bitset_test(&bs, 63));
  screen_printf("Set 63...\n");
  bitset_set(&bs, 63);
  screen_printf("Test 63: %d\n", bitset_test(&bs, 63));
  screen_printf("Clear 63...\n");
  bitset_clear(&bs, 63);
  screen_printf("Test 63: %d\n", bitset_test(&bs, 63));
}

// void kernel_main(struct multiboot *mboot_ptr) {
void kernel_main() {
  init_gdt();
  init_idt();
  init_paging();
  screen_clear();
  test_print();

  screen_printf("The value is %x", *(uint32_t *)0xA0000000);
  // test_bitset();

  // init_timer(1);
  init_keyboard();
  for (;;) {
    asm volatile("hlt");
  }
}
