#include "loader.h"
void printChar(char *str);
void myMemThread(int sth1, int *sth2);
#define OSTHREAD_SIZE	0x1000
void _start()
{
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
	);
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	/****************************>       External Prototypes       <****************************/
	//OSScreen functions
	void(*OSScreenInit)();
	unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
	unsigned int(*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
	//OS Memory functions
	void*(*memset)(void * dest, uint32_t value, uint32_t bytes);
	void*(*OSAllocFromSystem)(uint32_t size, int align);
	void(*OSFreeToSystem)(void *ptr);
	//IM functions
	int(*IM_Open)();
	int(*IM_Close)(int fd);
	int(*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b);
	/****************************>             Exports             <****************************/
	//OSScreen functions
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);
	//OS Memory functions
	OSDynLoad_FindExport(coreinit_handle, 0, "memset", &memset);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
	//IM functions
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Open", &IM_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Close", &IM_Close);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_SetDeviceState", &IM_SetDeviceState);
	/****************************>          Initial Setup          <****************************/
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
	//Call the Screen initilzation function.
	OSScreenInit();
	//Grab the buffer size for each screen (TV and gamepad)
	int buf0_size = OSScreenGetBufferSizeEx(0);
	int buf1_size = OSScreenGetBufferSizeEx(1);
	//Set the buffer area.
	OSScreenSetBufferEx(0, (void *)0xF4000000);
	OSScreenSetBufferEx(1, (void *)0xF4000000 + buf0_size);
	//Clear both framebuffers.
	int ii = 0;
	for (ii; ii < 2; ii++)
	{
		fillScreen(0,0,0,0);
		flipBuffers();
	}
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
	//current thread is broken, switch to a working one in core 1
	int ret = OSCreateThread(thread, myMemThread, 0, (void*)0, stack, 0x1000, 0, 0xA);
	if (ret == 0)
		OSFatal("Failed to create thread");

	/* Schedule it for execution */
	OSResumeThread(thread);
	/* SO UGLY but it works magically */
	asm volatile (
	"repeat:\n"
	"    nop\n"
	"    nop\n"
	"    nop\n"
	"    nop\n"
	"    nop\n"
	"    nop\n"
	"    nop\n"
	"    nop\n"
	"    b repeat\n"
	);
}
#if (VER==532)
#define MEM_SEARCH_START	0x17000000
#define MEM_SEARCH_END		0x26000000
#else
#define MEM_SEARCH_START	0x12000000
#define MEM_SEARCH_END		0x27FA0000
#endif

void myMemThread(int sth1, int *sth2)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit", &coreinit_handle);
	/* All of our Memory Mess */
	unsigned int(*MEMGetTotalFreeSizeForExpHeap)(void *expHeap);
	OSDynLoad_FindExport(coreinit_handle, 0, "MEMGetTotalFreeSizeForExpHeap", &MEMGetTotalFreeSizeForExpHeap);
	void*(*MEMDestroyExpHeap)(void *expHeap);
	OSDynLoad_FindExport(coreinit_handle, 0, "MEMDestroyExpHeap", &MEMDestroyExpHeap);
	void*(*MEMCreateExpHeapEx)(void *expHeap, unsigned int size, unsigned short flags);
	OSDynLoad_FindExport(coreinit_handle, 0, "MEMCreateExpHeapEx", &MEMCreateExpHeapEx);
	void*(*MEMAllocFromExpHeapEx)(void *expHeap, unsigned int size, int align);
	OSDynLoad_FindExport(coreinit_handle, 0, "MEMAllocFromExpHeapEx", &MEMAllocFromExpHeapEx);
	void*(*MEMFreeToExpHeap)(void *expHeap, void *mem);
	OSDynLoad_FindExport(coreinit_handle, 0, "MEMFreeToExpHeap", &MEMFreeToExpHeap);	
	unsigned char *str;
	unsigned char *memstart = (void*)0;
	unsigned char *memend = (void*)0;
	char buf[64];
	/* find a appropriate last heap to then count down from */
	for(str = (unsigned char*)MEM_SEARCH_START; str < (unsigned char*)MEM_SEARCH_END; str+=4)
	{
		if(*(unsigned int*)str == 0x45585048)
		{
			__os_snprintf(buf,64,"Found Start at %08x",str);
			printChar(buf);
			memstart = str;
			break;
		}
	}
	if(memstart == (void*)0) OSFatal("No Start Found!");	
	/* find a appropriate last heap to then count down from */
	for(str = (unsigned char*)(MEM_SEARCH_END-4); str > (unsigned char*)MEM_SEARCH_START; str-=4)
	{
		if(*(unsigned int*)str == 0x45585048)
		{
			__os_snprintf(buf,64,"Found Start at %08x, Found End at %08x",memstart,str);
			printChar(buf);
			memend = str;
			break;
		}
	}
	if(memend == (void*)0) OSFatal("No End Found!");
	/* get rid of all the existing heaps */
	for(str = memstart; str < memend; str+=4)
	{
		if(*(unsigned int*)str == 0x45585048)
		{
			__os_snprintf(buf,64,"Destroying %08x",str);
			printChar(buf);
			MEMDestroyExpHeap(str);
		}
	}
	float tmpflt = (float)(memend-memstart);
	tmpflt = tmpflt/1024/1024;	
	__os_snprintf(buf,64,"Creating a %.2fMB Heap",tmpflt);
	printChar(buf);	
	void *handle = MEMCreateExpHeapEx(memstart, (unsigned int)(memend-memstart), 0);
	if(handle == (void*)0) OSFatal("No Heap Created!");
	unsigned int fullsize = MEMGetTotalFreeSizeForExpHeap(handle);
	unsigned char *allmem = MEMAllocFromExpHeapEx(handle, fullsize-32, 32);
	if(allmem == (void*)0) OSFatal("Memory cant be allocated!");
	tmpflt = (float)fullsize-32;
	tmpflt = tmpflt/1024/1024;
	unsigned char *allmemend = allmem+(fullsize-32);
	unsigned char *i;
	for(i = allmem; i < allmemend; i+=4)
	{
		if( ( ((unsigned int)i) % 0x00100000 ) == 0 )
		{
			__os_snprintf(buf,64,"Checking %.2fMB starting at %08x... %08x",tmpflt,allmem,i);
			printChar(buf);
		}
		*(unsigned int*)i = 0x12345678;
		if(*(unsigned int*)i != 0x12345678) OSFatal("Memtest Failed!");
	}
	MEMFreeToExpHeap(handle, allmem);
	/* Verify that everything worked */
	float totalbytes = (float)MEMGetTotalFreeSizeForExpHeap(handle);
	__os_snprintf(buf,64,"All Good!");
	printChar(buf);
	unsigned int t1 = 0xFFFFFFF;
	while(t1--) ;	
	void(*_Exit)();
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	_Exit();
}
 
void printChar(char *str)
{
	int i=0;
	for(i;i<2;i++)
	{
		fillScreen(0,0,0,0);
		drawString(0,0,str);
		flipBuffers();
	}
}
