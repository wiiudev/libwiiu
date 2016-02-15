PATH		:=	$(DEVKITPPC)/bin:$(PATH)
PREFIX		=	powerpc-eabi-
CC			=	$(PREFIX)gcc
CFLAGS		=	-nostdinc -fno-builtin -c
root		:=	.
project 	:=	$(root)/src
build		:=	$(root)/bin

all: setup libwiiu550 libwiiu532 libwiiu500 libwiiu410 libwiiu400 libwiiu310 libwiiu300 libwiiu210 libwiiu200

setup:
	mkdir -p $(root)/bin/550/
	mkdir -p $(root)/bin/532/
	mkdir -p $(root)/bin/500/
	mkdir -p $(root)/bin/410/
	mkdir -p $(root)/bin/400/
	mkdir -p $(root)/bin/310/
	mkdir -p $(root)/bin/300/
	mkdir -p $(root)/bin/210/
	mkdir -p $(root)/bin/200/

libwiiu550:
	$(CC) $(CFLAGS) -DVER=550  $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/550
	rm $(root)/*.o

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

libwiiu310:
	$(CC) $(CFLAGS) -DVER=310 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/310
	rm $(root)/*.o

libwiiu300:
	$(CC) $(CFLAGS) -DVER=300 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/300
	rm $(root)/*.o

libwiiu210:
	$(CC) $(CFLAGS) -DVER=210 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/210
	rm $(root)/*.o

libwiiu200:
	$(CC) $(CFLAGS) -DVER=200 $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/200
	rm $(root)/*.o

clean:
	rm -r $(build)/550/* $(build)/532/* $(build)/500/* $(build)/410/* $(build)/400/* $(build)/310/* $(build)/300/* $(build)/210/* $(build)/200/*
