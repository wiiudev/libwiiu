
#ifndef _INIT_H_
#define _INIT_H_

extern void (*OSDynLoad_Acquire)(char* rpl, unsigned int *handle);
extern void (*OSDynLoad_FindExport)(unsigned int handle, int isdata, char *symbol, void *address);
extern void (*OSFatal)(char* msg);
extern void (*__os_snprintf)(char* s, int n, const char * format, ... );
extern unsigned int coreinit_handle;

#endif
