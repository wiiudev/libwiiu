#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/types.h"
#include "../../../libwiiu/src/socket.h"

int strlen(char *str);

void _start();

void _entryPoint();
#endif /* LOADER_H */