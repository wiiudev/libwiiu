#include "loader.h"

void _start()
{
	/****************************>            Fix Stack            <****************************/
	//Load a good stack
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
		);
	/****************************>           Get Handles           <****************************/
	//Get a handle to coreinit.rpl
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	/****************************>       External Prototypes       <****************************/
	//OSScreen functions
	void(*OSScreenInit)();
	unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
	unsigned int(*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
	//OS Thread functions
	long(*OSCheckActiveThreads)();
	//Misc OS functions
	void(*OSRestartGame)();
	//OS Memory functions
	void(*DCFlushRange)(void *buffer, u32 length);
	/****************************>             Exports             <****************************/
	//OSScreen functions
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);
	//OSThread functions
	OSDynLoad_FindExport(coreinit_handle, 0, "OSCheckActiveThreads", &OSCheckActiveThreads);
	//Misc OS functions
	OSDynLoad_FindExport(coreinit_handle, 0, "OSRestartGame", &OSRestartGame);
	//OS Memory functions
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	/****************************>          Initial Setup          <****************************/
	//Restart the web browser. This stops active threads for browser. Browser GUI thread runs on CPU0.
	OSRestartGame();
	//Wait for the web browser to start closing by observing the total number of threads.
	long initialNumberOfThreads = OSCheckActiveThreads();
	long currentNumberOfThreads = OSCheckActiveThreads();
	while (initialNumberOfThreads - currentNumberOfThreads < 3)
	{
		currentNumberOfThreads = OSCheckActiveThreads();
	}
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
	//Jump to entry point.
	_entryPoint();
}