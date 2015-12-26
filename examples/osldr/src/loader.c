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
		0x38601000,		/* li      r3,4096 */
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

	/* Create and start the kernel downloader thread */
	OSContext *thread1 = (OSContext*)OSAllocFromSystem(0x1000,8);
	uint32_t *stack1 = (uint32_t*)OSAllocFromSystem(0x1000,0x20);
	if (!OSCreateThread(thread1, &kernload_net, 1, "http://example.com/kernel.elf", stack1 + 0x400, 0x1000, 0, 0x2 | 0x8)) OSFatal("Failed to create thread");
	OSResumeThread(thread1);

	/* Infinite loop */
	while(1);
}

/* curl macros */
#define CURLOPT_URL				10002
#define CURLOPT_WRITEFUNCTION	20011
#define CURLOPT_WRITEDATA		0x0
#define CURLINFO_RESPONSE_CODE	0x200002

/* curl download callback */
static size_t write_cb(void *buf, size_t size, size_t nmemb, void *userdata)
{
	/* Convert context pointer to the byte count pointer */
	size_t *bytes_written = (size_t*)userdata;

	/* Copy the received data */
	size_t realsize = size * nmemb;
	memcpy((void*)(0xF4000000 + *bytes_written), buf, realsize);

	/* Increment the amount of bytes written and return */
	*bytes_written += realsize;
	return realsize;
}

/* Download the new kernel into MEM1 and run it */
void kernload_net(int argc, void *arg)
{
	/* Get a handle to coreinit.rpl and nlibcurl.rpl */
	uint32_t coreinit_handle, nlibcurl_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	OSDynLoad_Acquire("nlibcurl.rpl", &nlibcurl_handle);

	/* Memory functions */
	void (*DCFlushRange)(void *buffer, uint32_t length);
	void (*ICInvalidateRange)(void *buffer, uint32_t length);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "ICInvalidateRange", &ICInvalidateRange);

	/* curl functions */
	void* (*curl_easy_init)();
	void (*curl_easy_cleanup)(void *handle);
	void (*curl_easy_setopt)(void *handle, uint32_t param, void *op);
	int (*curl_easy_perform)(void *handle);
	void (*curl_easy_getinfo)(void *handle, uint32_t param, void *info);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_init", &curl_easy_init);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_cleanup", &curl_easy_init);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_setopt", &curl_easy_setopt);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_perform", &curl_easy_perform);
	OSDynLoad_FindExport(nlibcurl_handle, 0, "curl_easy_getinfo", &curl_easy_getinfo);

	/* Try to initialize curl */
	void *curl_handle = curl_easy_init();
	if (!curl_handle) OSFatal("Failed to initialize curl");

	/* Set the URL, callback, and context for callback */
	size_t bytes_written = 0;
	curl_easy_setopt(curl_handle, CURLOPT_URL, arg);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &write_cb);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &bytes_written);

	/* Try to perform the download */
	int status = curl_easy_perform(curl_handle);
	if (status != 0) OSFatal("Failed to download kernel");

	/* Check the HTPP error code and the number of bytes */
	status = 404;
	curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &status);
	if (status != 200) OSFatal("Failed to download kernel");
	if (bytes_written == 0 || bytes_written > 0x2000000) OSFatal("Invalid kernel size");

	/* Clean up the curl session */
	curl_easy_cleanup(curl_handle);

	/* Flush and invalidate the downloaded kernel in MEM1 */
	DCFlushRange((void*)0xF4000000, bytes_written);
	ICInvalidateRange((void*)0xF4000000, bytes_written);

	/* Make the syscall handler jump to 0, and start running the kernel */
	uint32_t sc_kaddr = 0xFFF00C00;
	uint32_t sc_stub[4] = 
	{
		0x38601000,		/* li      r3,4096 */
		0x7c600124, 	/* mtmsr   r3 */
		0x4c00012c, 	/* isync */
		0x48000002		/* ba 0 */
	};
	memcpy((void*)(0xA0000000 + (sc_kaddr - 0xC0000000)), sc_stub, sizeof(sc_stub));
	DCFlushRange((void*)(0xA0000000 + (sc_kaddr - 0xC0000000)), sizeof(sc_stub));
	ICInvalidateRange((void*)(0xA0000000 + (sc_kaddr - 0xC0000000)), sizeof(sc_stub));
	asm("sc");

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
