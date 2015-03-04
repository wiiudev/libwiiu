#ifndef PROGRAM_H
#define PROGRAM_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/vpad.h"
#include "../../../libwiiu/src/types.h"
//Using modified version of draw to render at twice the scale to improve framerate
#include "draw.h"
#include "../../../libwiiu/src/loader.h"

#include "pong.h"

void _entryPoint();

#endif /* PROGRAM_H */