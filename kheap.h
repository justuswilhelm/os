// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void *kmalloc(size_t size);
void *kmalloc_a(size_t size);
void *kmalloc_ap(size_t size, uintptr_t *phys);
void *kmalloc_p(size_t size, uintptr_t *phys);
