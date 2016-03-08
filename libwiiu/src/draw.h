#ifndef DRAW_H
#define DRAW_H

#include "types.h"

void flipBuffersEx(bool tv, bool pad);
void drawStringEx(bool tv, bool pad, int x, int y, char * string);
void fillScreenEx(bool tv, bool pad, char r, char g, char b, char a);
void drawPixelEx(bool tv, bool pad, int x, int y, char r, char g, char b, char a);
void drawLineEx(bool tv, bool pad, int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRectEx(bool tv, bool pad, int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRectEx(bool tv, bool pad, int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawCircleEx(bool tv, bool pad, int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCircleEx(bool tv, bool pad, int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCircleCircumEx(bool tv, bool pad, int cx, int cy, int x, int y, char r, char g, char b, char a);

/* THE FOLLOWING FUNCTIONS ARE PROVIDED SOLELY FOR BACKWARDS COMPATIBILITY WITH OLDER VERSIONS OF LIBWIIU.
 * IF YOU PREFER TO CALL THE ABOVE EX FUNCTIONS DIRECTLY, REMOVE THE BELOW FUNCTIONS AS THEY ARE UNNEEDED BLOAT FOR YOUR PROGRAM. */
void flipBuffers();
void drawString(int x, int y, char * string);
void fillScreen(char r, char g, char b, char a);
void drawPixel(int x, int y, char r, char g, char b, char a);
void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawCircle(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCircle(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCircleCircum(int cx, int cy, int x, int y, char r, char g, char b, char a);

#endif