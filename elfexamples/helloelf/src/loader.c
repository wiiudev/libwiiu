#include "init.h"
#include "loader.h"

extern char __rx_start, __rx_end;
extern char __rw_start, __rw_end;
void _main()
{
    char buf[255];
    __os_snprintf(buf,255,"Hello, I am a ELF!\n"
                  "rx area:0x%08X-0x%08X\n"
                  "rw area:0x%08X-0x%08X\n",
                  &__rx_start,&__rx_end,&__rw_start,&__rw_end);
    _osscreeninit();
    _printstr(buf);
    unsigned int t1 = 0x60000000;
    while(t1--) ;
    _osscreenexit();
}

void flipBuffers()
{
    void(*DCFlushRange)(void *buffer, unsigned int length);
    unsigned int(*OSScreenFlipBuffersEx)(unsigned int bufferNum);
    OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenFlipBuffersEx", &OSScreenFlipBuffersEx);
    unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
    //Grab the buffer size for each screen (TV and gamepad)
    int buf0_size = OSScreenGetBufferSizeEx(0);
    int buf1_size = OSScreenGetBufferSizeEx(1);
    //Flush the cache
    DCFlushRange((void *)0xF4000000 + buf0_size, buf1_size);
    DCFlushRange((void *)0xF4000000, buf0_size);
    //Flip the buffer
    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);
}

void drawString(int x, int line, char * string)
{
    unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
    OSScreenPutFontEx(0, x, line, string);
    OSScreenPutFontEx(1, x, line, string);
}

void fillScreen(char r,char g,char b,char a)
{
    unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
    unsigned int num = (r << 24) | (g << 16) | (b << 8) | a;
    OSScreenClearBufferEx(0, num);
    OSScreenClearBufferEx(1, num);
}

void _osscreeninit()
{
    void(*OSScreenInit)();
    unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
    unsigned int(*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);
    //Call the Screen initilzation function.
    OSScreenInit();
    //Grab the buffer size for each screen (TV and gamepad)
    int buf0_size = OSScreenGetBufferSizeEx(0);
    int buf1_size = OSScreenGetBufferSizeEx(1);
    //Set the buffer area.
    OSScreenSetBufferEx(0, (void *)0xF4000000);
    OSScreenSetBufferEx(1, (void *)0xF4000000 + buf0_size);
}

void _printstr(char *str)
{
    int ii = 0;
    for (ii; ii < 2; ii++)
    {
        fillScreen(0,0,0,0);
        drawString(0,0,str);
        flipBuffers();
    }
}

void _osscreenexit()
{
    int ii=0;
    for(ii; ii<2; ii++)
    {
        fillScreen(0,0,0,0);
        flipBuffers();
    }
}
