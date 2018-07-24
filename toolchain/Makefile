export PREFIX=$(CURDIR)
export TARGET=i386-elf
export CC=$(shell which gcc-8)
export CXX=$(shell which g++-8)
export LD=$(shell which ld)
export CFLAGS=-Wno-error=deprecated-declarations

BINUTILS_VER = binutils-2.24

.PHONY: all

all: binutils
	echo done

binutils: $(BINUTILS_VER)/Makefile $(BINUTILS_VER)
	make -C $(BINUTILS_VER)

$(BINUTILS_VER)/Makefile:
	cd $(BINUTILS_VER); ./configure --prefix=$(PREFIX) --target=$(TARGET) --disable-nls


$(BINUTILS_VER).tar.gz:
	curl -O http://ftp.gnu.org/gnu/binutils/$@

$(BINUTILS_VER): $(BINUTILS_VER).tar.gz
	tar xvzf $<

