#ifndef DRAW_H
#define DRAW_H
#include "coreinit.h"
#include "types.h"

#define SCREEN_TV 1
#define SCREEN_GAMEPAD 2
#define SCREEN_ALL (SCREEN_TV | SCREEN_GAMEPAD)

//Function declarations for my graphics library
void flipBuffers();
void fillScreen(char r, char g, char b, char a);
void drawString(int x, int line, char * string);
void drawPixel(int x, int y, char r, char g, char b, char a);
void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawFillRect(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
void drawCircle(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawFillCircle(int xCen, int yCen, int radius, char r, char g, char b, char a);
void drawCircleCircum(int cx, int cy, int x, int y, char r, char g, char b, char a);
/**
  * Use this function to set the screens affected by the drawing functions.
  * @param screens SCREEN_TV, SCREEN_GAMEPAD or SCREEN_ALL
  */
void setDrawingScreens(int screens);
#endif /* DRAW_H */
