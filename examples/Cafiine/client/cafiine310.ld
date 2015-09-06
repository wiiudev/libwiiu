OUTPUT(cafiine310.elf);

SECTIONS {
	.text 0x011d2000 : {
		server_ip = .;
		. = . + 4;
		*(.text._start);
		*(.text*);
		*(.magicptr*);
	}
	.magic : {
		*(.magic*);
	}
	/DISCARD/ : {
		*(*);
	}
}

/* FSA methods */
PROVIDE(FSAInit = 0x105105c);
PROVIDE(FSAShutdown = 0x1051124);
PROVIDE(FSAAddClient = 0x10554e4);
PROVIDE(FSADelClient = 0x1051250);
PROVIDE(FSAOpenFile = 0x10529a4);

/* FS methods */
PROVIDE(FSInit = 0x1057dd8);
PROVIDE(FSShutdown = 0x1057f00);
PROVIDE(FSAddClientEx = 0x1057fbc);
PROVIDE(FSDelClient = 0x10582fc);
PROVIDE(FSOpenFile = 0x105d5e0);
PROVIDE(FSCloseFile = 0x105d658);
PROVIDE(FSReadFile = 0x105d6d8);
PROVIDE(FSReadFileWithPos = 0x105d764);
PROVIDE(FSGetPosFile = 0x105da90);
PROVIDE(FSSetPosFile = 0x105db00);
PROVIDE(FSGetStatFile = 0x105db70);
PROVIDE(FSIsEof = 0x105dbe0);

/* GX2 methods */
PROVIDE(GX2WaitForVsync = 0x1142148);

/* Socket methods */
PROVIDE(socket_lib_init = 0x10a7ee0);
PROVIDE(socket = 0x10a93ec);
PROVIDE(socketclose = 0x10a9534);
PROVIDE(connect = 0x10a83ac);
PROVIDE(send = 0x10a8b98);
PROVIDE(recv = 0x10a8670);

/* Standard library methods */
PROVIDE(memcpy = 0x102b5c4);
PROVIDE(memset = 0x102b5b0);
PROVIDE(MEMAllocFromDefaultHeapEx = 0x100479AC);

/* OS data */
PROVIDE(title_id = 0x1000C870);
