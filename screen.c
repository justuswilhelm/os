#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cpu.h"
#include "screen.h"

#define CONVERSION_BUFFER_SIZE 32

// Location of VGA buffer
static volatile uint16_t *vga_buffer = (uint16_t *)VGA_ADDR;

// Location of cursor
static size_t screen_x = 0;
static size_t screen_y = 0;

// http://www.jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html
static void set_cursor(size_t x, size_t y) {
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

static void scroll() {
  screen_x = 0;
  if (screen_y >= SCREEN_HEIGHT) {
    for (size_t x = 0; x < VGA_BUFFER_SIZE; x++) {
      vga_buffer[x] = vga_buffer[x + SCREEN_WIDTH];
    }
    for (size_t x = VGA_BUFFER_SIZE; x < VGA_BUFFER_SIZE + SCREEN_HEIGHT; x++) {
      vga_buffer[x] = SCREEN_BLANK;
    }
    screen_y--;
  } else {
    screen_y++;
  }
}

static bool isdigit(const char c) { return '0' <= c && c <= '9'; }

static void screen_put(const char c) {
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

void screen_puts(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    char c = str[i];
    screen_put(c);
  }
  set_cursor(screen_x, screen_y);
}

void screen_clear() {
  for (size_t i = 0; i < VGA_BUFFER_SIZE; i++) {
    vga_buffer[i] = SCREEN_BLANK;
  }
  screen_x = 0;
  screen_y = 0;
  set_cursor(screen_x, screen_y);
}

struct print_ctx {
  bool has_minimum_width;
  int minimum_width;
  enum { ZERO, SPACE } padding;
};

static void print_padding(void put(const char), size_t length,
                          struct print_ctx *ctx) {
  if (ctx->has_minimum_width > 0) {
    for (size_t padded = 0; padded < ctx->minimum_width - length - 1;
         padded++) {
      switch (ctx->padding) {
      case ZERO:
        put('0');
        break;
      case SPACE:
        put(' ');
        break;
      default:
        put('X');
        return;
      }
    }
  }
}

static void print_hex(void put(const char), int number, struct print_ctx *ctx) {
  char buffer[16] = {0};
  size_t buffer_pos = 0;
  if (number == 0) {
    buffer[0] = 'A';
    buffer_pos++;
  } else {
    for (; number > 0; buffer_pos++) {
      char digit = number % 16;
      number /= 16;
      if (digit >= 0 && digit < 10) {
        digit = '0' + digit;
      } else {
        digit = 'A' + digit - 10;
      }
      buffer[buffer_pos] = digit;
    }
  }

  buffer_pos--;

  print_padding(put, buffer_pos, ctx);

  for (size_t i = 0; i <= buffer_pos; i++) {
    put(buffer[buffer_pos - i]);
  }
}

static void print_number(void put(const char), int number,
                         struct print_ctx *ctx) {
  char buffer[CONVERSION_BUFFER_SIZE] = {0};
  size_t buffer_pos = 0;
  if (number == 0) {
    buffer_pos++;
    buffer[0] = '0';
  } else {
    for (; number > 0; buffer_pos++) {
      buffer[buffer_pos] = (number % 10) + '0';
      number = number / 10;
    }
  }

  buffer_pos--;

  print_padding(put, buffer_pos, ctx);

  for (size_t printed = 0; printed <= buffer_pos; printed++) {
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
  struct print_ctx ctx = {
      .minimum_width = -1,
      .padding = SPACE,
  };
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
        ctx.minimum_width = (ctx.minimum_width * 10) + d;
      } else {
        i--;
        state = FORMAT_END;
      }
    } else if (state == FORMAT_START) {
      if (isdigit(c)) {
        char d = c - '0';
        if (d == 0) {
          ctx.padding = ZERO;
          // puts("not implemented");
          // return;
        } else {
          state = MINIMUM_WIDTH;
          ctx.has_minimum_width = true;
          ctx.minimum_width = d;
        }
      } else {
        i--;
        state = FORMAT_END;
      }
    } else if (state == FORMAT_END) {
      if (c == 'x') {
        int number = va_arg(args, int);
        print_hex(put, number, &ctx);
        state = NO_FORMAT;
      } else if (c == 'd') {
        int number = va_arg(args, int);
        print_number(put, number, &ctx);
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
    } else {
      puts("Something went wrong");
      return;
    }
  }
}

void screen_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf(screen_puts, screen_put, fmt, args);
  va_end(args);
}
