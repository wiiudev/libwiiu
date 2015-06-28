#ifndef DRAW_H
#define DRAW_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/types.h"

#define SCREEN_TV 1
#define SCREEN_PAD 2
#define SCREEN_ALL (SCREEN_TV | SCREEN_PAD)

//Function declarations for my graphics library
void flipBuffers(int screens);
void fillScreen(int screens, char r, char g, char b, char a);
void drawString(int sceens, int x, int line, char* string);
void drawPixel(int screens, int scale, int x, int y, char r, char g, char b, char a);
void drawLine(int screens, int scale, int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRect(int screens, int scale, int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRect(int screens, int scale, int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawCircle(int screens, int scale, int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCircle(int screens, int scale, int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCircleCircum(int screens, int scale, int cx, int cy, int x, int y, char r, char g, char b, char a);
#endif /* DRAW_H */
