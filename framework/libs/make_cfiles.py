import symbols

osv11_path = "C:/Users/George/Downloads/OSv11-500"
libnames = [\
    "coreinit.rpl",\
    "gx2.rpl",\
    "vpad.rpl"]

for lib in libnames:
    syms = symbols.get_symbols(osv11_path + '/' + lib)

    fout = open(lib.replace(".rpl",".c"),'w')
    for sym in syms:
        fout.write("void " + sym.decode("utf-8") + "(){}\n\n")
    fout.close()
