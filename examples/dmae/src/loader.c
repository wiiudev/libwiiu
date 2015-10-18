#include "loader.h"

/* Start of our code */

void _start()
{
    /* Load a good stack */
    asm(
        "lis %r1, 0x1ab5 ;"
        "ori %r1, %r1, 0xd138 ;"
    );

    /* Get a handle to coreinit.rpl, dmae.rpl, and nsysnet.rpl */
    unsigned int coreinit_handle, dmae_handle, nsysnet_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    OSDynLoad_Acquire("dmae.rpl", &dmae_handle);
    OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);

    /* Cache, DMA, and socket functions */
    void (*DCFlushRange)(void *addr, unsigned int length);
    void (*DCInvalidateRange)(void *addr, unsigned int length);
    unsigned long long (*DMAECopyMem)(void *dst, void *src, unsigned int nwords, int endian);
    int (*DMAEWaitDone)(unsigned long long ret);
    int (*socket)(int family, int type, int proto);
    int (*connect)(int fd, struct sockaddr *addr, int addrlen);
    int (*send)(int fd, const void *buffer, int len, int flags);

    /* Read the addresses of the functions */
    OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
    OSDynLoad_FindExport(coreinit_handle, 0, "DCInvalidateRange", &DCInvalidateRange);
    OSDynLoad_FindExport(dmae_handle, 0, "DMAECopyMem", &DMAECopyMem);
    OSDynLoad_FindExport(dmae_handle, 0, "DMAEWaitDone", &DMAEWaitDone);
    OSDynLoad_FindExport(nsysnet_handle, 0, "socket", &socket);
    OSDynLoad_FindExport(nsysnet_handle, 0, "connect", &connect);
    OSDynLoad_FindExport(nsysnet_handle, 0, "send", &send);

    /* Set up our socket address structure */
    struct sockaddr sin;
    sin.sin_family = AF_INET;
    sin.sin_port = 12345;
    sin.sin_addr.s_addr = PC_IP;
    int i;
    for (i = 0; i < 8; i++)
    {
        sin.sin_zero[i] = 0;
    }

    /* Connect to the PC */
    int pc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int status = connect(pc, &sin, 0x10);
    if (status) OSFatal("Error connecting to PC server");

    /* Cache stuff */
    DCFlushRange((void*)0x01000000, 0x20);
    DCInvalidateRange((void*)0x1dd7b820, 0x20);

    /* Do the copy */
    int success = DMAEWaitDone(DMAECopyMem((void*)0x1dd7b820, (void*)0x01000000, 2, 0));
    if (success)
    {
        send(pc, "Success", 8, 0);
    }
    else
    {
        send(pc, "Fail", 5, 0);
    }
    while(1);
}