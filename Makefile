CC_FLAG = -std=gnu99 -ffreestanding -g
CC = toolchain/bin/i386-elf-gcc
AS = toolchain/bin/i386-elf-as

SRC_PATH = src

OBJS = \
	boot.o \
	kernel.o \

.PHONY: toolchain

all: os.bin

boot.o: boot.s
	$(CC) $(CC_FLAG) -c $< -o $@

%.o: %.c
	$(CC) $(CC_FLAG) -c $< -o $@

os.bin: linker.ld $(OBJS)
	$(CC) $(CC_FLAG) -T $< $(OBJS) -o $@ -nostdlib -lgcc

toolchain:
	make -C toolchain all

clean:
	rm -f $(OBJS)
