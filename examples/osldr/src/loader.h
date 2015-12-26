#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/types.h"

/* Trap CPU0 and CPU2 at the ICI handler, and get the new kernel loaded */
void _start();

/* Start the stub loader */
void start_stubldr(int argc, void *arg);

/* memcpy() implementation */
void *memcpy(void* dst, const void* src, uint32_t size);

#endif /* LOADER_H */
