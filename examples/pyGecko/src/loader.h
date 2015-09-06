#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/socket.h"
#include "../../../libwiiu/src/types.h"

/* Kernel address table */
#if VER == 200
	#define KERN_ADDRESS_TBL		0x0
#elif VER == 210
	#define KERN_ADDRESS_TBL		0x0
#elif VER == 300
	#define KERN_ADDRESS_TBL		0xFFEB66E4
#elif VER == 310
	#define KERN_ADDRESS_TBL		0xFFEB66E4
#elif VER == 400
	#define KERN_ADDRESS_TBL		0x0
#elif VER == 410
	#define KERN_ADDRESS_TBL		0xffeb902c
#elif VER == 500
	#define KERN_ADDRESS_TBL		0xffea9e4c
#elif VER == 532
	#define KERN_ADDRESS_TBL		0xFFEAAA10
#else
#error "Unsupported Wii U software version"
#endif

void _start();

void _entryPoint();

/* Arbitrary kernel write syscall */
void kern_write(void *addr, uint32_t value);

#endif /* LOADER_H */