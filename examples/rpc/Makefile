CC=powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -c
LD=powerpc-eabi-ld
LDFLAGS=-Ttext 1800000 --oformat binary
project	:=	src
root:=$(CURDIR)
build	:=	 $(root)/bin
libs := $(root)/../../libwiiu/bin
www :=$(root)/../../www
framework:=$(root)/../../framework
all: setup main500 main410 main400
setup:
	mkdir -p $(root)/bin/
main500:
	$(CC) $(CFLAGS) -DVER=500 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code500.bin $(build)/loader.o $(libs)/500/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main410:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=410 -c $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code410.bin $(build)/loader.o $(libs)/410/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main400:
	$(CC) $(CFLAGS) -DVER=400 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(CC) $(CFLAGS) -o $(build)/code400.bin $(build)/loader.o $(libs)/400/*.o `find $(build) -name "*.o" ! -name "loader.o"`
clean:
	rm -r $(build)/*
