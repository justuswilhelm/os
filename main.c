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

void test_paging() {
  // this is paged in by default
  screen_printf("The value at 0x3e0000 is %x\n", *(uint32_t *)0x3E0000);

  // try creating a new page pointing to virtual 0x400000
  // page =
  struct page *pg = get_page(0x400000, get_current_directory());
  write_page(pg, true, true);
  screen_printf("The value at 0x400000 is %x\n", *(uint32_t *)0x400000);
  screen_printf("Now triggering a page fault\n");
  screen_printf("The value at 0x800000 is %x\n", *(uint32_t *)0x800000);
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
  test_paging();
  // test_bitset();

  // init_timer(1);
  init_keyboard();
  for (;;) {
    asm volatile("hlt");
  }
}
