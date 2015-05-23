#include "loader.h"

/* Start of our code */
void _start()
{
	/* Load a good stack */
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
	);

	/* Open the first USB controller */
	int uhs_handle = UhsOpenController(0);

	/* Get the interfaces plugged into each port */
	UhsInterfaceProfile profiles[4];
	UhsInterfaceFilter filter = { MATCH_ANY };
	int num_ifs = UhsQueryInterfaces(uhs_handle, &filter, &profiles[0], 4);
	if (num_ifs <= 0) OSFatal("No USB interfaces detected");

	/* Find a mass storage interface */
	UhsInterfaceProfile *iface_stor = NULL;
	for (int i = 0; i < 4; i++)
	{
		if (profiles[i].if_desc.bInterfaceClass == USBCLASS_STORAGE)
		{
			iface_stor = &profiles[i];
			break;
		}
	}
	if (!iface_stor) OSFatal("No storage interfaces detected");

	/* Infinite loop */
	while(1);
}
