#ifndef COREINIT_H
#define COREINIT_H

#if (VER==300)
#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x01022CBC)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x01023D88)
#define OSFatal ((void (*)(char* msg))0x01028A68)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x01027390)
#elif (VER==400) | (VER==410)
#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x01026e60)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x01028460)
#define OSFatal ((void (*)(char* msg))0x0102D01C)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x0102b9ac)
#elif VER==500
#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x01029CD8)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x0102B3E4)
#define OSFatal ((void (*)(char* msg))0x01030ECC)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x0102ECE0)
#elif VER==532
#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x0)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x0)
#define OSFatal ((void (*)(char* msg))0x1031368)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x102f09c)
#else
#error "Unsupported Wii U software version"
#endif

/* ioctlv() I/O vector */
struct iovec
{
	void *buffer;
	int len;
	char unknown8[0xc-0x8];
};

#endif /* COREINIT_H */