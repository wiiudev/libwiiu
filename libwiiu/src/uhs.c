#include "coreinit.h"
#include "uhs.h"

/* Callback for UHS event wait */
static void uhs_event_cb(int status, void *context)
{
	OSFatal("Event");
}

/* Wait asynchronously for a UHS event */
static int wait_uhs_event(int uhs_handle)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_IoctlAsync)(int fd, int request, void *inbuf, int inlen, void *outbuf, int outlen, void (*cb)(int,void*), void *context);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_IoctlAsync", &IOS_IoctlAsync);

	/* Allocate and fill in the request buffers */
	uint32_t reqbuf[8];
	reqbuf[0] = (uint32_t)-1;
	reqbuf[1] = 0;

	/* Perform the ioctl() request */
	uint32_t outbuf[0x180/4];
	return IOS_IoctlAsync(uhs_handle, 0x03, reqbuf, 8, outbuf, 0x180, &uhs_event_cb, (void*)uhs_handle);
}

/* Open a specific controller under /dev/uhs */
int UhsOpenController(int controller_num)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_Open)(char *path, int mode);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Open", &IOS_Open);

	/* Build the path name and return a handle */
	char path[32];
	__os_snprintf(path, 32, "/dev/uhs/%d", controller_num);
	return IOS_Open(path, 0);
}

/* Register a USB class driver */
int UhsClassDrvReg(int uhs_handle, UhsInterfaceFilter *filter, void *context, int (*cb)(void *ctx, UhsInterfaceProfile *profile))
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	void* (*memcpy)(void *dest, void *src, uint32_t length);
	int (*IOS_Ioctl)(int fd, int request, void *inbuf, int inlen, void *outbuf, int outlen);
	OSDynLoad_FindExport(coreinit_handle, false, "memcpy", &memcpy);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctl", &IOS_Ioctl);

	/* Allocate and fill in the request buffer */
	uint32_t reqbuf[6];
	reqbuf[0] = (uint32_t)context;
	reqbuf[1] = (uint32_t)cb;
	memcpy(&reqbuf[2], filter, sizeof(UhsInterfaceFilter));

	/* Perform the ioctl() request */
	int ret = -1;
	IOS_Ioctl(uhs_handle, 0x01, reqbuf, 0x18, &ret, 4);

	/* Set up the callback to be executed */
	wait_uhs_event(uhs_handle);

	return ret;
}

/* Determine which USB device interfaces are plugged in and available */
int UhsQueryInterfaces(int uhs_handle, UhsInterfaceFilter *filter, UhsInterfaceProfile *profiles, int max_profiles)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_Ioctl)(int fd, int request, void *inbuf, int inlen, void *outbuf, int outlen);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctl", &IOS_Ioctl);

	/* Perform the ioctl() request */
	return IOS_Ioctl(uhs_handle, 0x11, filter, 0x10, profiles, max_profiles * 0x16c);
}

/* Acquire a USB device interface for use */
int UhsAcquireInterface(int uhs_handle, uint32_t if_handle, void *unk1, int (*callback)(int arg0, int arg1, int arg2))
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_Ioctl)(int fd, int request, void *inbuf, int inlen, void *outbuf, int outlen);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctl", &IOS_Ioctl);

	/* Allocate and fill in the request buffer */
	uint32_t reqbuf[3];
	reqbuf[0] = if_handle;
	reqbuf[1] = (uint32_t)unk1;
	reqbuf[2] = (uint32_t)callback;

	/* Perform the ioctl() request */
	return IOS_Ioctl(uhs_handle, 0x4, &reqbuf[0], 0xc, 0, 0);
}

/* Release a currently-held USB device interface */
int UhsReleaseInterface(int uhs_handle, uint32_t if_handle, bool no_reacquire)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_Ioctl)(int fd, int request, void *inbuf, int inlen, void *outbuf, int outlen);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctl", &IOS_Ioctl);

	/* Allocate and fill in the request buffer */
	uint32_t reqbuf[3];
	reqbuf[0] = if_handle;
	reqbuf[1] = (uint32_t)no_reacquire;
	reqbuf[2] = 0;

	/* Perform the ioctl() request */
	return IOS_Ioctl(uhs_handle, 0x5, &reqbuf[0], 0xc, 0, 0);
}

