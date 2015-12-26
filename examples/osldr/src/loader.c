#include "loader.h"

/* Download the new kernel over Cafe OS's */
void kern_download(int argc, void *arg);

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

	/* Patch the ICI handler to reinitialize the CPU state and wait for a flag */

	/* Send ICIs to CPU0 and CPU2 */
	__KernelSendICI(3, 0, 0x050000, 0);
	__KernelSendICI(3, 2, 0x050000, 0);

	/* Create and start the kernel downloader thread */
	OSContext *thread1 = (OSContext*)OSAllocFromSystem(0x1000,8);
	uint32_t *stack1 = (uint32_t*)OSAllocFromSystem(0x1000,0x20);
	if (!OSCreateThread(thread1, &kern_download, 0, NULL, stack1 + 0x400, 0x1000, 0, 0x2 | 0x8)) OSFatal("Failed to create thread");
	OSResumeThread(thread1);

	/* Infinite loop */
	while(1);
}

/* Download the new kernel over Cafe OS's */
void kern_download(int argc, void *arg)
{
	/* Infinite loop */
	while(1);
}
