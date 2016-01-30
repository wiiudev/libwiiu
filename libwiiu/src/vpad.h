#ifndef VPAD_H
#define VPAD_H

#include "types.h"

#define BUTTON_A        0x8000
#define BUTTON_B        0x4000
#define BUTTON_X        0x2000
#define BUTTON_Y        0x1000
#define BUTTON_LEFT     0x0800
#define BUTTON_RIGHT    0x0400
#define BUTTON_UP       0x0200
#define BUTTON_DOWN     0x0100
#define BUTTON_ZL       0x0080
#define BUTTON_ZR       0x0040
#define BUTTON_L        0x0020
#define BUTTON_R        0x0010
#define BUTTON_PLUS     0x0008
#define BUTTON_MINUS    0x0004
#define BUTTON_HOME     0x0002
#define BUTTON_SYNC     0x0001
#define BUTTON_STICK_R  0x00020000
#define BUTTON_STICK_L  0x00040000
#define BUTTON_TV       0x00010000

typedef struct
{
    float x,y;
} Vec2D;

typedef struct
{
    uint16_t x, y;               /* Touch coordinates */
    uint16_t touched;            /* 1 = Touched, 0 = Not touched */
    uint16_t validity;           /* 0 = All valid, 1 = X invalid, 2 = Y invalid, 3 = Both invalid? */
} VPADTPData;
 
typedef struct
{
    uint32_t btn_hold;           /* Held buttons */
    uint32_t btn_trigger;        /* Buttons that are pressed at that instant */
    uint32_t btn_release;        /* Released buttons */
    Vec2D lstick, rstick;        /* Each contains 4-byte X and Y components */
    char unknown1c[0x52 - 0x1c]; /* Contains accelerometer and gyroscope data somewhere */
    VPADTPData tpdata;           /* Normal touchscreen data */
    VPADTPData tpdata1;          /* Modified touchscreen data 1 */
    VPADTPData tpdata2;          /* Modified touchscreen data 2 */
    char unknown6a[0xa0 - 0x6a];
    uint8_t volume;
    uint8_t battery;             /* 0 to 6 */
    uint8_t unk_volume;          /* One less than volume */
    char unknowna4[0xac - 0xa4];
} VPADData;

#endif /* VPAD_H */
