// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
#include <stdbool.h>
#include <stdint.h>

#include "bitset.h"
#include "irq.h"
#include "kheap.h"
#include "paging.h"
#include "screen.h"
#include "string.h"

struct page_table *boot_table = NULL;
struct page_directory *kernel_directory = NULL;
struct page_directory *current_directory = NULL;

// How many pages do we want to make available?
// Static for now
// 4 * 4 MiB
// This will result in 128 bytes being allocated
uint32_t pages_bitset_bits[BITSET_SIZE(1024 * 4)] = {0};
struct bitset pages_bitset = {.values = pages_bitset_bits, .size = 1024 * 4};

static uint32_t page_table_ptr_to_pde(void *table) {
  return (((uint32_t)table) & 0xfffff000) | 0x7;
}

// Return memory address of new page
static void *allocate_page() {
  struct bitset_find_free_result result = bitset_find_free(&pages_bitset);
  if (!result.success) {
    PANIC("No pages available");
  }
  bitset_set(&pages_bitset, result.index);
  return (void *)(result.index * 4096);
}

static struct page_table *allocate_page_table(uint32_t table_idx,
                                              struct page_directory *dir) {
  // Allocate table entry in virtual memory
  struct page_table *table = kmalloc_a(sizeof(struct page_table), PAGE_SIZE);
  memset(table, 0, sizeof(struct page_table));
  // Update physical pointer with identity mapped adddress
  dir->tables_physical[table_idx] = page_table_ptr_to_pde(table);
  // This is in virtual memory anyway
  dir->tables[table_idx] = table;
  return table;
}

struct page *get_page(uintptr_t address, struct page_directory *dir) {
  // Turn the address into an index. Identifies the table in directory.
  address /= 0x1000;
  // Find the page table containing this address.
  uint32_t table_idx = address / 1024;
  uint32_t idx = address % 1024;
  // if caller wishes, ensure that table exists
  if (!dir->tables[table_idx]) {
    // allocate table if it does not exist
    allocate_page_table(table_idx, dir);
  }
  return &dir->tables[table_idx]->pages[idx];
}

void write_page(struct page *page, bool is_kernel, bool is_writeable) {
  uintptr_t frame = (uintptr_t)allocate_page();
  if (page->present) {
    return; // Frame was already allocated, return straight away.
  } else {
    page->present = 1;                 // Mark it as present.
    page->rw = (is_writeable) ? 1 : 0; // Should the page be writeable?
    page->user = (is_kernel) ? 0 : 1;  // Should the page be user-mode?
    page->frame = frame >> 12;
  }
}

struct page_directory *get_current_directory() {
  return current_directory;
}

static void set_current_directory(struct page_directory *dir) {
  current_directory = dir;
}

void init_paging() {
  kernel_directory = kmalloc_a(sizeof(struct page_directory), PAGE_SIZE);
  memset(kernel_directory, 0, (sizeof(struct page_directory)));
  boot_table = kmalloc_a(sizeof(struct page_table), PAGE_SIZE);
  memset(boot_table, 0, (sizeof(struct page_table)));
  kernel_directory->tables[0] = boot_table;
  kernel_directory->tables_physical[0] = page_table_ptr_to_pde(boot_table);

  for (size_t i = 0; i < 1024; i++) {
    uintptr_t identity_ptr = i * 4096;
    struct page *pg = get_page(identity_ptr, kernel_directory);
    write_page(pg, true, true);
  }

  // register our page fault handler.
  register_interrupt_handler(14, page_fault);

  // Now, enable paging!
  switch_page_directory(kernel_directory);
}

void switch_page_directory(struct page_directory *dir) {
  set_current_directory(dir);
  asm volatile("mov %0, %%cr3" ::"r"(&dir->tables_physical));

  uint32_t cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

void page_fault(struct registers regs) {
  // A page fault has occurred.
  // The faulting address is stored in the CR2 register.
  uintptr_t faulting_address;
  asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

  // The error code gives us details of what happened.
  // Page not present
  int present = !(regs.err_code & 0x1);
  // Write operation?
  int rw = regs.err_code & 0x2;
  // Processor was in user-mode?
  int us = regs.err_code & 0x4;
  // Overwritten CPU-reserved bits of page entry?
  int reserved = regs.err_code & 0x8;
  // Caused by an instruction fetch?
  // int id = regs.err_code & 0x10;

  // Output an error message.
  screen_printf("Page fault! ( ");
  if (present) {
    screen_printf("present ");
  }
  if (rw) {
    screen_printf("read-only ");
  }
  if (us) {
    screen_printf("user-mode ");
  }
  if (reserved) {
    screen_printf("reserved ");
  }
  screen_printf(") at 0x%08x\n", faulting_address);
  PANIC("Page fault");
}
