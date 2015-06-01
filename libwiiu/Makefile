CC=powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -c
override CURDIR:=$(shell cygpath -m $(CURDIR))
root	:= $(CURDIR)
project := $(root)/src
build	:=	$(root)/bin
all: setup libwiiu500 libwiiu410 libwiiu400

setup:
	mkdir -p $(root)/bin/500/
	mkdir -p $(root)/bin/410/
	mkdir -p $(root)/bin/400/
libwiiu500:
	$(CC) $(CFLAGS) -DVER=500  $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/500
	rm $(root)/*.o
libwiiu410:
	$(CC) $(CFLAGS) -DVER=410 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/410
	rm $(root)/*.o
libwiiu400:
	$(CC) $(CFLAGS) -DVER=400 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/400
	rm $(root)/*.o
clean:
	rm -r $(build)/500/* $(build)/410/* $(build)/400/*
