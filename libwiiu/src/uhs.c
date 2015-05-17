#include "coreinit.h"
#include "uhs.h"

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

/* Submit a bulk request to an endpoint */
int UhsSubmitBulkRequest(int uhs_handle, uint32_t if_handle, int endpoint, int direction, void *buffer, int length, int timeout)
{
	/* Symbol loading */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	int (*IOS_Ioctlv)(int fd, int request, int cnt_in, int cnt_out, struct iovec *vecbuf);
	OSDynLoad_FindExport(coreinit_handle, false, "IOS_Ioctvl", &IOS_Ioctlv);
}
