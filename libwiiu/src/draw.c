#include "draw.h"

void flipBuffersEx(bool tv, bool pad)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	void(*DCFlushRange)(void *buffer, uint32_t length);
	unsigned int(*OSScreenFlipBuffersEx)(unsigned int bufferNum);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenFlipBuffersEx", &OSScreenFlipBuffersEx);
	unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
	int buf0_size = OSScreenGetBufferSizeEx(0);
	int buf1_size = OSScreenGetBufferSizeEx(1);
	
	if(tv) {
		DCFlushRange((void *)0xF4000000, buf0_size);
		OSScreenFlipBuffersEx(0);
	}
	if(pad) {
		DCFlushRange((void *)0xF4000000 + buf0_size, buf1_size);
		OSScreenFlipBuffersEx(1);
	}
}

void drawStringEx(bool tv, bool pad, int x, int y, char * string)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
	if(tv) OSScreenPutFontEx(0, x, y, string);
	if(pad) OSScreenPutFontEx(1, x, y, string);
}

void fillScreenEx(bool tv, bool pad, char r,char g,char b,char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	if(tv) OSScreenClearBufferEx(0, num);
	if(pad) OSScreenClearBufferEx(1, num);
}

void drawPixelEx(bool tv, bool pad, int x, int y, char r, char g, char b, char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	if(tv) OSScreenPutPixelEx(0,x,y,num);
	if(pad) OSScreenPutPixelEx(1,x,y,num);
}

void drawLineEx(bool tv, bool pad, int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{

	int x, y;
	if (x1 == x2){
		if (y1 < y2) for (y = y1; y <= y2; y++) drawPixelEx(tv, pad, x1, y, r, g, b, a);
		else for (y = y2; y <= y1; y++) drawPixelEx(tv, pad, x1, y, r, g, b, a);
	}
	else {
		if (x1 < x2) for (x = x1; x <= x2; x++) drawPixelEx(tv, pad, x, y1, r, g, b, a);
		else for (x = x2; x <= x1; x++) drawPixelEx(tv, pad, x, y1, r, g, b, a);
	}
}

void drawRectEx(bool tv, bool pad, int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawLineEx(tv, pad, x1, y1, x2, y1, r, g, b, a);
	drawLineEx(tv, pad, x2, y1, x2, y2, r, g, b, a);
	drawLineEx(tv, pad, x1, y2, x2, y2, r, g, b, a);
	drawLineEx(tv, pad, x1, y1, x1, y2, r, g, b, a);
}

void drawFillRectEx(bool tv, bool pad, int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	int X1, X2, Y1, Y2, i, j;

	if (x1 < x2){
		X1 = x1;
		X2 = x2;
	}
	else {
		X1 = x2;
		X2 = x1;
	}

	if (y1 < y2){
		Y1 = y1;
		Y2 = y2;
	}
	else {
		Y1 = y2;
		Y2 = y1;
	}
	for (i = X1; i <= X2; i++){
		for (j = Y1; j <= Y2; j++){
			drawPixelEx(tv, pad, i, j, r, g, b, a);
		}
	}
}

void drawCircleEx(bool tv, bool pad, int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius * 4) / 4;
	drawCircleCircumEx(tv, pad, xCen, yCen, x, y, r, g, b, a);
	while (x < y){
		x++;
		if (p < 0){
			p += 2 * x + 1;
		}
		else{
			y--;
			p += 2 * (x - y) + 1;
		}
		drawCircleCircumEx(tv, pad, xCen, yCen, x, y, r, g, b, a);
	}
}

void drawFillCircleEx(bool tv, bool pad, int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	drawCircleEx(tv, pad, xCen, yCen, radius, r, g, b, a);
	int x, y;
	for (y = -radius; y <= radius; y++){
		for (x = -radius; x <= radius; x++)
			if (x*x + y*y <= radius*radius + radius * .8f)
				drawPixelEx(tv, pad, xCen + x, yCen + y, r, g, b, a);
	}
}

void drawCircleCircumEx(bool tv, bool pad, int cx, int cy, int x, int y, char r, char g, char b, char a)
{

	if (x == 0){
		drawPixelEx(tv, pad, cx, cy + y, r, g, b, a);
		drawPixelEx(tv, pad, cx, cy - y, r, g, b, a);
		drawPixelEx(tv, pad, cx + y, cy, r, g, b, a);
		drawPixelEx(tv, pad, cx - y, cy, r, g, b, a);
	}
	if (x == y){
		drawPixelEx(tv, pad, cx + x, cy + y, r, g, b, a);
		drawPixelEx(tv, pad, cx - x, cy + y, r, g, b, a);
		drawPixelEx(tv, pad, cx + x, cy - y, r, g, b, a);
		drawPixelEx(tv, pad, cx - x, cy - y, r, g, b, a);
	}
	if (x < y){
		drawPixelEx(tv, pad, cx + x, cy + y, r, g, b, a);
		drawPixelEx(tv, pad, cx - x, cy + y, r, g, b, a);
		drawPixelEx(tv, pad, cx + x, cy - y, r, g, b, a);
		drawPixelEx(tv, pad, cx - x, cy - y, r, g, b, a);
		drawPixelEx(tv, pad, cx + y, cy + x, r, g, b, a);
		drawPixelEx(tv, pad, cx - y, cy + x, r, g, b, a);
		drawPixelEx(tv, pad, cx + y, cy - x, r, g, b, a);
		drawPixelEx(tv, pad, cx - y, cy - x, r, g, b, a);
	}
}

void flipBuffers()
{
	flipBuffersEx(true, true);
}

void drawString(int x, int y, char * string)
{
	drawStringEx(true, true, x, y, string);
}

void fillScreen(char r, char g, char b, char a)
{
	fillScreenEx(true, true, r, g, b, a);
}

void drawPixel(int x, int y, char r, char g, char b, char a)
{
	drawPixelEx(true, true, x, y, r, g, b, a);
}

void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawLineEx(true, true, x1, y1, x2, y2, r, g, b, a);
}

void drawRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawRectEx(true, true, x1, y1, x2, y2, r, g, b, a);
}

void drawFillRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawFillRectEx(true, true, x1, y1, x2, y2, r, g, b, a);
}

void drawCircle(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	drawCircleEx(true, true, xCen, yCen, radius, r, g, b, a);
}

void drawFillCircle(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	drawFillCircleEx(true, true, xCen, yCen, radius, r, g, b, a);
}

void drawCircleCircum(int cx, int cy, int x, int y, char r, char g, char b, char a)
{
	drawCircleCircumEx(true, true, cx, cy, x, y, r, g, b, a);
}