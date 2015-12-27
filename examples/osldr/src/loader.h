#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/types.h"

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
