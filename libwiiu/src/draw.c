#include "draw.h"

void flipBuffers()
{
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

    void(*DCFlushRange)(void *buffer, uint32_t length);
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
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
    OSScreenPutFontEx(0, x, line, string);
    OSScreenPutFontEx(1, x, line, string);
}

void fillScreen(char r,char g,char b,char a)
{
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
    uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
    OSScreenClearBufferEx(0, num);
    OSScreenClearBufferEx(1, num);
}

//Rendering in
void drawPixel(int x, int y, char r, char g, char b, char a)
{
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
    uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
    OSScreenPutPixelEx(0,x,y,num);
    OSScreenPutPixelEx(1,x,y,num);
    //Code to write to framebuffer directly. For some reason this is only writing to one of the framebuffers when calling flipBuffers. Should provide speedup but needs investigation.
    /*
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);

    void(*memcpy)(void* dest, void* src, int length);
    OSDynLoad_FindExport(coreinit_handle, 0, "memcpy", &memcpy);
    int buf0_size = OSScreenGetBufferSizeEx(0);
    int height = 1024;
    int width = 1280;
    char *screen = (void *)0xF4000000;
    uint32_t v=(x + y*width)*4;
    screen[v]=r;
    screen[v+1]=g;
    screen[v+2]=b;
    screen[v+3]=a;

    height = 480;
    width = 896;
    char *screen2 = (void *)0xF4000000 + buf0_size;
    v=(x + y*width)*4;
    screen2[v]=r;
    screen2[v+1]=g;
    screen2[v+2]=b;
    screen2[v+3]=a;
    */
}

void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{

    int x, y;
    if (x1 == x2) {
        if (y1 < y2) for (y = y1; y <= y2; y++) drawPixel(x1, y, r, g, b, a);
        else for (y = y2; y <= y1; y++) drawPixel(x1, y, r, g, b, a);
    }
    else {
        if (x1 < x2) for (x = x1; x <= x2; x++) drawPixel(x, y1, r, g, b, a);
        else for (x = x2; x <= x1; x++) drawPixel(x, y1, r, g, b, a);
    }
}

void drawRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
    drawLine(x1, y1, x2, y1, r, g, b, a);
    drawLine(x2, y1, x2, y2, r, g, b, a);
    drawLine(x1, y2, x2, y2, r, g, b, a);
    drawLine(x1, y1, x1, y2, r, g, b, a);
}

void drawFillRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
    int X1, X2, Y1, Y2, i, j;

    if (x1 < x2) {
        X1 = x1;
        X2 = x2;
    }
    else {
        X1 = x2;
        X2 = x1;
    }

    if (y1 < y2) {
        Y1 = y1;
        Y2 = y2;
    }
    else {
        Y1 = y2;
        Y2 = y1;
    }
    for (i = X1; i <= X2; i++) {
        for (j = Y1; j <= Y2; j++) {
            drawPixel(i, j, r, g, b, a);
        }
    }
}

void drawCircle(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
    int x = 0;
    int y = radius;
    int p = (5 - radius * 4) / 4;
    drawCircleCircum(xCen, yCen, x, y, r, g, b, a);
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        }
        else {
            y--;
            p += 2 * (x - y) + 1;
        }
        drawCircleCircum(xCen, yCen, x, y, r, g, b, a);
    }
}

void drawFillCircle(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
    drawCircle(xCen, yCen, radius, r, g, b, a);
    int x, y;
    for (y = -radius; y <= radius; y++) {
        for (x = -radius; x <= radius; x++)
            if (x*x + y*y <= radius*radius + radius * .8f)
                drawPixel(xCen + x, yCen + y, r, g, b, a);
    }
}

void drawCircleCircum(int cx, int cy, int x, int y, char r, char g, char b, char a)
{

    if (x == 0) {
        drawPixel(cx, cy + y, r, g, b, a);
        drawPixel(cx, cy - y, r, g, b, a);
        drawPixel(cx + y, cy, r, g, b, a);
        drawPixel(cx - y, cy, r, g, b, a);
    }
    if (x == y) {
        drawPixel(cx + x, cy + y, r, g, b, a);
        drawPixel(cx - x, cy + y, r, g, b, a);
        drawPixel(cx + x, cy - y, r, g, b, a);
        drawPixel(cx - x, cy - y, r, g, b, a);
    }
    if (x < y) {
        drawPixel(cx + x, cy + y, r, g, b, a);
        drawPixel(cx - x, cy + y, r, g, b, a);
        drawPixel(cx + x, cy - y, r, g, b, a);
        drawPixel(cx - x, cy - y, r, g, b, a);
        drawPixel(cx + y, cy + x, r, g, b, a);
        drawPixel(cx - y, cy + x, r, g, b, a);
        drawPixel(cx + y, cy - x, r, g, b, a);
        drawPixel(cx - y, cy - x, r, g, b, a);
    }
}