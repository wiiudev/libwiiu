import os

libnames = [\
    "coreinit.rpl",\
    "gx2.rpl",\
    "vpad.rpl"]

for lib in libnames:
    cfile = lib[:-3] + 'c'
    ofile = lib[:-3] + 'o'
    sofile = lib[:-3] + "so"
    os.system("powerpc-eabi-gcc -fPIC -ffreestanding -nostdinc -g -c " + cfile)
    os.system("powerpc-eabi-gcc -shared -o lib" + sofile + " " + ofile)
    
