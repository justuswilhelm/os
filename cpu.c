// https://samypesse.gitbooks.io/how-to-create-an-operating-system/content/Chapter-5/
#include "cpu.h"

void outb(uint32_t address, uint8_t value) {
  asm volatile("outb %%al, %%dx" ::"d"(address), "a"(value));
  ;
}

void outw(uint32_t address, uint16_t value) {
  asm volatile("outw %%ax, %%dx" ::"d"(address), "a"(value));
}

void outl(uint32_t address, uint32_t value) {
  asm volatile("outl %%eax, %%dx" : : "d"(address), "a"(value));
}

uint8_t inb(uint32_t address) {
  uint8_t ret;
  asm volatile("inb %%dx, %%al" : "=a"(ret) : "d"(address));
  return ret;
}

uint16_t inw(uint32_t address) {
  uint16_t ret;
  asm volatile("inw %%dx, %%ax" : "=a"(ret) : "d"(address));
  return ret;
}

uint32_t inl(uint32_t address) {
  uint32_t ret;
  asm volatile("inl %%dx, %%eax" : "=a"(ret) : "d"(address));
  return ret;
}
