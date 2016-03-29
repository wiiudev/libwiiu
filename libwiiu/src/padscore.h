#ifndef PADSCORE_H
#define PADSCORE_H

#include "types.h"

#define WPAD_BUTTON_LEFT  0x0001
#define WPAD_BUTTON_RIGHT 0x0002
#define WPAD_BUTTON_DOWN  0x0004
#define WPAD_BUTTON_UP    0x0008
#define WPAD_BUTTON_PLUS  0x0010
#define WPAD_BUTTON_2     0x0100
#define WPAD_BUTTON_1     0x0200
#define WPAD_BUTTON_B     0x0400
#define WPAD_BUTTON_A     0x0800
#define WPAD_BUTTON_MINUS 0x1000
#define WPAD_BUTTON_Z     0x2000
#define WPAD_BUTTON_C     0x4000
#define WPAD_BUTTON_HOME  0x8000
 
#define WPAD_EXT_CORE          0
#define WPAD_EXT_NUNCHUK       1
#define WPAD_EXT_CLASSIC       2
#define WPAD_EXT_MPLUS         5
#define WPAD_EXT_MPLUS_NUNCHUK 6
#define WPAD_EXT_MPLUS_CLASSIC 7
 
#define WPAD_BATTERYLVL_CRIT   0
#define WPAD_BATTERYLVL_LOW    1
#define WPAD_BATTERYLVL_MEDIUM 2
#define WPAD_BATTERYLVL_HIGH   3
#define WPAD_BATTERYLVL_FULL   4
 
typedef struct {
    uint16_t x;    /* Resolution is 1024 */
    uint16_t y;    /* Resolution is 768 */
    uint16_t size;
    uint8_t  ID;
    uint8_t  pad;
} IRObject;
 
typedef struct {
    uint16_t   buttons; /* See WPAD_BUTTON */
    uint16_t   accelX;  /* Resolution is 1024 */
    uint16_t   accelY;  /* Resolution is 1024 */
    uint16_t   accelZ;  /* Resolution is 1024 */
    IRObject IR[4];   /* Max number of objects */
    uint8_t    ext;     /* Extension, see WPAD_EXT */
    uint8_t    err;     /* Error codes */
} WPADStatus; /* Normal Wiimote */
 
typedef struct {
    uint16_t   buttons;  /* See WPAD_BUTTON */
    uint16_t   accelX;   /* Resolution is 1024 */
    uint16_t   accelY;   /* Resolution is 1024 */
    uint16_t   accelZ;   /* Resolution is 1024 */
    IRObject IR[4];    /* Max number of objects */
    uint8_t    ext;      /* Extension, see WPAD_EXT */
    uint8_t    err;      /* Error codes */
    uint16_t   ncAccX;   /* Resolution is 1024 */
    uint16_t   ncAccY;   /* Resolution is 1024 */
    uint16_t   ncAccZ;   /* Resolution is 1024 */
    uint16_t   ncStickX; /* Resolution is 256 */
    uint16_t   ncStickY; /* Resolution is 256 */
} WPADNCStatus; /* Nunchuk extension */
 
typedef struct {
    uint16_t   buttons; /* See WPAD_BUTTON */
    uint16_t   accelX;  /* Resolution is 1024 */
    uint16_t   accelY;  /* Resolution is 1024 */
    uint16_t   accelZ;  /* Resolution is 1024 */
    IRObject IR[4];   /* Max number of objects */
    uint8_t    ext;     /* Extension, see WPAD_EXT */
    uint8_t    err;     /* Error codes */
    uint16_t   pad[6];  /* Padding, Classic/Nunchuk? */
    uint8_t    status;  /* WiiMotionPlus */
    uint8_t    reserved;
    uint16_t   pitch;   /* WiiMotionPlus */
    uint16_t   yaw;     /* WiiMotionPlus */
    uint16_t   roll;    /* WiiMotionPlus */
} WPADMPStatus; /* WiiMotionPlus */
 
typedef struct {
    uint16_t WPADData[20]; /* Default WPAD data isn't used */
    uint8_t  ext;          /* Extension, see WPAD_EXT */
    uint8_t  err;          /* Error codes */
    uint16_t press[4];     /* 4 Pads for sensing balance */
    uint8_t  temp;         /* Temperature */
    uint8_t  battery;      /* Battery */
} WPADBLStatus; /* Balance Board */

#endif