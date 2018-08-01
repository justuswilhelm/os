#include <stdarg.h>
#include <stdbool.h>
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

static bool isdigit(const char c) { return '0' <= c && c <= '9'; }

void screen_puts(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    char c = str[i];
    screen_put(c);
  }
  screen_set_cursor(screen_x, screen_y);
}

void screen_put(const char c) {
  const size_t index = screen_x + screen_y * SCREEN_WIDTH;
  switch (c) {
  case '\n':
    scroll();
    break;
  default:
    vga_buffer[index] = ((uint16_t)0x0F << 8) | c;
    screen_x++;
    if (screen_x >= SCREEN_WIDTH) {
      scroll();
    }
    break;
  }
}

void screen_clear() {
  for (size_t i = 0; i < VGA_BUFFER_SIZE; i++) {
    vga_buffer[i] = SCREEN_BLANK;
  }
  screen_x = 0;
  screen_y = 0;
  screen_set_cursor(screen_x, screen_y);
}

static void print_hex(void put(const char), int number) {
  char buffer[16] = {0};
  size_t buffer_pos;
  for (buffer_pos = 0; number > 0; buffer_pos++) {
    char digit = number % 16;
    number /= 16;
    if (digit >= 0 && digit < 10) {
      digit = '0' + digit;
    } else {
      digit = 'A' + digit - 10;
    }
    buffer[buffer_pos] = digit;
  }

  buffer_pos--;
  for (size_t i = 0; i <= buffer_pos; i++) {
    put(buffer[buffer_pos - i]);
  }
}

static void print_number(void put(const char), int number, int minimum_width) {
  char buffer[16] = {0};
  size_t buffer_pos;
  for (buffer_pos = 0; number > 0; buffer_pos++) {
    buffer[buffer_pos] = (number % 10) + '0';
    number = number / 10;
  }

  buffer_pos--;

  for (size_t padded = 0; padded < minimum_width - buffer_pos - 1; padded++) {
    put('0');
  }
  size_t printed = 0;
  for (printed = 0; printed <= buffer_pos; printed++) {
    put(buffer[buffer_pos - printed]);
  }
}

static void printf(void puts(const char *), void put(const char),
                   const char *fmt, va_list args) {
  enum state {
    NO_FORMAT,
    MINIMUM_WIDTH,
    FORMAT_START,
    FORMAT_END,
  } state = NO_FORMAT;
  int16_t minimum_width = -1;
  for (size_t i = 0; fmt[i] != '\0'; i++) {
    char c = fmt[i];
    if (state == NO_FORMAT) {
      if (c == '%') {
        state = FORMAT_START;
      } else {
        put(c);
      }
    } else if (state == MINIMUM_WIDTH) {
      if (isdigit(c)) {
        char d = c - '0';
        minimum_width = (minimum_width * 10) + d;
      } else {
        i--;
        state = FORMAT_END;
      }
    } else if (state == FORMAT_START) {
      if (isdigit(c)) {
        char d = c - '0';
        if (d == 0) {
          puts("not implemented");
          return;
        } else {
          state = MINIMUM_WIDTH;
          minimum_width = d;
        }
      } else {
        i--;
        state = FORMAT_END;
      }
    } else if (state == FORMAT_END) {
      if (c == 'x') {
        int number = va_arg(args, int);
        print_hex(put, number);
        state = NO_FORMAT;
      } else if (c == 'd') {
        int number = va_arg(args, int);
        print_number(put, number, minimum_width);
        state = NO_FORMAT;
      } else if (c == 's') {
        char *string = va_arg(args, char *);
        puts(string);
        state = NO_FORMAT;
      } else {
        puts("Expected format specification");
        state = NO_FORMAT;
        continue;
      }
    }
  }
}

void screen_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf(screen_puts, screen_put, fmt, args);
  va_end(args);
}
