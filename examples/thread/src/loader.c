#include "loader.h"

#define OSTHREAD_SIZE	0x1000

/* Entry point for core 0 */
void core0_entry(int argc, void *args);

/* Start of our code */
void _start()
{
    /* Load a good stack */
    asm(
        "lis %r1, 0x1ab5 ;"
        "ori %r1, %r1, 0xd138 ;"
    );

    /* Get a handle to coreinit.rpl */
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

    /* OS memory allocation functions */
    void* (*OSAllocFromSystem)(uint32_t size, int align);

    /* OS thread functions */
    bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t stack, uint32_t stack_size, int32_t priority, uint16_t attr);
    int32_t (*OSResumeThread)(void *thread);

    /* Exit function */
    void (*_Exit)();

    /* Read the addresses of the functions */
    OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);

    /* Create a string argument */
    char *str = OSAllocFromSystem(6, 4);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = 0;

    /* Allocate a stack for the thread */
    uint32_t stack = (uint32_t) OSAllocFromSystem(0x1000, 0x10);
    stack += 0x1000;

    /* Create the thread */
    void *thread = OSAllocFromSystem(OSTHREAD_SIZE, 8);
    bool ret = OSCreateThread(thread, OSFatal, (int)str, null, stack, 0x1000, 0, 1);
    if (ret == false)
    {
        OSFatal("Failed to create thread");
    }

    /* Schedule it for execution */
    OSResumeThread(thread);

    /* Infinite loop */
    while(1);
}