/* Administer a USB device */
int UhsAdministerDevice(int uhs_handle, uint32_t if_handle, int arg2, int arg3)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_Ioctl)(int fd, int request, void *inbuf, int inlen, void *outbuf, int outlen);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctl", &IOS_Ioctl);

	/* Allocate and fill in the request buffer */
	uint32_t reqbuf[3];
	reqbuf[0] = (uint32_t)arg2;
	reqbuf[1] = if_handle;
	reqbuf[2] = (uint32_t)arg3;

	/* Perform the ioctl() request */
	return IOS_Ioctl(uhs_handle, 0x12, &reqbuf[0], 0xc, 0, 0);
}

/* Submit a control request to endpoint 0 */
int UhsSubmitControlRequest(int uhs_handle, uint32_t if_handle, void *buffer, uint8_t bRequest, uint8_t bmRequestType, uint16_t wValue, uint16_t wIndex, uint16_t wLength, int timeout)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	void* (*memset)(void *buffer, int value, unsigned length);
	int (*IOS_Ioctlv)(int fd, int request, int cnt_in, int cnt_out, struct iovec *vecbuf);
	OSDynLoad_FindExport(coreinit_handle, false, "memset", &memset);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctlv", &IOS_Ioctlv);

	/* Allocate and fill in the request buffer */
	uint8_t reqbuf[0xa1];
	memset(&reqbuf[0], 0, sizeof(reqbuf));
	*((uint32_t*)&reqbuf[0x00]) = if_handle;
	reqbuf[0x04] = 0;						/* Endpoint 0 */
	*((uint32_t*)&reqbuf[0x05]) = timeout;
	*((uint32_t*)&reqbuf[0x09]) = 1;		/* Transfer type */
	reqbuf[0x0d] = bRequest;
	reqbuf[0x0e] = bmRequestType;
	*((uint16_t*)&reqbuf[0x0f]) = wValue;
	*((uint16_t*)&reqbuf[0x11]) = wIndex;
	*((uint16_t*)&reqbuf[0x13]) = wLength;

	/* Allocate and fill in the I/O vectors */
	struct iovec vecbuf[2];
	memset(&vecbuf[0], 0, sizeof(struct iovec) * 2);
	vecbuf[0].buffer = &reqbuf[0];
	vecbuf[0].len = sizeof(reqbuf);
	vecbuf[0].unknown8[3] = 1;
	vecbuf[1].buffer = buffer;
	vecbuf[1].len = (int)wLength;
	vecbuf[1].unknown8[3] = 1;

	/* Perform the ioctlv() request */
	if (bmRequestType & (1 << 7)) return IOS_Ioctlv(uhs_handle, 0xc, 1, 1, &vecbuf[0]);
	else return IOS_Ioctlv(uhs_handle, 0xc, 0, 2, &vecbuf[0]);
}

/* Submit a bulk request to an endpoint */
int UhsSubmitBulkRequest(int uhs_handle, uint32_t if_handle, uint8_t endpoint, int direction, void *buffer, int length, int timeout)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	void* (*memset)(void *buffer, int value, unsigned length);
	int (*IOS_Ioctlv)(int fd, int request, int cnt_in, int cnt_out, struct iovec *vecbuf);
	OSDynLoad_FindExport(coreinit_handle, false, "memset", &memset);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctlv", &IOS_Ioctlv);

	/* Allocate and fill in the request buffer */
	uint8_t reqbuf[0xa1];
	memset(&reqbuf[0], 0, sizeof(reqbuf));
	*((uint32_t*)&reqbuf[0x00]) = if_handle;
	reqbuf[0x04] = endpoint;
	*((uint32_t*)&reqbuf[0x05]) = timeout;
	*((uint32_t*)&reqbuf[0x09]) = 3;
	*((uint32_t*)&reqbuf[0x0d]) = direction;
	*((uint32_t*)&reqbuf[0x11]) = length;

	/* Allocate and fill in the I/O vectors */
	struct iovec vecbuf[2];
	memset(&vecbuf[0], 0, sizeof(struct iovec) * 2);
	vecbuf[0].buffer = &reqbuf[0];
	vecbuf[0].len = sizeof(reqbuf);
	vecbuf[1].buffer = buffer;
	vecbuf[1].len = length;

	/* Perform the ioctlv() request */
	if (direction == ENDPOINT_TRANSFER_OUT) return IOS_Ioctlv(uhs_handle, 0xe, 0, 2, &vecbuf[0]);
	else if (direction == ENDPOINT_TRANSFER_IN) return IOS_Ioctlv(uhs_handle, 0xe, 1, 1, &vecbuf[0]);
	else return -1;
}
