#include "loader.h"

void _start()
{
	/* Get the framebuffer of the TV or DRC */
	int dev = 0;
	unsigned int *fb = *((unsigned int*)(0x1032AEC0 + 0x304 + (dev * 0x20)));

	char buf[256];
	__os_snprintf(buf, 256, "0x%08X", fb);
	OSFatal(buf);
}
