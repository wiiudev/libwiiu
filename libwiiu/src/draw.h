#ifndef DRAW_H
#define DRAW_H
#include "types.h"
//Function declarations for my graphics library
void flipBuffers();
void flipBuffersTv();
void flipBuffersPad();
void drawString(int x, int y, char * string);
void drawStringTv(int x, int y, char * string);
void drawStringPad(int x, int y, char * string);
void fillScreen(char r, char g, char b, char a);
void fillScreenTv(char r, char g, char b, char a);
void fillScreenPad(char r, char g, char b, char a);
void drawPixel(int x, int y, char r, char g, char b, char a);
void drawPixelTv(int x, int y, char r, char g, char b, char a);
void drawPixelPad(int x, int y, char r, char g, char b, char a);
void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawLineTv(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawLinePad(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRectTv(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRectPad(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRectTv(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRectPad(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawCircle(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCircleTv(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCirclePad(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCircle(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCircleTv(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCirclePad(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCircleCircum(int cx, int cy, int x, int y, char r, char g, char b, char a);
void drawCircleCircumTv(int cx, int cy, int x, int y, char r, char g, char b, char a);
void drawCircleCircumPad(int cx, int cy, int x, int y, char r, char g, char b, char a);
#endif /* DRAW_H */