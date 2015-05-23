#ifndef UHS_H
#define UHS_H

#include "types.h"
#include "uhs_usbspec.h"

/* Determines which interface parameters to check */
#define MATCH_ANY				0x000
#define MATCH_DEV_VID			0x001
#define MATCH_DEV_PID			0x002
#define MATCH_DEV_CLASS			0x010
#define MATCH_DEV_SUBCLASS		0x020
#define MATCH_DEV_PROTOCOL		0x040
#define MATCH_IF_CLASS			0x080
#define MATCH_IF_SUBCLASS		0x100
#define MATCH_IF_PROTOCOL		0x200

/* Endpoint transfer directions */
#define ENDPOINT_TRANSFER_OUT	1
#define ENDPOINT_TRANSFER_IN	2
 
/* Interface filter */
typedef struct
{
	uint16_t match_params;		/* Bitmask of above flags */
	uint16_t vid, pid;			/* Vendor ID and product ID */
	char unknown6[0xa - 0x6];
	uint8_t dev_class;			/* Device class */
	uint8_t dev_subclass;		/* Device subclass */
	uint8_t dev_protocol;		/* Device protocol */
	uint8_t if_class;			/* Interface class */
	uint8_t if_subclass;		/* Interface subclass */
	uint8_t if_protocol;		/* Interface protocol */
} __attribute__((packed)) UhsInterfaceFilter;

/* Interface profile */
typedef struct
{
    uint32_t if_handle;
    char unknown4[0x28 - 0x4];
    UhsDeviceDescriptor dev_desc;
    UhsConfigDescriptor cfg_desc;
    UhsInterfaceDescriptor if_desc;
    char in_endpoints[0xdc - 0x4c];
	char out_endpoints[0x16c - 0xdc];
} __attribute__((packed)) UhsInterfaceProfile;

/* Open a specific controller under /dev/uhs */
int UhsOpenController(int controller_num);

/* Determine which USB device interfaces are plugged in and available */
int UhsQueryInterfaces(int uhs_handle, UhsInterfaceFilter *filter, UhsInterfaceProfile *profiles, int max_profiles);

/* Submit a bulk request to an endpoint */
int UhsSubmitBulkRequest(int uhs_handle, uint32_t if_handle, uint8_t endpoint, int direction, void *buffer, int length, int timeout);

#endif