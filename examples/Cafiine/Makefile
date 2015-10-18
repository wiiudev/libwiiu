export PATH	:=	$(DEVKITPPC)/bin:$(PATH)
CC=powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -c
LD=powerpc-eabi-ld
LDFLAGS=-Ttext 1800000 --oformat binary
project	:=	src
root:=.
build	:=	 $(root)/bin
libs := $(root)/../../libwiiu/bin
www :=$(root)/../../www
framework:=$(root)/../../framework
.PHONY: client
all: setup client main532 main310
setup:
	mkdir -p $(root)/bin/
client:
	cd client && $(MAKE)
main532:
	$(CC) $(CFLAGS) -DVER=532 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code532.bin $(build)/loader.o `find $(build) -name "*.o" ! -name "loader.o"`
main310:
	$(CC) $(CFLAGS) -DVER=310 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code310.bin $(build)/loader.o `find $(build) -name "*.o" ! -name "loader.o"`
clean:
	rm -r $(build)/*
