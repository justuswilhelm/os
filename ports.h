// http://www.jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html
#pragma once

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
