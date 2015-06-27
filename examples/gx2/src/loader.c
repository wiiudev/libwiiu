#include "loader.h"

void _start()
{
	/* Get a handle to coreinit.rpl and gx2.rpl */
	uint32_t coreinit_handle, gx2_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	OSDynLoad_Acquire("gx2.rpl", &gx2_handle);

	/* OS memory and IM functions */
	void* (*memset)(void *dest, uint32_t value, uint32_t bytes);
	void* (*OSAllocFromSystem)(uint32_t size, int align);
	void (*OSFreeToSystem)(void *ptr);
	int(*IM_Open)();
	int(*IM_Close)(int fd);
	int(*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b);
	OSDynLoad_FindExport(coreinit_handle, 0, "memset", &memset);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Open", &IM_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Close", &IM_Close);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_SetDeviceState", &IM_SetDeviceState);

	/* GX2 display functions */
	void (*GX2SetDRCEnable)(bool enable);
	void (*GX2WaitForVsync)();
	void (*GX2SwapScanBuffers)();
	void (*GX2Invalidate)(int buf_type, void *addr, int length);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SetDRCEnable", &GX2SetDRCEnable);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2WaitForVsync", &GX2WaitForVsync);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SwapScanBuffers", &GX2SwapScanBuffers);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Invalidate", &GX2Invalidate);

	/* Cause the other browser threads to exit */
	int fd = IM_Open();
	void *mem = OSAllocFromSystem(0x100, 64);
	memset(mem, 0, 0x100);
	IM_SetDeviceState(fd, mem, 3, 0, 0); 
	IM_Close(fd);
	OSFreeToSystem(mem);

	/* Wait for the threads to exit */
	unsigned int t1 = 0x1FFFFFFF;
	while(t1--);

	/* Get the framebuffer of the TV or DRC */
	int dev = 1;
	uint32_t *fb_front = (uint32_t*) *((uint32_t*)(0x1031AEC0 + 0x304 + (dev * 0x20)));
	uint32_t *fb_back = fb_front + (864 * 480);

	/* Write blue to the screen and swap the buffers */
	for (int i = 0; i < 864 * 320; i++) *fb_back++ = 0x00FF00FF;
	GX2Invalidate(0x40, fb_back, 864 * 320 * 4);
	GX2WaitForVsync();
	GX2SwapScanBuffers();

	void (*OSScreenInit)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
	OSScreenInit();

	while(1);
}
