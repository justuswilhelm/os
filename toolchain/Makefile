export PREFIX=$(CURDIR)
export TARGET=i386-elf
export LD=$(shell which ld)

BINUTILS_VER = 2.31.1
BINUTILS_PATH = binutils-$(BINUTILS_VER)
BINUTILS_AR = $(BINUTILS_PATH).tar.gz
BINUTILS_DL = http://ftp.gnu.org/gnu/binutils/$(BINUTILS_AR)

GCC_VER = 8.1.0
GCC_PATH = gcc-$(GCC_VER)
GCC_AR = $(GCC_PATH).tar.gz
GCC_DL = http://ftp.gnu.org/gnu/gcc/$(GCC_PATH)/$(GCC_AR)

.PHONY: all

all: binutils gcc
	echo done

binutils: $(BINUTILS_PATH)/Makefile
	make -C $(BINUTILS_PATH)
	make -C $(BINUTILS_PATH) install

$(BINUTILS_PATH)/Makefile: $(BINUTILS_PATH)/configure
	cd $(BINUTILS_PATH); \
		./configure \
			--prefix=$(PREFIX) \
			--target=$(TARGET) \
			--disable-nls

$(BINUTILS_PATH)/configure: $(BINUTILS_AR)
	tar xvzf $<
	touch $@

$(BINUTILS_AR):
	curl -O $(BINUTILS_DL)

gcc: $(GCC_PATH)/Makefile
	make -C $(GCC_PATH) all-gcc
	make -C $(GCC_PATH) install-gcc

$(GCC_PATH)/Makefile: $(GCC_PATH)/configure
	cd $(GCC_PATH); \
		./configure \
			--prefix=$(PREFIX) \
			--target=$(TARGET) \
			--disable-nls \
			--enable-languages=c \
			--enable-multilib \
			--with-system-zlib \
			--without-headers

$(GCC_PATH)/configure: $(GCC_AR)
	tar xvzf $<
	touch $@

$(GCC_AR):
	curl -O $(GCC_DL)
