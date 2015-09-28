
/* WiiU ELF Loader by FIX94 */

#include "loader.h"

#if (VER==532)
#define BUFFER_ADDR 0x1b201814
#else
#define BUFFER_ADDR 0x1dd7b814
#endif

#define OSTHREAD_SIZE	0x1000

void myMemThread(int sth1, int *sth2);
void doexit();

void _start()
{
    /* Get a safer stack */
    asm(
        "lis %r1, 0x1ab5 ;"
        "ori %r1, %r1, 0xd138 ;"
    );
    /* Get coreinit handle and keep it in memory */
    OSDynLoad_Acquire("coreinit", (unsigned int*)0xF5FFFFEC);
    unsigned int coreinit_handle = *(unsigned int*)0xF5FFFFEC;

    /* Get our memory functions */
    void*(*memset)(void * dest, unsigned int value, unsigned int bytes);
    void*(*OSAllocFromSystem)(unsigned int size, int align);
    void(*OSFreeToSystem)(void *ptr);
    OSDynLoad_FindExport(coreinit_handle, 0, "memset", &memset);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);

    /* Get functions to send restart signal */
    int(*IM_Open)();
    int(*IM_Close)(int fd);
    int(*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b);
    OSDynLoad_FindExport(coreinit_handle, 0, "IM_Open", &IM_Open);
    OSDynLoad_FindExport(coreinit_handle, 0, "IM_Close", &IM_Close);
    OSDynLoad_FindExport(coreinit_handle, 0, "IM_SetDeviceState", &IM_SetDeviceState);

    /* Send restart signal to get rid of unneded threads */
    int fd = IM_Open();
    void *mem = OSAllocFromSystem(0x100, 64);
    memset(mem, 0, 0x100);

    /* Sets wanted flag */
    IM_SetDeviceState(fd, mem, 3, 0, 0);
    IM_Close(fd);
    OSFreeToSystem(mem);

    /* Waits for thread exits */
    unsigned int t1 = 0x1FFFFFFF;
    while(t1--) ;

    /* Prepare for thread startups */
    int (*OSCreateThread)(void *thread, void *entry, int argc, void *args, unsigned int stack, unsigned int stack_size, int priority, unsigned short attr);
    int (*OSResumeThread)(void *thread);
    int (*OSIsThreadTerminated)(void *thread);

    OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSIsThreadTerminated", &OSIsThreadTerminated);

    /* Allocate a stack for the thread */
    /* IMPORTANT: libcurl uses around 0x1000 internally so make
    	sure to allocate more for the stack to avoid crashes */
    void *stack = OSAllocFromSystem(0x1200, 0x10);

    /* Create the thread */
    void *thread = OSAllocFromSystem(OSTHREAD_SIZE, 8);
    //current thread is broken, switch to a working one in core 1
    int ret = OSCreateThread(thread, myMemThread, 0, (void*)0, (unsigned int)stack+0x1200, 0x1200, 0, 0x1A);
    if (ret == 0)
        OSFatal("Failed to create thread");

    /* Schedule it for execution */
    OSResumeThread(thread);

    /* Keep this main thread around for ELF loading */
    while(OSIsThreadTerminated(thread) == 0)
    {
        asm volatile (
            "    nop\n"
            "    nop\n"
            "    nop\n"
            "    nop\n"
            "    nop\n"
            "    nop\n"
            "    nop\n"
            "    nop\n"
        );
    }
    /* Make sure we actually have a ELF */
    if(valid_elf_image() != 1) doexit();

    /* Start ELF Loadup */
    *(int*)0xF5FFFFFC = 0;
    load_elf_image();
}

/* Gets called in case something goes wrong */
void doexit()
{
    unsigned int coreinit_handle = *(unsigned int*)0xF5FFFFEC;
    void(*_Exit)();
    OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
    _Exit();
}

/* libcurl data write callback */
int write_data(void *buffer, int size, int nmemb, void *userp)
{
    int *filepos = (int*)0xF5FFFFFC;
    int insize = size*nmemb;
    if((*filepos)+insize > 0x7FFFFC) doexit();
    memcpy((void*)0xF5800000+(*filepos), buffer, insize);
    (*filepos) += insize;
    return insize;
}

