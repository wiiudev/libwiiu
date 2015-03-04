#ifndef VPAD_H
#define VPAD_H

#define KEY_LEFT	0x0800
#define KEY_RIGHT	0x0400
#define KEY_UP		0x0200
#define KEY_DOWN	0x0100
#define KEY_A		0x8000
#define KEY_B		0x4000
#define KEY_X		0x2000
#define KEY_Y		0x1000
#define KEY_ZR		0x0040
#define KEY_ZL		0x0080
#define KEY_R		0x0010
#define KEY_L		0x0020
#define KEY_PLUS	0x0008
#define KEY_MINUS	0x0004
#define KEY_SYNC	0x0001
#define KEY_POWER	0x0002
#define KEY_HOME	0x0002

typedef struct Vec
{
	unsigned int x;
} Vec;

typedef struct Vec2
{
	unsigned int x, y;
} Vec2;

typedef struct VPADDir
{
	Vec X;
	Vec Y;
	Vec Z;
} VPADDir;

typedef struct VPADTPData
{
	unsigned short x, y;
	unsigned short touch;
	unsigned short validity;
} VPADTPData;

typedef struct VPADStatus
{
	unsigned int hold;
	unsigned int press;
	unsigned int release;

	Vec2 left_stick;
	Vec2 right_stick;

	Vec acceleration;
	float acceleration_value;
	float acceleration_speed;

	Vec angular_velocity;
	Vec rotational_velocity;

	char error;

	VPADTPData touch_data;
	VPADTPData drawing_touch_data;
	VPADTPData moving_touch_data;

	VPADDir direction;

	char headphones;

	Vec magnet;
	unsigned char volume;
	unsigned char battery;
	unsigned char mic;
	unsigned short padding[8];
} VPADStatus;
#endif /* VPAD_H */