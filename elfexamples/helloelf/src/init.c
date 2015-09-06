#include "init.h"
void (*OSDynLoad_Acquire)(char* rpl, unsigned int *handle) = (void*)0;
void (*OSDynLoad_FindExport)(unsigned int handle, int isdata, char *symbol, void *address) = (void*)0;
void (*OSFatal)(char* msg) = (void*)0;
void (*__os_snprintf)(char* s, int n, const char * format, ... ) = (void*)0;
unsigned int coreinit_handle = 0;

void _doInit()
{
	OSDynLoad_Acquire = (void*)(*(unsigned int*)0xF5FFFFFC);
	OSDynLoad_FindExport = (void*)(*(unsigned int*)0xF5FFFFF8);
	OSFatal = (void*)(*(unsigned int*)0xF5FFFFF4);
	__os_snprintf = (void*)(*(unsigned int*)0xF5FFFFF0);
	coreinit_handle = *(unsigned int*)0xF5FFFFEC;
}

void _doExit()
{
	void(*_Exit)();
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	_Exit();
}

void _memset(void *b, int c, int len)
{
	int i;

	for (i = 0; i < len; i++)
		((unsigned char *)b)[i] = c;
}
