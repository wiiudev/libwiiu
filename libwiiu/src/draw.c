#include "draw.h"
#include "init.h"

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
	int buf0_size = OSScreenGetBufferSizeEx(0);
	int buf1_size = OSScreenGetBufferSizeEx(1);
	DCFlushRange((void *)0xF4000000 + buf0_size, buf1_size);
	DCFlushRange((void *)0xF4000000, buf0_size);
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void flipBuffersTv()
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
	int buf1_size = OSScreenGetBufferSizeEx(1);\
	DCFlushRange((void *)0xF4000000, buf0_size);
	OSScreenFlipBuffersEx(0);
}

void flipBuffersPad()
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
	DCFlushRange((void *)0xF4000000 + buf0_size, buf1_size);
	OSScreenFlipBuffersEx(1);
}

void drawString(int x, int y, char * string)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
	OSScreenPutFontEx(0, x, y, string);
	OSScreenPutFontEx(1, x, y, string);
}

void drawStringTv(int x, int y, char * string)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
	OSScreenPutFontEx(0, x, y, string);
}

void drawStringPad(int x, int y, char * string)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
	OSScreenPutFontEx(1, x, y, string);
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

void fillScreenTv(char r, char g, char b, char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	OSScreenClearBufferEx(0, num);
}

void fillScreenPad(char r, char g, char b, char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	OSScreenClearBufferEx(1, num);
}

void drawPixel(int x, int y, char r, char g, char b, char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	OSScreenPutPixelEx(0,x,y,num);
	OSScreenPutPixelEx(1,x,y,num);
}

void drawPixelTv(int x, int y, char r, char g, char b, char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	OSScreenPutPixelEx(0,x,y,num);
}

void drawPixelPad(int x, int y, char r, char g, char b, char a)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	OSScreenPutPixelEx(1,x,y,num);
}

void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{

	int x, y;
	if (x1 == x2){
		if (y1 < y2) for (y = y1; y <= y2; y++) drawPixel(x1, y, r, g, b, a);
		else for (y = y2; y <= y1; y++) drawPixel(x1, y, r, g, b, a);
	}
	else {
		if (x1 < x2) for (x = x1; x <= x2; x++) drawPixel(x, y1, r, g, b, a);
		else for (x = x2; x <= x1; x++) drawPixel(x, y1, r, g, b, a);
	}
}

void drawLineTv(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	int x, y;
	if (x1 == x2){
		if (y1 < y2) for (y = y1; y <= y2; y++) drawPixelTv(x1, y, r, g, b, a);
		else for (y = y2; y <= y1; y++) drawPixelTv(x1, y, r, g, b, a);
	}
	else {
		if (x1 < x2) for (x = x1; x <= x2; x++) drawPixelTv(x, y1, r, g, b, a);
		else for (x = x2; x <= x1; x++) drawPixelTv(x, y1, r, g, b, a);
	}
}

void drawLinePad(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	int x, y;
	if (x1 == x2){
		if (y1 < y2) for (y = y1; y <= y2; y++) drawPixelPad(x1, y, r, g, b, a);
		else for (y = y2; y <= y1; y++) drawPixelPad(x1, y, r, g, b, a);
	}
	else {
		if (x1 < x2) for (x = x1; x <= x2; x++) drawPixelPad(x, y1, r, g, b, a);
		else for (x = x2; x <= x1; x++) drawPixelPad(x, y1, r, g, b, a);
	}
}

void drawRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawLine(x1, y1, x2, y1, r, g, b, a);
	drawLine(x2, y1, x2, y2, r, g, b, a);
	drawLine(x1, y2, x2, y2, r, g, b, a);
	drawLine(x1, y1, x1, y2, r, g, b, a);
}

void drawRectTv(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawLineTv(x1, y1, x2, y1, r, g, b, a);
	drawLineTv(x2, y1, x2, y2, r, g, b, a);
	drawLineTv(x1, y2, x2, y2, r, g, b, a);
	drawLineTv(x1, y1, x1, y2, r, g, b, a);
}

void drawRectPad(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
{
	drawLinePad(x1, y1, x2, y1, r, g, b, a);
	drawLinePad(x2, y1, x2, y2, r, g, b, a);
	drawLinePad(x1, y2, x2, y2, r, g, b, a);
	drawLinePad(x1, y1, x1, y2, r, g, b, a);
}

void drawFillRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
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
			drawPixel(i, j, r, g, b, a);
		}
	}
}

void drawFillRectTv(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
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
			drawPixelTv(i, j, r, g, b, a);
		}
	}
}

void drawFillRectPad(int x1, int y1, int x2, int y2, char r, char g, char b, char a)
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
			drawPixelPad(i, j, r, g, b, a);
		}
	}
}

void drawCircle(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius * 4) / 4;
	drawCircleCircum(xCen, yCen, x, y, r, g, b, a);
	while (x < y){
		x++;
		if (p < 0){
			p += 2 * x + 1;
		}
		else{
			y--;
			p += 2 * (x - y) + 1;
		}
		drawCircleCircum(xCen, yCen, x, y, r, g, b, a);
	}
}

