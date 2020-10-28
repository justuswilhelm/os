CFLAGS = -std=gnu99 -ffreestanding -g -Wall -Wextra -Werror
CC = toolchain/bin/i386-elf-gcc
AS = toolchain/bin/i386-elf-as
GDB = toolchain/bin/i386-elf-gdb

SRC_PATH = src

AS_SRCS = \
	boot.s \
	gdt_s.s \
	idt_s.s \
	irq_s.s \

C_SRCS = \
	cpu.c \
	gdt.c \
	idt.c \
	irq.c \
	isr.c \
	keyboard.c \
	main.c \
	screen.c \
	timer.c \

OBJS = \
	$(patsubst %.c,%.o,$(C_SRCS)) \
	$(patsubst %.s,%.o,$(AS_SRCS)) \

.PHONY: toolchain depend

all: os.bin

qemu: os.bin
	qemu-system-i386 -kernel $<

qemu-debug: os.bin
	qemu-system-i386 -kernel $< -s -S -d int -no-reboot -no-shutdown

debug: os.bin
	$(GDB) -d int

depend: .depend

.depend: $(C_SRCS)
	rm -f $@
	$(CC) $(CFLAGS) -MM $^ -MF $@

include .depend

os.bin: linker.ld $(OBJS)
	$(CC) $(CC_FLAG) -T $< $(OBJS) -o $@ -nostdlib -lgcc

toolchain:
	make -C toolchain all

clean:
	rm -f $(OBJS)

format:
	clang-format -i *.c *.h
