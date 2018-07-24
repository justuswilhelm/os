export PREFIX=$(CURDIR)
export TARGET=i386-elf
export CC=$(shell which gcc-8)
export CXX=$(shell which g++-8)
export LD=$(shell which ld)

BINUTILS_VER = 2.31.1
BINUTILS_PATH = binutils-$(BINUTILS_VER)
BINUTILS_AR = $(BINUTILS_PATH).tar.gz
BINUTILS_DL = http://ftp.gnu.org/gnu/binutils/$(BINUTILS_AR)

.PHONY: all

all: binutils
	echo done

binutils: $(BINUTILS_PATH)/Makefile $(BINUTILS_PATH)
	make -C $(BINUTILS_PATH)

$(BINUTILS_PATH)/Makefile: $(BINUTILS_PATH)/configure
	cd $(BINUTILS_PATH); \
		./configure \
			--prefix=$(PREFIX) \
			--target=$(TARGET) \
			--disable-nls

$(BINUTILS_AR):
	curl -O $@

$(BINUTILS_PATH)/configure: $(BINUTILS_PATH).tar.gz
	tar xvzf $<
	touch $@
