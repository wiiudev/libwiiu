#ifndef PROGRAM_H
#define PROGRAM_H
#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/vpad.h"
#include "../../../libwiiu/src/types.h"
#include "../../../libwiiu/src/draw.h"

struct renderFlags{
int y;
int x;
int a;
int b;
char output[1000];
};



void _entryPoint();
void render(struct renderFlags *flags);

#endif /* PROGRAM_H */