void drawCircleTv(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius * 4) / 4;
	drawCircleCircumTv(xCen, yCen, x, y, r, g, b, a);
	while (x < y){
		x++;
		if (p < 0){
			p += 2 * x + 1;
		}
		else{
			y--;
			p += 2 * (x - y) + 1;
		}
		drawCircleCircumTv(xCen, yCen, x, y, r, g, b, a);
	}
}

void drawCirclePad(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius * 4) / 4;
	drawCircleCircumPad(xCen, yCen, x, y, r, g, b, a);
	while (x < y){
		x++;
		if (p < 0){
			p += 2 * x + 1;
		}
		else{
			y--;
			p += 2 * (x - y) + 1;
		}
		drawCircleCircumPad(xCen, yCen, x, y, r, g, b, a);
	}
}

void drawFillCircle(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	drawCircle(xCen, yCen, radius, r, g, b, a);
	int x, y;
	for (y = -radius; y <= radius; y++){
		for (x = -radius; x <= radius; x++)
			if (x*x + y*y <= radius*radius + radius * .8f)
				drawPixel(xCen + x, yCen + y, r, g, b, a);
	}
}

void drawFillCircleTv(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	drawCircleTv(xCen, yCen, radius, r, g, b, a);
	int x, y;
	for (y = -radius; y <= radius; y++){
		for (x = -radius; x <= radius; x++)
			if (x*x + y*y <= radius*radius + radius * .8f)
				drawPixelTv(xCen + x, yCen + y, r, g, b, a);
	}
}

void drawFillCirclePad(int xCen, int yCen, int radius, char r, char g, char b, char a)
{
	drawCirclePad(xCen, yCen, radius, r, g, b, a);
	int x, y;
	for (y = -radius; y <= radius; y++){
		for (x = -radius; x <= radius; x++)
			if (x*x + y*y <= radius*radius + radius * .8f)
				drawPixelPad(xCen + x, yCen + y, r, g, b, a);
	}
}

void drawCircleCircum(int cx, int cy, int x, int y, char r, char g, char b, char a)
{

	if (x == 0){
		drawPixel(cx, cy + y, r, g, b, a);
		drawPixel(cx, cy - y, r, g, b, a);
		drawPixel(cx + y, cy, r, g, b, a);
		drawPixel(cx - y, cy, r, g, b, a);
	}
	if (x == y){
		drawPixel(cx + x, cy + y, r, g, b, a);
		drawPixel(cx - x, cy + y, r, g, b, a);
		drawPixel(cx + x, cy - y, r, g, b, a);
		drawPixel(cx - x, cy - y, r, g, b, a);
	}
	if (x < y){
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

void drawCircleCircumTv(int cx, int cy, int x, int y, char r, char g, char b, char a)
{
	if (x == 0){
		drawPixelTv(cx, cy + y, r, g, b, a);
		drawPixelTv(cx, cy - y, r, g, b, a);
		drawPixelTv(cx + y, cy, r, g, b, a);
		drawPixelTv(cx - y, cy, r, g, b, a);
	}
	if (x == y){
		drawPixelTv(cx + x, cy + y, r, g, b, a);
		drawPixelTv(cx - x, cy + y, r, g, b, a);
		drawPixelTv(cx + x, cy - y, r, g, b, a);
		drawPixelTv(cx - x, cy - y, r, g, b, a);
	}
	if (x < y){
		drawPixelTv(cx + x, cy + y, r, g, b, a);
		drawPixelTv(cx - x, cy + y, r, g, b, a);
		drawPixelTv(cx + x, cy - y, r, g, b, a);
		drawPixelTv(cx - x, cy - y, r, g, b, a);
		drawPixelTv(cx + y, cy + x, r, g, b, a);
		drawPixelTv(cx - y, cy + x, r, g, b, a);
		drawPixelTv(cx + y, cy - x, r, g, b, a);
		drawPixelTv(cx - y, cy - x, r, g, b, a);
	}
}

void drawCircleCircumPad(int cx, int cy, int x, int y, char r, char g, char b, char a)
{
	if (x == 0){
		drawPixelPad(cx, cy + y, r, g, b, a);
		drawPixelPad(cx, cy - y, r, g, b, a);
		drawPixelPad(cx + y, cy, r, g, b, a);
		drawPixelPad(cx - y, cy, r, g, b, a);
	}
	if (x == y){
		drawPixelPad(cx + x, cy + y, r, g, b, a);
		drawPixelPad(cx - x, cy + y, r, g, b, a);
		drawPixelPad(cx + x, cy - y, r, g, b, a);
		drawPixelPad(cx - x, cy - y, r, g, b, a);
	}
	if (x < y){
		drawPixelPad(cx + x, cy + y, r, g, b, a);
		drawPixelPad(cx - x, cy + y, r, g, b, a);
		drawPixelPad(cx + x, cy - y, r, g, b, a);
		drawPixelPad(cx - x, cy - y, r, g, b, a);
		drawPixelPad(cx + y, cy + x, r, g, b, a);
		drawPixelPad(cx - y, cy + x, r, g, b, a);
		drawPixelPad(cx + y, cy - x, r, g, b, a);
		drawPixelPad(cx - y, cy - x, r, g, b, a);
	}
}