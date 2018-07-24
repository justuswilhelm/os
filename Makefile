CC_FLAG = -std=gnu99 -ffreestanding -g
CC = toolchain/bin/i386-elf-gcc
AS = toolchain/bin/i386-elf-as

SRC_PATH = src

.PHONY: toolchain

all: os.bin

boot.o: boot.s
	$(CC) $(CC_FLAG) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CC_FLAG) -c $< -o $@

os.bin: linker.ld boot.o kernel.o
	$(CC) $(CC_FLAG) -T $< boot.o kernel.o -o $@ -nostdlib -lgcc

toolchain:
	make -C toolchain all

clean:
	rm *.o
