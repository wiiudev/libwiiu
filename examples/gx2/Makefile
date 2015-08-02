<<<<<<< HEAD
CC=$(DEVKITPPC)/bin/powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -c
LD=$(DEVKITPPC)/bin/powerpc-eabi-ld
LDFLAGS=-Ttext 1800000 --oformat binary
project	:=	src
root:=.
build	:=	 $(root)/bin
libs := $(root)/../../libwiiu/bin
www :=$(root)/../../www
framework:=$(root)/../../framework
all: setup main532 main500 main410 main400 main310 main300 main210 main200
setup:
	mkdir -p $(root)/bin/
main532:
	$(CC) $(CFLAGS) -DVER=532 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code532.bin $(build)/loader.o $(libs)/532/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main500:
	$(CC) $(CFLAGS) -DVER=500 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code500.bin $(build)/loader.o $(libs)/500/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main410:
	$(CC) $(CFLAGS) -DVER=410 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code410.bin $(build)/loader.o $(libs)/410/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main400:
	$(CC) $(CFLAGS) -DVER=400 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code400.bin $(build)/loader.o $(libs)/400/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main310:
	$(CC) $(CFLAGS) -DVER=310 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code310.bin $(build)/loader.o $(libs)/310/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main300:
	$(CC) $(CFLAGS) -DVER=300 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code300.bin $(build)/loader.o $(libs)/300/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main210:
	$(CC) $(CFLAGS) -DVER=210 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code210.bin $(build)/loader.o $(libs)/210/*.o `find $(build) -name "*.o" ! -name "loader.o"`
main200:
	$(CC) $(CFLAGS) -DVER=200 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code200.bin $(build)/loader.o $(libs)/200/*.o `find $(build) -name "*.o" ! -name "loader.o"`
clean:
	rm -r $(build)/*
