CC=powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -c

root	:= .
project := $(root)/src
build	:=	$(root)/bin
all: setup libwiiu532 libwiiu500 libwiiu410 libwiiu400 libwiiu300

setup:
	mkdir -p $(root)/bin/532/
	mkdir -p $(root)/bin/500/
	mkdir -p $(root)/bin/410/
	mkdir -p $(root)/bin/400/
	mkdir -p $(root)/bin/300/
libwiiu532:
	$(CC) $(CFLAGS) -DVER=532  $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/532
	rm $(root)/*.o
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
libwiiu300:
	$(CC) $(CFLAGS) -DVER=300 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/300
	rm $(root)/*.o
clean:
	rm -r $(build)/532/* $(build)/500/* $(build)/410/* $(build)/400/* $(build)/300/*
