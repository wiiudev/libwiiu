PATH		:=	$(DEVKITPPC)/bin:$(PATH)
PREFIX		=	powerpc-eabi-
CC			=	$(PREFIX)gcc
CFLAGS		=	-std=gnu99 -nostdinc -fno-builtin -fno-toplevel-reorder -c -Os
LD			=	$(PREFIX)ld
LDFLAGS		=	-Ttext 1800000 --oformat binary -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2 -lgcc
project		:=	src
root		:=	.
build		:=	$(root)/bin
libs		:=	$(root)/../../libwiiu/bin
www			:=	$(root)/../../www
framework	:=	$(root)/../../framework

all: setup main532 main500 main410 main400 main310 main300 main210 main200

setup:
	mkdir -p $(root)/bin/

main540:
	make main FIRMWARE=532

main532:
	make main FIRMWARE=532

main500:
	make main FIRMWARE=500

main410:
	make main FIRMWARE=410

main400:
	make main FIRMWARE=400

main310:
	make main FIRMWARE=310

main300:
	make main FIRMWARE=300

main210:
	make main FIRMWARE=210

main200:
	make main FIRMWARE=200

main:
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) $(project)/*.c
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) -o $(build)/code$(FIRMWARE).bin $(build)/loader.o $(libs)/$(FIRMWARE)/*.o `find $(build) -name "*.o" ! -name "loader.o"` $(LDFLAGS)

clean:
	rm -r $(build)/*
