// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "isr.h"

#define PAGE_SIZE 0x1000

struct page {
  // Page present in memory
  uint32_t present : 1;
  // Read-only if clear, readwrite if set
  uint32_t rw : 1;
  // Supervisor level only if clear
  uint32_t user : 1;
  // Has the page been accessed since last refresh?
  uint32_t accessed : 1;
  // Has the page been written to since last refresh?
  uint32_t dirty : 1;
  // Amalgamation of unused and reserved bits
  uint32_t unused : 7;
  // Frame address (shifted right 12 bits)
  uint32_t frame : 20;
};

struct page_table {
  struct page pages[1024];
};

struct page_directory {
  // Array of pointers to the pagetable above, but given their physical
  // location, for loading into the CR3 register.
  uint32_t tables_physical[1024];
  // Array of pointers to pagetable
  struct page_table *tables[1024];
  // The physical address of tablesPhysical. This comes into play when we get
  // our kernel heap allocated and the directory may be in a different location
  // in virtual memory.
  uintptr_t physical_address;
};

// Retrieves a pointer to the page required.
// If make == 1, if the page-table in which this page should reside isn't
// created, create it!
struct page *get_page(uintptr_t address, struct page_directory *dir);
void write_page(struct page *page, bool is_kernel, bool is_writeable);

struct page_directory *get_current_directory();

//  Sets up the environment, page directories etc and enables paging.
void init_paging();

// Causes the specified page directory to be loaded into the CR3 register.
void switch_page_directory(struct page_directory *);

// Handler for page faults.
void page_fault(struct registers regs);
