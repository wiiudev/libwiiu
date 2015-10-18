#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/socket.h"
#include "../../../libwiiu/src/types.h"

/* Kernel address table */
#if VER == 200
	#define KERN_ADDRESS_TBL		0xFFEB4E00
#elif VER == 210
	#define KERN_ADDRESS_TBL		0xFFEB4E40
#elif VER == 300
	#define KERN_ADDRESS_TBL		0xFFEB66E4
#elif VER == 310
	#define KERN_ADDRESS_TBL		0xFFEB66E4
#elif VER == 400
	#define KERN_ADDRESS_TBL		0xFFEB7E5C
#elif VER == 410
	#define KERN_ADDRESS_TBL		0xFFEB902C
#elif VER == 500
	#define KERN_ADDRESS_TBL		0xFFEA9E4C
#elif VER == 532
	#define KERN_ADDRESS_TBL		0xFFEAAA10
#elif VER == 550
	#define KERN_ADDRESS_TBL		0xFFEAB7A0
#else
#error "Unsupported Wii U software version"
#endif

void _start();

void _entryPoint();

void kern_write(void *addr, uint32_t value);
void* memcpy(void* dst, const void* src, uint32_t size);

#endif /* LOADER_H */