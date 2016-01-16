#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/types.h"

/* Start a thread for the FS funcs */
void _start();

/* FS testing */
void fs_test();

/* Set a range of memory to a specific value */
void *memset(void *dst, int value, uint32_t len);

#endif /* LOADER_H */