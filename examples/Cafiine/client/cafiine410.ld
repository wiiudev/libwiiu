OUTPUT(cafiine410.elf);

SECTIONS {
	.text 0x011da800 : {
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
PROVIDE(FSAInit = 0x1059b1c);
PROVIDE(FSAShutdown = 0x1059be4);
PROVIDE(FSAAddClient = 0x105e340);
PROVIDE(FSADelClient = 0x1059d10);
PROVIDE(FSAOpenFile = 0x105b464);

/* FS methods */
PROVIDE(FSInit = 0x1060d70);
PROVIDE(FSShutdown = 0x1060ea0);
PROVIDE(FSAddClientEx = 0x1060f5c);
PROVIDE(FSDelClient = 0x106129c);
PROVIDE(FSOpenFile = 0x10668bc);
PROVIDE(FSCloseFile = 0x1066934);
PROVIDE(FSReadFile = 0x10669b4);
PROVIDE(FSReadFileWithPos = 0x1066a40);
PROVIDE(FSGetPosFile = 0x1066d6c);
PROVIDE(FSSetPosFile = 0x1066ddc);
PROVIDE(FSGetStatFile = 0x1066e4c);
PROVIDE(FSIsEof = 0x1066ebc);

/* GX2 methods */
PROVIDE(GX2WaitForVsync = 0x11454bc);

/* Socket methods */
PROVIDE(socket_lib_init = 0x10b44d4);
PROVIDE(socket = 0x10b5da0);
PROVIDE(socketclose = 0x10b5eec);
PROVIDE(connect = 0x10b49fc);
PROVIDE(send = 0x10b5528);
PROVIDE(recv = 0x10b4cc0);

/* Standard library methods */
PROVIDE(memcpy = 0x1030ee4);
PROVIDE(memset = 0x1030ed0);
PROVIDE(MEMAllocFromDefaultHeapEx = 0x10049edc);

/* OS data */
PROVIDE(title_id = 0x1000ecb0);