/* The downloader thread */
#define CURLOPT_URL 10002
#define CURLOPT_WRITEFUNCTION 20011
#define CURLINFO_RESPONSE_CODE 0x200002
void myMemThread(int sth1, int *sth2)
{
    unsigned int coreinit_handle = *(unsigned int*)0xF5FFFFEC;
    char *leaddr = (char*)0;
    unsigned char *str;
    char buf[64];

    /* find address left in ram */
    for(str = (unsigned char*)0x1A000000; str < (unsigned char*)0x20000000; str++)
    {   /* Search for /payload which indicates the current address */
        if(*(unsigned int*)str == 0x2F706179 && *(unsigned int*)(str+4) == 0x6C6F6164)
        {
            leaddr = (char*)str;
            while(*leaddr) leaddr--;
            leaddr++;
            /* If string starts with http its likely to be correct */
            if(*(unsigned int*)leaddr == 0x68747470)
                break;
            leaddr = (char*)0;
        }
    }
    if(leaddr == (char*)0) doexit();

    /* Generate the boot.elf address */
    memcpy(buf,leaddr,64);
    char *ptr = buf;
    while(*ptr) ptr++;
    while(*ptr != 0x2F) ptr--;
    memcpy(ptr+1, "boot.elf", 9);

    /* Acquire and setup libcurl */
    *(int*)0xF5FFFFFC = 0;
    unsigned int libcurl_handle;
    OSDynLoad_Acquire("nlibcurl", &libcurl_handle);
    void*(*curl_easy_init)();
    OSDynLoad_FindExport(libcurl_handle, 0, "curl_easy_init", &curl_easy_init);
    void *curl = curl_easy_init();
    if(!curl) doexit();
    void(*curl_easy_setopt)(void *handle, unsigned int param, void *op);
    OSDynLoad_FindExport(libcurl_handle, 0, "curl_easy_setopt", &curl_easy_setopt);
    curl_easy_setopt(curl, CURLOPT_URL, buf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    /* Download file */
    int(*curl_easy_perform)(void *handle);
    OSDynLoad_FindExport(libcurl_handle, 0, "curl_easy_perform", &curl_easy_perform);
    int ret = curl_easy_perform(curl);
    if(ret) doexit();

    /* Do error checks */
    int datalen = *(int*)0xF5FFFFFC;
    if(!datalen || datalen > 0x7FFFFC) doexit();

    void(*curl_easy_getinfo)(void *handle, unsigned int param, void *op);
    OSDynLoad_FindExport(libcurl_handle, 0, "curl_easy_getinfo", &curl_easy_getinfo);
    int resp = 404;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp);
    if(resp != 200) doexit();

    /* Cleanup to gain back memory */
    void (*curl_easy_cleanup)(void *handle);
    OSDynLoad_FindExport(libcurl_handle, 0, "curl_easy_cleanup", &curl_easy_cleanup);
    curl_easy_cleanup(curl);

    /* Release libcurl and exit thread */
    void (*OSDynLoad_Release)(unsigned int handle);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSDynLoad_Release", &OSDynLoad_Release);
    OSDynLoad_Release(libcurl_handle);
}

/* rop recover */
void ropend()
{
    asm volatile(
        "lis %r3, 0xF600 ;"
        "lwz %r1, -8(%r3);"
    );
    load_elf_image();
    doexit();
}

/* rop save */
void ropstart()
{
    asm volatile(
        "lis %r3, 0xF600 ;"
        "stw %r1, -8(%r3);"
    );
#if (VER==532)
    rop(BUFFER_ADDR + 0x30c);
#else
    asm(
        "lis %r1, 0x1dd7 ;"
        "ori %r1, %r1, 0xb814 ;"
    );
    rop();
#endif
}

