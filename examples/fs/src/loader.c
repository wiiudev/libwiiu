#include "loader.h"

/* Start a thread for the FS funcs */
void _start()
{
    /* Get a handle to coreinit.rpl */
	uint32_t coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

	/* Memory allocation functions */
	void* (*OSAllocFromSystem)(uint32_t size, int align);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);

	/* OS thread functions */
	bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t *stack, uint32_t stack_size, int priority, uint16_t attr);
	int (*OSResumeThread)(void *thread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);

	/* Make a thread to run the FS functions */
	OSContext *thread = (OSContext*)OSAllocFromSystem(0x1000, 8);
	uint32_t *stack = (uint32_t*)OSAllocFromSystem(0x1000, 0x20);
	if (!OSCreateThread(thread, &fs_test, 0, NULL, stack + 0x400, 0x1000, 0, 0x2 | 0x8)) OSFatal("Failed to create thread");
	OSResumeThread(thread);

	/* Infinite loop */
	while(1);
}

/* FS testing */
void fs_test()
{
    /* Get a handle to coreinit.rpl */
	uint32_t coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

	/* Memory allocation functions */
	uint32_t *memalign, *freemem;
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMAllocFromDefaultHeapEx", &memalign);
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMFreeToDefaultHeap", &freemem);
	void* (*MEMAllocFromDefaultHeapEx)(uint32_t size, int align) = (void* (*)(uint32_t,int))*memalign;
	void (*MEMFreeToDefaultHeap)(void *ptr) = (void (*)(void*))*freemem;

	/* FS functions */
	void (*FSInit)();
	void (*FSShutdown)();
	int (*FSAddClient)(void *client, int unk1);
	void (*FSInitCmdBlock)(void *cmd);
	int (*FSOpenDir)(void *client, void *cmd, char *path, uint32_t *dir_handle, int unk1);
	int (*FSReadDir)(void *client, void *cmd, uint32_t dir_handle, void *buffer, int unk1);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSInit", &FSInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSShutdown", &FSShutdown);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSAddClient", &FSAddClient);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSInitCmdBlock", &FSInitCmdBlock);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSOpenDir", &FSOpenDir);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSReadDir", &FSReadDir);

	/* Shutdown and reinitialize the FS library 
	FSShutdown();
	FSInit();*/

	/* Initialize the FS library */
	FSInit();

	/* Set up the client and command blocks */
	void *client = MEMAllocFromDefaultHeapEx(0x1700, 0x20);
	void *cmd = MEMAllocFromDefaultHeapEx(0xA80, 0x20);
	if (!client || !cmd) OSFatal("Failed to allocate client and command block");
	//((void (*)())0x101cd70)();
	*((uint32_t*)0x10174F88) = 0;
	//((void (*)())0x101cd70)();
	int ret = FSAddClient(client, -1);
	//((void (*)())0x101cd70)();
	FSInitCmdBlock(cmd);

	/* Open /vol/save */
	uint32_t dir_handle;
	ret = FSOpenDir(client, cmd, "/vol/storage_mlc01/usr/save/00050030/10012100/user/", &dir_handle, -1);

	char buf[256];
	__os_snprintf(buf, 256, "FSOpenDir() returned %d, handle=0x%08X", ret, dir_handle);
	OSFatal(buf);

	//((void (*)())0x101cd70)();

	/* Start reading its directory entries */
	uint32_t *buffer = MEMAllocFromDefaultHeapEx(0x200, 0x20);
	while (1)
	{
		/* Read the directory entry */
		int ret = FSReadDir(client, cmd, dir_handle, buffer, -1);
		//((void (*)())0x101cd70)();
		if (ret != 0) break;

		/* Get the attributes, size, and name */
		uint32_t attr = buffer[0];
		uint32_t size = buffer[1];
		char *name = (char*)&buffer[25];

		//((void (*)())0x101cd70)();

		/* Print it out */
		char buf[256];
		__os_snprintf(buf, 256, "%s\t%d bytes\t%s\n", name, size, (attr & 0x80000000) ? "Directory" : "File");
		OSFatal(buf);
	}

	OSFatal("Failed to read any dirents");
}

void *memset(void *dst, int value, uint32_t len)
{
	for (int i = 0; i < len; i++) ((uint8_t*)dst)[i] = (uint8_t)value;
	return dst;
}
