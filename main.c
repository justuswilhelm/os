/* Inspired by https://wiki.osdev.org/User:Zesterer/Bare_Bones#kernel.c */

#include "bitset.h"
#include "fs.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "screen.h"
#include "timer.h"

void kernel_end(void);

void check_initrd(struct multiboot_info *mbi) {
  uintptr_t heap_start = 0;
  for (unsigned i = 0; i < mbi->mods_count; i++) {
    struct multiboot_mod_list *mod = get_multiboot_module(mbi, i);
    screen_printf("Multiboot module %s at [0x%x, 0x%x]\n", mod->cmdline,
                  mod->mod_start, mod->mod_end);
    heap_start = mod->mod_end;
  }
  if (heap_start == 0) {
    heap_start = (uint32_t)&kernel_end;
  }
  screen_printf("Placing heap at 0x%x\n", heap_start);
  set_heap_start(heap_start);
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

void test_fs() {
  struct file *file = open("test.txt");
  screen_printf("open() returns: 0x%x\n", file);
  char buffer[32] = {0};
  size_t ret = read(file, buffer, 12);
  screen_printf("read(): %d, %s\n", ret, buffer);
}

// void kernel_main() {
void kernel_main(uint32_t magic, struct multiboot_info *mboot_ptr) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    screen_printf("Invalid bootloader magic: 0x%x", magic);
  }
  init_gdt();
  init_idt();
  screen_clear();
  check_initrd(mboot_ptr);
  init_paging();
  test_fs();
  // test_paging();
  // test_bitset();

  // init_timer(1);
  init_keyboard();
  for (;;) {
    asm volatile("hlt");
  }
}
