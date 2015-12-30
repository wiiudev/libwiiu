#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/socket.h"

/* Install an exception handler */
int _start(int argc, char **argv);

/* memcpy() implementation */
void *memcpy(void* dst, const void* src, unsigned int size);

#endif /* LOADER_H */