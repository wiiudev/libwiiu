#include "loader.h"

/* Install an exception handler */
int _start(int argc, char **argv)
{
	/* Get a handle to coreinit.rpl and nsysnet.rpl */
	unsigned int coreinit_handle, nsysnet_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);

	/* Memory and cache functions */
	unsigned int (*OSEffectiveToPhysical)(void *ptr);
	void (*DCFlushRange)(void *addr, unsigned int len);
	void (*DCInvalidateRange)(void *addr, unsigned int len);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSEffectiveToPhysical", &OSEffectiveToPhysical);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCInvalidateRange", &DCInvalidateRange);

	/* Kernel is mapped */
	if (OSEffectiveToPhysical((void*)0xA0000000) == 0xC0000000)
	{
		/* Patches installed */
		if (*((unsigned int*)(0xA0000000 + (0xFFF00B40 - 0xC0000000))) == 1)
		{
			/* Socket functions */
			int (*get_socket_rm_fd)();
			int (*socket)(int family, int type, int proto);
			int (*connect)(int fd, struct sockaddr *addr, int addrlen);
			OSDynLoad_FindExport(nsysnet_handle, 0, "get_socket_rm_fd", &get_socket_rm_fd);
			OSDynLoad_FindExport(nsysnet_handle, 0, "socket", &socket);
			OSDynLoad_FindExport(nsysnet_handle, 0, "connect", &connect);

			/* Set up our socket address structure */
			struct sockaddr sin;
			sin.sin_family = AF_INET;
			sin.sin_port = 12345;
			sin.sin_addr.s_addr = PC_IP;
			int i;
			for (i = 0; i < 8; i++)
			{
				sin.sin_zero[i] = 0;
			}

			/* Connect to the PC */
			int pc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int status = connect(pc, &sin, 0x10);
			if (status) OSFatal("Error connecting to PC");

			/* Store the socket info in a place the kernel can access */
			unsigned int sock_info[4] = {1, (unsigned int)get_socket_rm_fd(), (unsigned int)pc, 0};
			memcpy((void*)(0xA0000000 + (0xFFF00B40 - 0xC0000000)), sock_info, 0x10);
			DCFlushRange((void*)(0xA0000000 + (0xFFF00B40 - 0xC0000000)), 0x20);
			DCInvalidateRange((void*)(0xA0000000 + (0xFFF00B40 - 0xC0000000)), 0x20);
		}
		/* Patches not yet installed, but they will be next time */
		else
		{
			/* Signify patches installed, socket data still invalid, spinlock 0 */
			*((unsigned int*)(0xA0000000 + (0xFFF00B40 - 0xC0000000))) = 1;
			*((unsigned int*)(0xA0000000 + (0xFFF00B44 - 0xC0000000))) = -1;
			*((unsigned int*)(0xA0000000 + (0xFFF00B48 - 0xC0000000))) = -1;
			*((unsigned int*)(0xA0000000 + (0xFFF00B4C - 0xC0000000))) = 0;
			DCFlushRange((void*)(0xA0000000 + (0xFFF00B40 - 0xC0000000)), 0x20);
			DCInvalidateRange((void*)(0xA0000000 + (0xFFF00B40 - 0xC0000000)), 0x20);
		}
	}

	/* Return to main() */
#if VER == 532
	int (*main)(int argc, char **argv) = (int (*)(int,char**)) (*((unsigned int*)0x1005D180));
#elif VER == 550
	int (*main)(int argc, char **argv) = (int (*)(int,char**)) (*((unsigned int*)0x1005E040));
#else
	int (*main)(int argc, char **argv) = (int (*)(int,char**)) (*((unsigned int*)0x0));
#endif
	return main(argc, argv);
}

/* memcpy() implementation */
void *memcpy(void* dst, const void* src, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
		((unsigned char*) dst)[i] = ((const unsigned char*) src)[i];
	return dst;
}
