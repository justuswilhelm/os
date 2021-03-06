export PREFIX=$(CURDIR)
export TARGET=i386-elf

TAR = tar
TAR_FLAG = xzf

BINUTILS_VER = 2.31.1
BINUTILS_PATH = binutils-$(BINUTILS_VER)
BINUTILS_BUILD_PATH = $(BINUTILS_PATH)-build
BINUTILS_AR = $(BINUTILS_PATH).tar.gz
BINUTILS_DL = https://ftp.gnu.org/gnu/binutils/$(BINUTILS_AR)

GCC_VER = 8.1.0
GCC_PATH = gcc-$(GCC_VER)
GCC_BUILD_PATH = $(GCC_PATH)-build
GCC_AR = $(GCC_PATH).tar.gz
GCC_DL = https://ftp.gnu.org/gnu/gcc/$(GCC_PATH)/$(GCC_AR)

GDB_VER = 10.1
GDB_PATH = gdb-$(GDB_VER)
GDB_BUILD_PATH = $(GDB_PATH)-build
GDB_AR = $(GDB_PATH).tar.gz
GDB_DL = https://ftp.gnu.org/gnu/gdb/$(GDB_AR)

.PHONY: all

all: binutils gcc
	echo done

binutils: $(BINUTILS_BUILD_PATH)/Makefile
	make -C $(BINUTILS_BUILD_PATH)
	make -C $(BINUTILS_BUILD_PATH) install

$(BINUTILS_BUILD_PATH)/Makefile: $(BINUTILS_PATH)/configure
	mkdir -p $(BINUTILS_BUILD_PATH)
	cd $(BINUTILS_BUILD_PATH); \
		../$(BINUTILS_PATH)/configure \
			--prefix=$(PREFIX) \
			--target=$(TARGET) \
			--disable-nls \
			--with-sysroot

$(BINUTILS_PATH)/configure: $(BINUTILS_AR)
	$(TAR) $(TAR_FLAG) $<
	touch $@

$(BINUTILS_AR):
	curl -O $(BINUTILS_DL)

gcc: $(GCC_BUILD_PATH)/Makefile
	make -C $(GCC_BUILD_PATH) all-gcc
	make -C $(GCC_BUILD_PATH) all-target-libgcc
	make -C $(GCC_BUILD_PATH) install-gcc
	make -C $(GCC_BUILD_PATH) install-target-libgcc

$(GCC_BUILD_PATH)/Makefile: $(GCC_PATH)/configure
	mkdir -p $(GCC_BUILD_PATH)
	cd $(GCC_BUILD_PATH); \
		../$(GCC_PATH)/configure \
			--prefix=$(PREFIX) \
			--target=$(TARGET) \
			--disable-nls \
			--enable-languages=c \
			--enable-multilib \
			--with-system-zlib \
			--enable-multilib \
			--without-headers

$(GCC_PATH)/configure: $(GCC_AR)
	$(TAR) $(TAR_FLAG) $<
	touch $@

$(GCC_AR):
	curl -O $(GCC_DL)

gdb: $(GDB_BUILD_PATH)/Makefile
	make -C $(GDB_BUILD_PATH) all
	make -C $(GDB_BUILD_PATH) install

$(GDB_BUILD_PATH)/Makefile: $(GDB_PATH)/configure
	mkdir -p $(GDB_BUILD_PATH)
	cd $(GDB_BUILD_PATH); \
		../$(GDB_PATH)/configure \
			--prefix=$(PREFIX) \
			--target=$(TARGET)

$(GDB_PATH)/configure: $(GDB_AR)
	$(TAR) $(TAR_FLAG) $<
	touch $@

$(GDB_AR):
	curl -O $(GDB_DL)

clean:
	rm -rf $(GCC_PATH) $(BINUTILS_PATH) $(GCC_BUILD_PATH) $(BINUTILS_BUILD_PATH)
