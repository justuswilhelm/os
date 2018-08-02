#pragma once
#include <stdint.h>

void outb(uint32_t address, uint8_t value);
void outw(uint32_t address, uint16_t value);
void outl(uint32_t address, uint32_t value);

uint8_t inb(uint32_t address);
uint16_t inw(uint32_t address);
uint32_t inl(uint32_t address);
