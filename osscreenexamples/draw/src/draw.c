#include "draw.h"

inline int draw_max(int value1, int value2) {
	return value1 > value2 ? value1 : value2;
}

inline int draw_min(int value1, int value2) {
	return value1 < value2 ? value1 : value2;
}

inline int draw_abs(int value) {
	return value < 0 ? value * (-1) : value;
}

void flipBuffers(int screens) {
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
	if(screens & SCREEN_TV)
		DCFlushRange((void *)0xF4000000 + buf0_size, buf1_size);
	if(screens & SCREEN_PAD)
		DCFlushRange((void *)0xF4000000, buf0_size);
	//Flip the buffer
	if(screens & SCREEN_TV)
		OSScreenFlipBuffersEx(0);

	if(screens & SCREEN_PAD)
		OSScreenFlipBuffersEx(1);
}

void drawString(int screens, int x, int line, char * string) {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int line, void * buffer);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);

	if(screens & SCREEN_TV)
		OSScreenPutFontEx(0, x, line, string);

	if(screens & SCREEN_PAD)
		OSScreenPutFontEx(1, x, line, string);
}

void fillScreen(int screens, char r, char g, char b, char a) {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	if(screens & SCREEN_TV)
		OSScreenClearBufferEx(0, num);
	if(screens & SCREEN_PAD)
		OSScreenClearBufferEx(1, num);
}

//Rendering in 
void drawPixel(int screens, int scale, int x, int y, char r, char g, char b, char a) {
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, uint32_t color);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
	uint32_t num = (r << 24) | (g << 16) | (b << 8) | a;
	int add_x, add_y;

	if(screens & SCREEN_TV) {
		for(add_x = 0; add_x < scale; add_x++) {
			for(add_y = 0; add_y < scale; add_y++) {
				OSScreenPutPixelEx(0, x * scale + add_x, y * scale + add_y, num);
			}
		}
	}

	if(screens & SCREEN_PAD) {
		for(add_x = 0; add_x < scale; add_x++) {
			for(add_y = 0; add_y < scale; add_y++) {
				OSScreenPutPixelEx(1, x * scale + add_x, y * scale + add_y, num);
			}
		}
	}
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

void drawLine(int screens, int scale, int x1, int y1, int x2, int y2, char r, char g, char b, char a) {
	int dx = draw_abs(x1 - x2), dy = draw_abs(y1 - y2);
	int mx = draw_min(x1, x2);
	int my = draw_min(y1, y2);

	if((dx == 0) && (dy == 0))
		return;

	int x, y;
	int X1, X2, Y1, Y2;

	if (dx < dy) {
		// make surre that Y1 < Y2
		if (y1 > y2) {
			X1 = x2;
			Y1 = y2;

			X2 = x1;
			Y2 = y1;
		} else {
			X1 = x1;
			Y1 = y1;

			X2 = x2;
			Y2 = y2;
		}

		for (y = Y1; y <= Y2; y++) {
			int p = Y2 - y;
			if(X1 < X2)
				p = dy - p;

			x = mx + dx * p / dy;
			drawPixel(screens, scale, x, y, r, g, b, a);
		}
	} else {
		// make surre that X1 < X2
		if (x1 > x2) {
			X1 = x2;
			Y1 = y2;

			X2 = x1;
			Y2 = y1;
		} else {
			X1 = x1;
			Y1 = y1;

			X2 = x2;
			Y2 = y2;
		}

		for (x = X1; x <= X2; x++) {
			int p = X2 - x;
			if(Y1 < Y2)
				p = dx - p;

			y = my + dy * p / dx;
			drawPixel(screens, scale, x, y, r, g, b, a);
		}
	}
}

void drawRect(int screens, int scale, int x1, int y1, int x2, int y2, char r, char g, char b, char a) {
	drawLine(screens, scale, x1, y1, x2, y1, r, g, b, a);
	drawLine(screens, scale, x2, y1, x2, y2, r, g, b, a);
	drawLine(screens, scale, x1, y2, x2, y2, r, g, b, a);
	drawLine(screens, scale, x1, y1, x1, y2, r, g, b, a);
}

void drawFillRect(int screens, int scale, int x1, int y1, int x2, int y2, char r, char g, char b, char a) {
	int X1, X2, Y1, Y2, i, j;

	if (x1 < x2) {
		X1 = x1;
		X2 = x2;
	} else {
		X1 = x2;
		X2 = x1;
	}

	if (y1 < y2){
		Y1 = y1;
		Y2 = y2;
	} else {
		Y1 = y2;
		Y2 = y1;
	}

	for (i = X1; i <= X2; i++){
		for (j = Y1; j <= Y2; j++){
			drawPixel(screens, scale, i, j, r, g, b, a);
		}
	}
}

void drawCircle(int screens, int scale, int xCen, int yCen, int radius, char r, char g, char b, char a) {
	int x = 0;
	int y = radius;
	int p = (5 - radius * 4) / 4;
	drawCircleCircum(screens, scale, xCen, yCen, x, y, r, g, b, a);
	while (x < y){
		x++;
		if (p < 0){
			p += 2 * x + 1;
		} else{
			y--;
			p += 2 * (x - y) + 1;
		}
		drawCircleCircum(screens, scale, xCen, yCen, x, y, r, g, b, a);
	}
}

void drawFillCircle(int screens, int scale, int xCen, int yCen, int radius, char r, char g, char b, char a) {
	drawCircle(screens, scale, xCen, yCen, radius, r, g, b, a);
	int x, y;
	for (y = -radius; y <= radius; y++){
		for (x = -radius; x <= radius; x++)
			if (x*x + y*y <= radius*radius + radius * .8f)
				drawPixel(screens, scale, xCen + x, yCen + y, r, g, b, a);
	}
}

void drawCircleCircum(int screens, int scale, int cx, int cy, int x, int y, char r, char g, char b, char a) {
	if (x == 0) {
		drawPixel(screens, scale, cx, cy + y, r, g, b, a);
		drawPixel(screens, scale, cx, cy - y, r, g, b, a);
		drawPixel(screens, scale, cx + y, cy, r, g, b, a);
		drawPixel(screens, scale, cx - y, cy, r, g, b, a);
	}

	if (x == y) {
		drawPixel(screens, scale, cx + x, cy + y, r, g, b, a);
		drawPixel(screens, scale, cx - x, cy + y, r, g, b, a);
		drawPixel(screens, scale, cx + x, cy - y, r, g, b, a);
		drawPixel(screens, scale, cx - x, cy - y, r, g, b, a);
	}

	if (x < y) {
		drawPixel(screens, scale, cx + x, cy + y, r, g, b, a);
		drawPixel(screens, scale, cx - x, cy + y, r, g, b, a);
		drawPixel(screens, scale, cx + x, cy - y, r, g, b, a);
		drawPixel(screens, scale, cx - x, cy - y, r, g, b, a);
		drawPixel(screens, scale, cx + y, cy + x, r, g, b, a);
		drawPixel(screens, scale, cx - y, cy + x, r, g, b, a);
		drawPixel(screens, scale, cx + y, cy - x, r, g, b, a);
		drawPixel(screens, scale, cx - y, cy - x, r, g, b, a);
	}
}
