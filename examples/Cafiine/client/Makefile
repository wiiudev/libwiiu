
PATH := $(DEVKITPPC)/bin:$(PATH)

PREFIX ?= powerpc-eabi-
LD := $(PREFIX)ld
AS := $(PREFIX)as
CC := $(PREFIX)gcc
OBJDUMP ?= $(PREFIX)objdump
OBJCOPY ?= $(PREFIX)objcopy

SFLAGS := -mgekko -mregnames

# -O2: optimise lots
# -Wall: generate lots of warnings
# -x c: compile as C code
# -std=gnu99: use the C99 standard with GNU extensions
# -ffreestanding: we don't have libc; don't expect we do
# -mrvl: enable wii/gamecube compilation
# -mcpu=750: enable processor specific compilation
# -meabi: enable eabi specific compilation
# -mhard-float: enable hardware floating point instructions
# -fshort-wchar: use 16 bit whcar_t type in keeping with Wii executables
# -msdata-none: do not use r2 or r13 as small data areas
# -memb: enable embedded application specific compilation
# -ffunction-sections: split up functions so linker can garbage collect
# -fdata-sections: split up data so linker can garbage collect
CFLAGS := -O2 -Wall -x c -std=gnu99 \
          -ffreestanding \
          -mrvl -mcpu=750 -meabi -mhard-float -fshort-wchar \
          -msdata=none -memb -ffunction-sections -fdata-sections \
          -Wno-unknown-pragmas -Wno-strict-aliasing \

SRC := $(wildcard *.S) $(wildcard *.c)
OBJ := $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SRC)))

all: ../src/cafiine310.h ../src/cafiine410.h ../src/cafiine532.h

../src/cafiine%.h: cafiine%.text.bin cafiine%.magic.bin
	xxd -i cafiine$*.magic.bin | sed "s/unsigned/static const unsigned/g;s/ine$*/ine/g" > $@
	xxd -i cafiine$*.text.bin | sed "s/unsigned/static const unsigned/g;s/ine$*/ine/g" >> $@

cafiine%.text.bin: cafiine%.elf
	$(OBJCOPY) -j .text -O binary $< $@
cafiine%.magic.bin: cafiine%.elf
	$(OBJCOPY) -j .magic -O binary $< $@

cafiine%.elf: cafiine%.ld $(OBJ)
	$(LD) -T $< $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $+
%.o: %.S
	$(AS) $(SFLAGS) -o $@ $+

clean:
	rm -f $(wildcard *.o) $(wildcard *.elf) $(wildcard *.bin) $(wildcard ../src/cafiine310.h ../src/cafiine410.h \
		../src/cafiine500.h ../src/cafiine532.h)

