root	:= $(CURDIR)
project := $(CURDIR)/src
build	:=	 $(root)/bin
all: libwiiu500 libwiiu410 libwiiu400

libwiiu500:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=500 -c $(project)/*.c 
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/500
	rm $(root)/*.o
libwiiu410:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=500 -c $(project)/*.c 
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/410
	rm $(root)/*.o
libwiiu400:
	powerpc-eabi-gcc -nostdinc -fno-builtin -DVER=500 -c $(project)/*.c 
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)/400
	rm $(root)/*.o
clean:
	rm -r $(build)/500/* $(build)/410/* $(build)/400/*