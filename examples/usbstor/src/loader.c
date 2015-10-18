#include "loader.h"

/* Acquisition callback */
int callback(void *context, UhsInterfaceProfile *profile);

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

    /* Register a class driver for all devices */
    UhsInterfaceFilter filter = { MATCH_ANY };
    UhsClassDrvReg(uhs_handle, &filter, (void*)uhs_handle, &callback);

    while(1);

    /* Get the interfaces plugged into each port */
    UhsInterfaceProfile profiles[4];
    //UhsInterfaceFilter filter = { MATCH_ANY };
    int num_ifs = UhsQueryInterfaces(uhs_handle, &filter, &profiles[0], 4);
    if (num_ifs <= 0) OSFatal("No USB interfaces detected");

    /* Find an audio interface */
    UhsInterfaceProfile *iface_stor = NULL;
    for (int i = 0; i < 4; i++)
    {
        if (profiles[i].if_desc.bInterfaceClass == USBCLASS_AUDIO)
        {
            iface_stor = &profiles[i];
            break;
        }
    }
    if (!iface_stor) OSFatal("No audio interfaces detected");

    /* Administer the device */
    int ret = UhsAdministerDevice(uhs_handle, iface_stor->if_handle, 2, 0);

    /* Reset the storage interface */
    //int ret = UhsSubmitControlRequest(uhs_handle, iface_stor->if_handle, NULL, 0xFF, 0x21, 0, (uint16_t)iface_stor->if_desc.bInterfaceNumber, 0, TIMEOUT_NONE);

    char buffer[256];
    __os_snprintf(buffer, 256, "Administer returned 0x%08X", ret);
    OSFatal(buffer);

    /* Infinite loop */
    while(1);
}

/* Class driver callback */
int callback(void *context, UhsInterfaceProfile *profile)
{
    OSFatal("In the callback");
}
