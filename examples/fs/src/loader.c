#include "loader.h"

void _start()
{
    /* Get a handle to coreinit.rpl */
    uint32_t coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

    /* Memory allocation and FS functions */
    void* (*OSAllocFromSystem)(uint32_t size, int align);
    int (*FSInit)();
    int (*FSAddClient)(void *client, int unk1);
    int (*FSInitCmdBlock)(void *cmd);
    int (*FSOpenDir)(void *client, void *cmd, char *path, uint32_t *dir_handle, int unk1);
    int (*FSReadDir)(void *client, void *cmd, uint32_t dir_handle, void *buffer, int unk1);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
    OSDynLoad_FindExport(coreinit_handle, 0, "FSInit", &FSInit);
    OSDynLoad_FindExport(coreinit_handle, 0, "FSAddClient", &FSAddClient);
    OSDynLoad_FindExport(coreinit_handle, 0, "FSInitCmdBlock", &FSInitCmdBlock);
    OSDynLoad_FindExport(coreinit_handle, 0, "FSOpenDir", &FSOpenDir);
    OSDynLoad_FindExport(coreinit_handle, 0, "FSReadDir", &FSReadDir);

    /* Initialize the FS library */
    FSInit();

    /* Set up the client and command blocks */
    void *client = OSAllocFromSystem(0x1700, 0x20);
    void *cmd = OSAllocFromSystem(0xA80, 0x20);
    if (!(client && cmd)) OSFatal("Failed to allocate client and command block");
    FSAddClient(client, 0);
    FSInitCmdBlock(cmd);

    /* Open /vol/save */
    uint32_t dir_handle;
    FSOpenDir(client, cmd, "/vol/save", &dir_handle, -1);

    /* Start reading its directory entries */
    uint32_t *buffer = OSAllocFromSystem(0x200, 0x20);
    while (1)
    {
        /* Read the directory entry */
        int ret = FSReadDir(client, cmd, dir_handle, buffer, -1);
        if (ret != 0) break;

        /* Get the attributes, size, and name */
        uint32_t attr = buffer[0];
        uint32_t size = buffer[1];
        char *name = (char*)&buffer[25];

        /* Print it out */
        char buf[256];
        __os_snprintf("%s\t%d bytes\t%s", name, size, (attr & 0x80000000) ? "Directory" : "File");
        OSFatal(buf);
    }
}
