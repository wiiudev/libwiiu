CC=$(DEVKITPPC)/bin/powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -c
LD=$(DEVKITPPC)/bin/powerpc-eabi-ld
LDFLAGS=-Ttext 1800000 --oformat binary

root:=.
project	:=	$(root)/src
build	:=	 $(root)/bin
all: setup findcode532 findcode500 findcode410 findcode400 findcode310 findcode300 findcode210 findcode200
setup:
	mkdir -p $(root)/bin
findcode532:
ifeq ("$(wildcard $(build)/findcode532.bin)","")
	# Build findcode
	$(CC) $(CFLAGS) -DVER=532+ $(project)/findcode532.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode532.o $(build)
	rm $(root)/findcode532.o
	$(LD) $(LDFLAGS) -o $(build)/findcode532.bin $(build)/findcode532.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode532.bin of=$(build)/findcode532.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode532.bin of=$(build)/findcode532.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode532.bin of=$(build)/findcode532.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc

	dd if=$(build)/findcode532.bin of=$(build)/stack532.bin obs=876 seek=1 conv=notrunc
endif
findcode500:
ifeq ("$(wildcard $(build)/findcode500.bin)","")
	# Build findcode
	$(CC) $(CFLAGS) -DVER=500+ $(project)/findcode500.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode500.o $(build)
	rm $(root)/findcode500.o
	$(LD) $(LDFLAGS) -o $(build)/findcode500.bin $(build)/findcode500.o
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
	$(CC) $(CFLAGS) -DVER=410+ $(project)/findcode410.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode410.o $(build)
	rm $(root)/findcode410.o
	$(LD) $(LDFLAGS) -o $(build)/findcode410.bin $(build)/findcode410.o
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
	$(CC) $(CFLAGS) -DVER=400+ $(project)/findcode400.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode400.o $(build)
	rm $(root)/findcode400.o
	$(LD) $(LDFLAGS) -o $(build)/findcode400.bin $(build)/findcode400.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode400.bin of=$(build)/findcode400.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode400.bin of=$(build)/findcode400.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode400.bin of=$(build)/findcode400.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	#Different on 400
	dd if=$(build)/findcode400.bin of=$(build)/stack400.bin obs=352 seek=1 conv=notrunc
endif
findcode310:
ifeq ("$(wildcard $(build)/findcode310.bin)","")
	# Build findcode
	$(CC) $(CFLAGS) -DVER=310+ $(project)/findcode310.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode310.o $(build)
	rm $(root)/findcode310.o
	$(LD) $(LDFLAGS) -o $(build)/findcode310.bin $(build)/findcode310.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode310.bin of=$(build)/findcode310.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode310.bin of=$(build)/findcode310.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode310.bin of=$(build)/findcode310.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	
	dd if=$(build)/findcode310.bin of=$(build)/stack310.bin obs=352 seek=1 conv=notrunc
endif
findcode300:
ifeq ("$(wildcard $(build)/findcode300.bin)","")
	# Build findcode
	$(CC) $(CFLAGS) -DVER=300+ $(project)/findcode300.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode300.o $(build)
	rm $(root)/findcode300.o
	$(LD) $(LDFLAGS) -o $(build)/findcode300.bin $(build)/findcode300.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode300.bin of=$(build)/findcode300.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode300.bin of=$(build)/findcode300.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode300.bin of=$(build)/findcode300.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	
	dd if=$(build)/findcode300.bin of=$(build)/stack300.bin obs=352 seek=1 conv=notrunc
endif
findcode210:
ifeq ("$(wildcard $(build)/findcode210.bin)","")
	# Build findcode
	$(CC) $(CFLAGS) -DVER=210+ $(project)/findcode210.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode210.o $(build)
	rm $(root)/findcode210.o
	$(LD) $(LDFLAGS) -o $(build)/findcode210.bin $(build)/findcode210.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode210.bin of=$(build)/findcode210.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode210.bin of=$(build)/findcode210.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode210.bin of=$(build)/findcode210.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	
	dd if=$(build)/findcode210.bin of=$(build)/stack210.bin obs=352 seek=1 conv=notrunc
endif
findcode200:
ifeq ("$(wildcard $(build)/findcode200.bin)","")
	# Build findcode
	$(CC) $(CFLAGS) -DVER=200+ $(project)/findcode200.c
	#-Wa,-a,-ad
	cp -r $(root)/findcode200.o $(build)
	rm $(root)/findcode200.o
	$(LD) $(LDFLAGS) -o $(build)/findcode200.bin $(build)/findcode200.o
	# Get rid of GCC function prologue and move stack out of our buffer
	# Result should start with: 94 21 E0 00 7C 3F 0B 78 3D 20 1D D7 61 29 B8 14
	dd if=$(build)/findcode200.bin of=$(build)/findcode200.bin bs=4 obs=4 skip=5 count=1 conv=notrunc
	dd if=$(build)/findcode200.bin of=$(build)/findcode200.bin bs=4 obs=4 skip=4 seek=1 count=1 conv=notrunc
	dd if=$(build)/findcode200.bin of=$(build)/findcode200.bin bs=4 obs=4 skip=6 seek=2 conv=notrunc
	
	dd if=$(build)/findcode200.bin of=$(build)/stack200.bin obs=352 seek=1 conv=notrunc
endif
clean:
	rm -r $(build)/*
