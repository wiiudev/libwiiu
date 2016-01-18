#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/types.h"
#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/socket.h"

/* Start a thread for RPC */
void _start();

/* RPC thread */
void rpc();

#endif /* LOADER_H */