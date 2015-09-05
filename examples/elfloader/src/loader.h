#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"

#if (VER==532)
#define rop ((void (*)(unsigned int r3))0xd8f502c)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x1035a6c)
#elif (VER==500)
#define rop ((void (*)())0x0DC80440)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x1035460)
#elif (VER==410)
#define rop ((void (*)())0x0E0D595C)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x01030ee4)
#elif (VER==400)
#define rop ((void (*)())0x0EA125FC)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x01030e9c)
#elif (VER==310)
#define rop ((void (*)())0x0E6CE574)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x0102B5C8)
#elif (VER==300)
#define rop ((void (*)())0x0E6CE574)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x0102B5A8)
#elif (VER==210)
#define rop ((void (*)())0x0E6C3A44)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x01029E84)
#elif (VER==200)
#define rop ((void (*)())0x0E65E8F4)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x01029C04)
#else
#define rop 0
#define memcpy 0
#endif

void _start();
void dorop(unsigned int dst, unsigned int src, int len);

#endif /* LOADER_H */