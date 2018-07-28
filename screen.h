// http://www.jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html
#pragma once

#include <stddef.h>
#include <stdint.h>

#define VGA_ADDR 0xB8000
#define SCREEN_HEIGHT 24
#define SCREEN_WIDTH 80
#define VGA_BUFFER_SIZE (SCREEN_HEIGHT * SCREEN_WIDTH)

// Write a single character out to the screen.
void screen_put(const uint8_t c);

// Clear the screen to all black.
void screen_clear();

// Output a null-terminated ASCII string to the monitor.
void screen_puts(const char *c);