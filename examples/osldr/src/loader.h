#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/types.h"

/* Syscall table address */
#if VER == 200
	#define KERN_SYSCALL_TBL		((void**)0x0)
#elif VER == 210
	#define KERN_SYSCALL_TBL		((void**)0x0)
#elif VER == 300
	#define KERN_SYSCALL_TBL		((void**)0xFFE85950)
#elif VER == 310
	#define KERN_SYSCALL_TBL		((void**)0x0)
#elif VER == 400
	#define KERN_SYSCALL_TBL		((void**)0x0)
#elif VER == 410
	#define KERN_SYSCALL_TBL		((void**)0xffe85890)
#elif VER == 500
	#define KERN_SYSCALL_TBL		((void**)0xffea9520)
#elif (VER == 532) || (VER == 540)
	#define KERN_SYSCALL_TBL		((void**)0xFFEAA0E0)
#elif VER == 550
	#define KERN_SYSCALL_TBL		((void**)0xFFEAAE60)
#else
#error "Unsupported Wii U software version"
#endif

/* Trap CPU0 and CPU2 at the ICI handler, and get the new kernel loaded */
void _start();

/* Download the new kernel into MEM1 and run it */
void kernload_net(int argc, void *arg);

/* memcpy() implementation */
void *memcpy(void* dst, const void* src, uint32_t size);

/* Arbitrary read and write syscalls */
uint32_t kern_read(const void *addr);
void kern_write(void *addr, uint32_t value);

#endif /* LOADER_H */
