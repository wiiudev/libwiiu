root:=$(CURDIR)
project	:=	$(root)/src
build	:=	 $(root)/bin
all: findcode500 findcode410 findcode400
findcode500:
ifeq ("$(wildcard $(build)/findcode500.bin)","")
	# Build findcode
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=500+ -c $(project)/findcode500.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode500.o $(build)
	rm $(root)/findcode500.o
	powerpc-eabi-ld -Ttext 1800000 --oformat binary -o $(build)/findcode500.bin $(build)/findcode500.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode500.bin of=$(build)/findcode500.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode500.bin of=$(build)/findcode500.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode500.bin of=$(build)/findcode500.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc

	dd if=$(build)/findcode500.bin of=$(build)/stack500.bin obs=432 seek=1 conv=notrunc
endif
findcode410:
ifeq ("$(wildcard $(build)/findcode410.bin)","")
	# Build findcode
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=410+ -c $(project)/findcode410.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode410.o $(build)
	rm $(root)/findcode410.o
	powerpc-eabi-ld -Ttext 1800000 --oformat binary -o $(build)/findcode410.bin $(build)/findcode410.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode410.bin of=$(build)/findcode410.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode410.bin of=$(build)/findcode410.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode410.bin of=$(build)/findcode410.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	dd if=$(build)/findcode410.bin of=$(build)/stack410.bin obs=432 seek=1 conv=notrunc
endif
findcode400:
ifeq ("$(wildcard $(build)/findcode400.bin)","")
	# Build findcode
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=400+ -c $(project)/findcode400.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode400.o $(build)
	rm $(root)/findcode400.o
	powerpc-eabi-ld -Ttext 1800000 --oformat binary -o $(build)/findcode400.bin $(build)/findcode400.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode400.bin of=$(build)/findcode400.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode400.bin of=$(build)/findcode400.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode400.bin of=$(build)/findcode400.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	#Differen't on 400
	dd if=$(build)/findcode400.bin of=$(build)/stack400.bin obs=352 seek=1 conv=notrunc
endif
clean:
	rm -r $(build)/*