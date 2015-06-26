#ifndef PROGRAM_H
#define PROGRAM_H
#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/vpad.h"
#include "../../../libwiiu/src/types.h"
#include "../../../libwiiu/src/draw.h"

struct renderFlags {
	int a;
	int b;
	int x;
	int y;
	char aPressed[32];
	char bPressed[32];
	char xPressed[32];
	char yPressed[32];
};



void _entryPoint();
void render(struct renderFlags *flags);

#endif /* PROGRAM_H */