/* Prepares and executes rop chain to copy desired data */
void dorop(unsigned int dst, unsigned int src, int len)
{
    /* Bring custom stack into position */
#if (VER==532)
    memcpy((void *)BUFFER_ADDR, (void*)BUFFER_ADDR + 0x1000, 0x5C0);
#else
    memcpy((void *)BUFFER_ADDR, (void *)BUFFER_ADDR + 0x800, 0x600);
#endif

    /* Setup stack to needs */
#if (VER==532)
    unsigned int *ropbuf = (unsigned int*)BUFFER_ADDR;
    ropbuf[0x318/4] = (unsigned int)&ropbuf[0xa0/4];
    ropbuf[0xd8/4] = dst;
    ropbuf[0x108/4] = dst;
    ropbuf[0x138/4] = dst;
    ropbuf[0xdc/4] = src;
    ropbuf[0xe0/4] = len;
    ropbuf[0x10c/4] = len;
    ropbuf[0x13c/4] = len;
    ropbuf[0x184/4] = (unsigned int)ropend;
#elif (VER==500 || VER==410)
    unsigned int * dst_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x88);
    unsigned int * dst_addr_2 = (unsigned int *)(BUFFER_ADDR + 0xBC);
    unsigned int * dst_addr_3 = (unsigned int *)(BUFFER_ADDR + 0xDC);
    unsigned int * dst_addr_4 = (unsigned int *)(BUFFER_ADDR + 0x114);
    unsigned int * dst_addr_5 = (unsigned int *)(BUFFER_ADDR + 0x134);
    unsigned int * dst_addr_6 = (unsigned int *)(BUFFER_ADDR + 0x16C);
    *dst_addr_1 = dst;
    *dst_addr_2 = dst;
    *dst_addr_3 = dst;
    *dst_addr_4 = dst;
    *dst_addr_5 = dst;
    *dst_addr_6 = dst;
    unsigned int * src_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x94);
    *src_addr_1 = src;
    unsigned int * len_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x60);
    unsigned int * len_addr_2 = (unsigned int *)(BUFFER_ADDR + 0xEC);
    unsigned int * len_addr_3 = (unsigned int *)(BUFFER_ADDR + 0x144);
    *len_addr_1 = len;
    *len_addr_2 = len;
    *len_addr_3 = len;
    unsigned int * ret_addr = (unsigned int *)(BUFFER_ADDR + 0x19C);
    *ret_addr = (unsigned int)ropend;
#elif (VER==400)
    unsigned int * dst_addr_1 = (unsigned int *)(BUFFER_ADDR + 0xAC);
    unsigned int * dst_addr_2 = (unsigned int *)(BUFFER_ADDR + 0xC4);
    unsigned int * dst_addr_3 = (unsigned int *)(BUFFER_ADDR + 0x104);
    *dst_addr_1 = dst;
    *dst_addr_2 = dst;
    *dst_addr_3 = dst;
    unsigned int * src_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x2C);
    unsigned int * src_addr_2 = (unsigned int *)(BUFFER_ADDR + 0x44);
    *src_addr_1 = src;
    *src_addr_2 = src;
    unsigned int * len_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x58);
    unsigned int * len_addr_2 = (unsigned int *)(BUFFER_ADDR + 0xD4);
    unsigned int * len_addr_3 = (unsigned int *)(BUFFER_ADDR + 0xEC);
    unsigned int * len_addr_4 = (unsigned int *)(BUFFER_ADDR + 0x114);
    unsigned int * len_addr_5 = (unsigned int *)(BUFFER_ADDR + 0x12C);
    *len_addr_1 = len;
    *len_addr_2 = len;
    *len_addr_3 = len;
    *len_addr_4 = len;
    *len_addr_5 = len;
    unsigned int * ret_addr = (unsigned int *)(BUFFER_ADDR + 0x154);
    *ret_addr = (unsigned int)ropend;
#else
    unsigned int * dst_addr_1 = (unsigned int *)(BUFFER_ADDR + 0xAC);
    unsigned int * dst_addr_2 = (unsigned int *)(BUFFER_ADDR + 0xC4);
    unsigned int * dst_addr_3 = (unsigned int *)(BUFFER_ADDR + 0xEC);
    unsigned int * dst_addr_4 = (unsigned int *)(BUFFER_ADDR + 0x104);
    unsigned int * dst_addr_5 = (unsigned int *)(BUFFER_ADDR + 0x12C);
    *dst_addr_1 = dst;
    *dst_addr_2 = dst;
    *dst_addr_3 = dst;
    *dst_addr_4 = dst;
    *dst_addr_5 = dst;
    unsigned int * src_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x2C);
    unsigned int * src_addr_2 = (unsigned int *)(BUFFER_ADDR + 0x44);
    unsigned int * src_addr_3 = (unsigned int *)(BUFFER_ADDR + 0x54);
    unsigned int * src_addr_4 = (unsigned int *)(BUFFER_ADDR + 0x7C);
    *src_addr_1 = src;
    *src_addr_2 = src;
    *src_addr_3 = src;
    *src_addr_4 = src;
    unsigned int * len_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x78);
    unsigned int * len_addr_2 = (unsigned int *)(BUFFER_ADDR + 0xD4);
    unsigned int * len_addr_3 = (unsigned int *)(BUFFER_ADDR + 0x114);
    *len_addr_1 = len;
    *len_addr_2 = len;
    *len_addr_3 = len;
    unsigned int * ret_addr = (unsigned int *)(BUFFER_ADDR + 0x154);
    *ret_addr = (unsigned int)ropend;
#endif
    /* Go! */
    ropstart();
}
