CC_FLAG = -std=gnu99 -ffreestanding -g -Wall -Wextra
CC = toolchain/bin/i386-elf-gcc
AS = toolchain/bin/i386-elf-as

SRC_PATH = src

SRCS = \
	main.c \

OBJS = \
	boot.o \
	$(patsubst %.c,%.o,$(SRCS)) \

.PHONY: toolchain depend

all: os.bin

qemu: os.bin
	qemu-system-i386 -kernel $<

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend

include .depend

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

format:
	clang-format -i *.c *.h
