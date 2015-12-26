#include "loader.h"

/* Trap CPU0 and CPU2 at the ICI handler, and get the new kernel loaded */
void _start()
{
    /* Get a handle to coreinit.rpl */
	uint32_t coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

	/* Memory and heap functions */
	void (*DCFlushRange)(void *buffer, uint32_t length);
	void (*ICInvalidateRange)(void *buffer, uint32_t length);
	uint32_t (*OSEffectiveToPhysical)(void *vaddr);
	void* (*OSAllocFromSystem)(uint32_t size, int align);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "ICInvalidateRange", &ICInvalidateRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSEffectiveToPhysical", &OSEffectiveToPhysical);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);

	/* OS thread functions */
	bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t *stack, uint32_t stack_size, int priority, uint16_t attr);
	int (*OSResumeThread)(void *thread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);

	/* ICI functions */
	void (*__KernelSendICI)(int unk1, int cpunum, int code, int unk2);
	OSDynLoad_FindExport(coreinit_handle, 0, "__KernelSendICI", &__KernelSendICI);

	/* Make sure the kernel is mapped */
	if (OSEffectiveToPhysical((void*)0xA0000000) != 0xC0000000) OSFatal("Kernel must be mapped as RW");

	/* Patch the ICI handler to disable caches and loop */
	uint32_t ici_stub[] = 
	{
		0x38601040,		/* li      r3,4160 */
		0x7c600124, 	/* mtmsr   r3 */
		0x4c00012c, 	/* isync */
		0x7c79faa6, 	/* mfl2cr  r3 */
		0x3c807fff, 	/* lis     r4,32767 */
		0x6084ffff, 	/* ori     r4,r4,65535 */
		0x7c632038, 	/* and     r3,r3,r4 */
		0x7c79fba6, 	/* mtl2cr  r3 */
		0x7c0004ac, 	/* sync */
		0x7c70faa6, 	/* mfspr   r3,1008  */
		0x3c80ffff, 	/* lis     r4,-1 */
		0x60843fff, 	/* ori     r4,r4,16383 */
		0x7c632038, 	/* and     r3,r3,r4 */
		0x7c70fba6, 	/* mtspr   1008,r3 */
		0x7c0004ac, 	/* sync  */
		0x48000000		/* b .loop  */
	};
	uint32_t *ici_vector = (uint32_t*)(0xA0000000 + (0xFFF00F00 - 0xC0000000));
	memcpy(ici_vector, ici_stub, sizeof(ici_stub));
	DCFlushRange(ici_vector, sizeof(ici_stub));
	ICInvalidateRange(ici_vector, sizeof(ici_stub));

	/* Send ICIs to CPU0 and CPU2 */
	__KernelSendICI(3, 0, 0x050000, 0);
	__KernelSendICI(3, 2, 0x050000, 0);

	/* Create and start the stub loader */
	OSContext *thread1 = (OSContext*)OSAllocFromSystem(0x1000,8);
	uint32_t *stack1 = (uint32_t*)OSAllocFromSystem(0x1000,0x20);
	if (!OSCreateThread(thread1, &start_stubldr, 0, NULL, stack1 + 0x400, 0x1000, 0, 0x2 | 0x8)) OSFatal("Failed to create thread");
	OSResumeThread(thread1);

	/* Infinite loop */
	while(1);
}

/* Start the stub loader */
void start_stubldr(int argc, void *arg)
{
	/* Infinite loop */
	while(1);
}

/* memcpy() implementation */
void *memcpy(void* dst, const void* src, uint32_t size)
{
	uint32_t i;
	for (i = 0; i < size; i++)
		((uint8_t*) dst)[i] = ((const uint8_t*) src)[i];
	return dst;
}
