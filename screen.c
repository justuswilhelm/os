#include <stddef.h>
#include <stdint.h>

#include "ports.h"
#include "screen.h"

// Location of VGA buffer
static volatile uint16_t *vga_buffer = (uint16_t *)VGA_ADDR;

// Location of cursor
static size_t screen_x = 0;
static size_t screen_y = 0;

// http://www.jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html
static void screen_set_cursor(size_t x, size_t y) {
  // The screen is 80 characters wide...
  size_t location = y * SCREEN_WIDTH + x;
  // Tell the VGA board we are setting the high cursor byte.
  outb(0x3D4, 14);
  // Send the high cursor byte.
  outb(0x3D5, location >> 8);
  // Tell the VGA board we are setting the low cursor byte.
  outb(0x3D4, 15);
  // Send the low cursor byte.
  outb(0x3D5, location);
}

void screen_put(const uint8_t c) {
  const size_t index = screen_x + screen_y * SCREEN_WIDTH;
  vga_buffer[index] = ((uint16_t)0x0F << 8) | c;
}

static void check_scrollback() {
  if ((screen_y) >= (SCREEN_HEIGHT)) {
    for (size_t x = 0; x < VGA_BUFFER_SIZE; x++) {
      vga_buffer[x] = vga_buffer[x + SCREEN_WIDTH];
    }
    for (size_t x = VGA_BUFFER_SIZE; x < VGA_BUFFER_SIZE + SCREEN_HEIGHT; x++) {
      vga_buffer[x] = SCREEN_BLANK;
    }
  }
}

static void scroll() {
  screen_x = 0;
  if (screen_y >= SCREEN_HEIGHT) {
    check_scrollback();
    screen_y--;
  } else {
    screen_y++;
  }
  screen_set_cursor(screen_x, screen_y);
}

void screen_puts(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    char c = str[i];
    switch (c) {
    case '\n':
      scroll();
      break;
    default:
      screen_put(c);
      screen_x++;
      if (screen_x >= SCREEN_WIDTH) {
        scroll();
      }
      break;
    }
  }
  screen_set_cursor(screen_x, screen_y);
}

void screen_clear() {
  for (size_t i = 0; i < VGA_BUFFER_SIZE; i++) {
    vga_buffer[i] = SCREEN_BLANK;
  }
  screen_x = 0;
  screen_y = 0;
  screen_set_cursor(screen_x, screen_y);
}
