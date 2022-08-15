// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void *kmalloc(size_t);
void *kmalloc_a(size_t, uintptr_t);
void set_heap_start(uintptr_t);
