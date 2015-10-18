OUTPUT(cafiine532.elf);

SECTIONS {
	.text 0x011de000 : {
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
PROVIDE(FSAInit = 0x10608ac);
PROVIDE(FSAShutdown = 0x1060974);
PROVIDE(FSAAddClient = 0x106546c);
PROVIDE(FSADelClient = 0x1060aa4);
PROVIDE(FSAOpenFile = 0x10621f8);

/* FS methods */
PROVIDE(FSInit = 0x10683c8);
PROVIDE(FSShutdown = 0x1068538);
PROVIDE(FSAddClientEx = 0x10685fc);
PROVIDE(FSDelClient = 0x1068a08);
PROVIDE(FSOpenFile = 0x106ef7c);
PROVIDE(FSCloseFile = 0x106f088);
PROVIDE(FSReadFile = 0x106f108);
PROVIDE(FSReadFileWithPos = 0x106f194);
PROVIDE(FSGetPosFile = 0x106f4c0);
PROVIDE(FSSetPosFile = 0x106f530);
PROVIDE(FSGetStatFile = 0x106f5a0);
PROVIDE(FSIsEof = 0x106f610);

/* GX2 methods */
PROVIDE(GX2WaitForVsync = 0x1151964);

/* Socket methods */
PROVIDE(socket_lib_init = 0x10c02f4);
PROVIDE(socket = 0x10c21c8);
PROVIDE(socketclose = 0x10c2314);
PROVIDE(connect = 0x10c0828);
PROVIDE(send = 0x10c16ac);
PROVIDE(recv = 0x10c0aec);

/* Standard library methods */
PROVIDE(memcpy = 0x1035a68);
PROVIDE(memset = 0x1035a54);
PROVIDE(MEMAllocFromDefaultHeapEx = 0x1004e9c0);

/* OS data */
PROVIDE(title_id = 0x100136D0);