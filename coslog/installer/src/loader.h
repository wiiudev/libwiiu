#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"

/* Kernel addresses */
#if VER == 532
	#define KERN_ADDRESS_TBL		0xFFEAAA10
#elif VER == 550
	#define KERN_ADDRESS_TBL		0xFFEAB7A0
#else
	#define KERN_ADDRESS_TBL		0x0
#endif

/* Install an exception handler */
void _start();

/* memcpy() implementation */
void *memcpy(void* dst, const void* src, unsigned int size);

/* Arbitrary read and write syscalls */
unsigned int kern_read(const void *addr);
void kern_write(void *addr, unsigned int value);

#endif /* LOADER_H */