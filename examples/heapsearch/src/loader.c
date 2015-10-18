#include "loader.h"

void _start()
{
    asm(
        "lis %r1, 0x1ab5 ;"
        "ori %r1, %r1, 0xd138 ;"
    );
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit", &coreinit_handle);
    unsigned int(*MEMGetTotalFreeSizeForExpHeap)(void*expHeap);
    OSDynLoad_FindExport(coreinit_handle, 0, "MEMGetTotalFreeSizeForExpHeap", &MEMGetTotalFreeSizeForExpHeap);
    unsigned char *str;
    void *expHeap = (void*)0;
    float expSize = 0.f;
    /* this area appears semi-safe to search through */
    for(str = (unsigned char*)0x15000000; str < (unsigned char*)0x25000000; str+=4)
    {
        if(*(unsigned int*)str == 0x45585048)
        {
            void *tmpHeap = str;
            float tmpSize = (float)MEMGetTotalFreeSizeForExpHeap(tmpHeap);
            if(tmpSize > expSize)
            {
                expHeap = tmpHeap;
                expSize = tmpSize;
            }
        }
    }
    if(expHeap != (void*)0)
    {
        char buf[64];
        __os_snprintf(buf,64,"Biggest ExpHeap is %08x with %.2fMB left", expHeap, expSize/1024/1024);
        OSFatal(buf);
    }
    OSFatal("No Heaps found!");
}