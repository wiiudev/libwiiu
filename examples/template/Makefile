project	:=	src
root:=$(CURDIR)
build	:=	 $(root)/bin
libs := $(root)/../../libwiiu/bin
www :=$(root)/../../www
framework:=$(root)/../../framework
all: main500 main410 main400

main500:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=500 -c $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	powerpc-eabi-ld -Ttext 1800000 --oformat binary -o $(build)/code500.bin $(libs)/500/loader.o $(build)/*.o `find $(libs)/500/ -name "*.o" ! -name "loader.o"`
main410:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=410 -c $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	powerpc-eabi-ld -Ttext 1800000 --oformat binary -o $(build)/code410.bin $(libs)/410/loader.o $(build)/*.o `find $(libs)/410/ -name "*.o" ! -name "loader.o"`
main400:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=400 -c $(project)/*.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	powerpc-eabi-ld -Ttext 1800000 --oformat binary -o $(build)/code400.bin $(libs)/400/loader.o $(build)/*.o `find $(libs)/400/ -name "*.o" ! -name "loader.o"`
clean:
	rm -r $(build)/*