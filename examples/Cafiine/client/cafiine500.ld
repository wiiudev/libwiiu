OUTPUT(cafiine500.elf);

SECTIONS {
	.text 0x011dcc00 : {
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
PROVIDE(FSAInit = 0x105ef88);
PROVIDE(FSAShutdown = 0x105f050);
PROVIDE(FSAAddClient = 0x10637b0);
PROVIDE(FSADelClient = 0x105f180);
PROVIDE(FSAOpenFile = 0x10608d4);

/* FS methods */
PROVIDE(FSInit = 0x10666fc);
PROVIDE(FSShutdown = 0x10668b0);
PROVIDE(FSAddClientEx = 0x1066974);
PROVIDE(FSDelClient = 0x1066d8c);
PROVIDE(FSOpenFile = 0x106d4d8);
PROVIDE(FSCloseFile = 0x106d550);
PROVIDE(FSReadFile = 0x106d5d0);
PROVIDE(FSReadFileWithPos = 0x106d65c);
PROVIDE(FSGetPosFile = 0x106d988);
PROVIDE(FSSetPosFile = 0x106d9f8);
PROVIDE(FSGetStatFile = 0x106da68);
PROVIDE(FSIsEof = 0x106dad8);

/* GX2 methods */
PROVIDE(GX2WaitForVsync = 0x1150868);

/* Socket methods */
PROVIDE(socket_lib_init = 0x10bf2f4);
PROVIDE(socket = 0x10c11c8);
PROVIDE(socketclose = 0x10c1314);
PROVIDE(connect = 0x10bf828);
PROVIDE(send = 0x10c06ac);
PROVIDE(recv = 0x10bfaec);

/* Standard library methods */
PROVIDE(memcpy = 0x1035460);
PROVIDE(memset = 0x103544c);
PROVIDE(MEMAllocFromDefaultHeapEx = 0x1004e2d0);

/* OS data */
PROVIDE(title_id = 0x10013010);
