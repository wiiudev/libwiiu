#ifndef COREINIT_H
#define COREINIT_H

#if (VER==400) | (VER==410)
#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x1026e60)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x1028460)
#define OSFatal ((void (*)(char* msg))0x0102D01C)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x102b9ac)
#elif VER==500
#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x1029CD8)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x102B3E4)
#define OSFatal ((void (*)(char* msg))0x1030ECC)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x102ECE0)
#else
#error "Unsupported Wii U software version"
#endif

#endif /* COREINIT_H */