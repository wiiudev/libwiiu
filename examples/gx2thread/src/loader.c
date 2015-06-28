#include "loader.h"

#define OSTHREAD_SIZE	0x1000
void myGXthread(int fbs, int *bufs);
void _start()
{
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
	);
	unsigned int coreinit_handle, gx2_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	OSDynLoad_Acquire("gx2", &gx2_handle);
	//OS Memory functions
	void*(*memset)(void * dest, unsigned int value, unsigned int bytes);
	void*(*OSAllocFromSystem)(unsigned int size, int align);
	void(*OSFreeToSystem)(void *ptr);
	//IM functions
	int(*IM_Open)();
	int(*IM_Close)(int fd);
	int(*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b);

	//OS Memory functions
	OSDynLoad_FindExport(coreinit_handle, 0, "memset", &memset);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
	//IM functions
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Open", &IM_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Close", &IM_Close);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_SetDeviceState", &IM_SetDeviceState);
	//Restart system to get lib access
	int fd = IM_Open();
	void *mem = OSAllocFromSystem(0x100, 64);
	memset(mem, 0, 0x100);
	//set restart flag to force quit browser
	IM_SetDeviceState(fd, mem, 3, 0, 0); 
	IM_Close(fd);
	OSFreeToSystem(mem);
	//wait a bit for browser end
	unsigned int t1 = 0x1FFFFFFF;
	while(t1--) ;

	/* Get the framebuffer of the TV or DRC */
	void(*GX2SwapScanBuffers)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SwapScanBuffers", &GX2SwapScanBuffers);
	unsigned char *abuseFunc = (unsigned char*)GX2SwapScanBuffers;
	unsigned short f_hi = *(unsigned short*)(abuseFunc+0x12);
	unsigned short f_lo = *(unsigned short*)(abuseFunc+0x16);
	unsigned int gx2settingBase = (((f_lo & 0x8000) ? (f_hi-1) : f_hi) << 16) | f_lo;
	unsigned int args[2];
	args[0] = *((unsigned int*)(gx2settingBase + 0x304));
	args[1] = *((unsigned int*)(gx2settingBase + 0x304 + 0x20));
	int(*OSGetCoreId)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSGetCoreId", &OSGetCoreId);
	void(*OSTestThreadCancel)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSTestThreadCancel", &OSTestThreadCancel);
	void(*GX2Shutdown)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Shutdown", &GX2Shutdown);
	int(*GX2GetMainCoreId)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2GetMainCoreId", &GX2GetMainCoreId);

	/* Prepare for our own death */
	void*(*OSGetCurrentThread)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSGetCurrentThread", &OSGetCurrentThread);
	void *myBorkedThread = OSGetCurrentThread();
	int (*OSSuspendThread)(void *thread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSSuspendThread", &OSSuspendThread);

	/* Prepare for thread startups */
	int (*OSCreateThread)(void *thread, void *entry, int argc, void *args, unsigned int stack, unsigned int stack_size, int priority, unsigned short attr);
	int (*OSResumeThread)(void *thread);
	int (*OSIsThreadTerminated)(void *thread);

	OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSIsThreadTerminated", &OSIsThreadTerminated);

	/* Allocate a stack for the thread */
	unsigned int stack = (unsigned int) OSAllocFromSystem(0x1000, 0x10);
	stack += 0x1000;
	/* Create the thread */
	void *thread = OSAllocFromSystem(OSTHREAD_SIZE, 8);
	
	if(OSGetCoreId() != GX2GetMainCoreId()) //needed for access without crashing
	{
		int ret = OSCreateThread(thread, GX2Shutdown, 0, (void*)0, stack, 0x1000, 0, 0x10 | (1<<GX2GetMainCoreId()));
		if (ret == 0)
			OSFatal("Failed to create thread");
		/* Schedule it for execution */
		OSResumeThread(thread);
		while(OSIsThreadTerminated(thread) == 0) ;
	}
	else //same core, easy
		GX2Shutdown();

	//current thread is broken, switch to a working one in core 1
	int ret = OSCreateThread(thread, myGXthread, 2, args, stack, 0x1000, 0, 0xA);
	if (ret == 0)
		OSFatal("Failed to create thread");

	/* Schedule it for execution */
	OSResumeThread(thread);
	/* SO UGLY but it works magically */
	while(1) ;
	//would be better but again, crashes 5.3.2
	//OSSuspendThread(myBorkedThread);
	//OSFatal("I am still not dead!");
}
void setupColorBuffer(GX2ColorBuffer *colorBuffer, unsigned int gx2_handle);
void myGXthread(int argc, int *argv)
{
	if(argc != 2) OSFatal("GX Thread did not start proper!");
	unsigned int *tv_fb = (unsigned int*)argv[0];
	unsigned int *drc_fb = (unsigned int*)argv[1];
	unsigned int coreinit_handle, gx2_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	OSDynLoad_Acquire("gx2", &gx2_handle);

	void(*GX2Init)(void *args);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Init", &GX2Init);
	GX2Init((void*)0);

	int(*OSGetCoreId)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSGetCoreId", &OSGetCoreId);
	void(*GX2SwapScanBuffers)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SwapScanBuffers", &GX2SwapScanBuffers);
	void(*GX2Shutdown)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Shutdown", &GX2Shutdown);
	int(*GX2GetMainCoreId)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2GetMainCoreId", &GX2GetMainCoreId);
	if(OSGetCoreId() != GX2GetMainCoreId()) OSFatal("GX Not switched!");

	void(*GX2DrawDone)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2DrawDone", &GX2DrawDone);
	void(*GX2Flush)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Flush", &GX2Flush);
	//void(*GX2SwapScanBuffers)();
	//OSDynLoad_FindExport(gx2_handle, 0, "GX2SwapScanBuffers", &GX2SwapScanBuffers);
	void(*GX2WaitForVsync)();
	OSDynLoad_FindExport(gx2_handle, 0, "GX2WaitForVsync", &GX2WaitForVsync);
	//Make sure buffer is usable for GX
	void(*GX2Invalidate)(unsigned int flags,void *buffer,unsigned int size);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2Invalidate", &GX2Invalidate);
	GX2Invalidate(0x40,tv_fb,(1280*720*4)*2);
	GX2Invalidate(0x40,drc_fb,(854*480*4)*2);
	//set it!
	int(*GX2SetTVBuffer)(void *buffer,unsigned int size,unsigned int flag1,unsigned int flag2,unsigned int flag3);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SetTVBuffer", &GX2SetTVBuffer);
	int(*GX2SetDRCBuffer)(void *buffer,unsigned int size,unsigned int flag1,unsigned int flag2,unsigned int flag3);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SetDRCBuffer", &GX2SetDRCBuffer);
	GX2SetTVBuffer(tv_fb,(1280*720*4)*2,3,0x1A,2); //test gradient demo setup for show
	GX2SetDRCBuffer(drc_fb,(854*480*4)*2,1,0x1A,2); //will look broken, no color buffer setup yet
	/* Draw is far from complete */
	/*void(*GX2DrawEx)(int type, int count, int start, int instances);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2DrawEx", &GX2DrawEx);
	void(*GX2SetAttribBuffer)(int index, int size, int vtxStride, void *buf);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SetAttribBuffer", &GX2SetAttribBuffer);
	float myclearer[8] =
	{
		0.0f,  0.0f,
		1.0f,  0.0f,
		0.0f,  1.0f,
		1.0f,  1.0f
	};
	GX2Invalidate(0x40, myclearer, sizeof(myclearer));*/

	GX2ColorBuffer myCBuf;
	setupColorBuffer(&myCBuf, gx2_handle);

	void(*GX2ClearColor)(GX2ColorBuffer *buffer, float r, float g, float b, float a);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2ClearColor", &GX2ClearColor);

	void(*GX2SwapBuffers)(GX2ColorBuffer *buffer);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2SwapBuffers", &GX2SwapBuffers);

	void(*GX2CopyColorBufferToScanBuffer)(GX2ColorBuffer *buffer, unsigned int target);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2CopyColorBufferToScanBuffer", &GX2CopyColorBufferToScanBuffer);

	float val = 1;
	float valInc = -0.005;
	val += valInc;
	while(val < 1)
	{
		val += valInc;
		//GX2SetAttribBuffer(0,sizeof(mybuf),8,mybuf);
		//GX2DrawEx(6,4,0,1);
		GX2ClearColor(&myCBuf, val, val, val, 1);
		GX2Invalidate(0x40, myCBuf.surface.imagePtr,myCBuf.surface.imageSize);
		GX2DrawDone();
		GX2Flush();
		GX2CopyColorBufferToScanBuffer(&myCBuf,1);
		GX2SwapScanBuffers();
		GX2Flush();
		GX2WaitForVsync();
		if(val <= 0) valInc = 0.005;
	}
	//clear for browser (needed?)
	GX2Shutdown();

	void(*_Exit)();
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	_Exit();
}

void setupColorBuffer(GX2ColorBuffer *colorBuffer, unsigned int gx2_handle)
{
	colorBuffer->surface.surfaceUse = 0xA;
    colorBuffer->surface.dim = 1;
    colorBuffer->surface.width = 1280;
    colorBuffer->surface.height = 720;
    colorBuffer->surface.depth = 1;
    colorBuffer->surface.numMips = 1;
    colorBuffer->surface.surfaceFormat = 0x1A;
    colorBuffer->surface.aa = 0;
    colorBuffer->surface.tileMode = 0;
    colorBuffer->surface.swizzle = 0;
    colorBuffer->viewMip = 0;
    colorBuffer->viewFirstSlice = 0;
    colorBuffer->viewNumSlices = 1;
	void(*GX2CalcSurfaceSizeAndAlignment)(GX2Surface *sufcace);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2CalcSurfaceSizeAndAlignment", &GX2CalcSurfaceSizeAndAlignment);
	GX2CalcSurfaceSizeAndAlignment(&colorBuffer->surface);
	void(*GX2InitColorBufferRegs)(GX2ColorBuffer *buffer);
	OSDynLoad_FindExport(gx2_handle, 0, "GX2InitColorBufferRegs", &GX2InitColorBufferRegs);
    GX2InitColorBufferRegs(colorBuffer);
	//DAMN thats dirty, ignore plz!
	colorBuffer->surface.imagePtr = (void*)((0xF6000000-colorBuffer->surface.imageSize)&~colorBuffer->surface.alignment);
}