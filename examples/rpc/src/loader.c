#include "loader.h"

/* Start a thread for RPC */
void _start()
{
	/* Load a good stack */
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
	);
 
	/* Get a handle to coreinit.rpl */
	uint32_t coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

	/* Memory allocation functions */
	void* (*OSAllocFromSystem)(uint32_t size, int align);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);

	/* OS thread functions */
	bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t *stack, uint32_t stack_size, int priority, uint16_t attr);
	int (*OSResumeThread)(void *thread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);

	/* Make a thread to run the FS functions */
	OSContext *thread = (OSContext*)OSAllocFromSystem(0x1000, 8);
	uint32_t *stack = (uint32_t*)OSAllocFromSystem(0x1000, 0x20);
	if (!OSCreateThread(thread, &rpc, 0, NULL, stack + 0x400, 0x1000, 0, 0x2 | 0x8)) OSFatal("Failed to create thread");
	OSResumeThread(thread);

	/* Infinite loop */
	while(1);
}

/* RPC thread */
void rpc()
{
	/* Get a handle to nsysnet.rpl */
	uint32_t nsysnet_handle;
	OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);

	/* Socket functions */
	int (*socket)(int family, int type, int proto);
	int (*connect)(int fd, struct sockaddr *addr, int addrlen);
	int (*recv)(int fd, void *buffer, int len, int flags);
	int (*send)(int fd, const void *buffer, int len, int flags);
	OSDynLoad_FindExport(nsysnet_handle, 0, "socket", &socket);
	OSDynLoad_FindExport(nsysnet_handle, 0, "connect", &connect);
	OSDynLoad_FindExport(nsysnet_handle, 0, "recv", &recv);
	OSDynLoad_FindExport(nsysnet_handle, 0, "send", &send);

	/* Set up our socket address structure */
	struct sockaddr sin;
	sin.sin_family = AF_INET;
	sin.sin_port = 12345;
	sin.sin_addr.s_addr = PC_IP;
	for (int i = 0; i < 8; i++) sin.sin_zero[i] = 0;

	/* Connect to the PC */
	int rpc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int status = connect(rpc, &sin, 0x10);
	if (status) OSFatal("Error connecting to RPC server");

	/* Block waiting for data */
	uint32_t buffer[128];
	while(1)
	{
		/* Get data from the socket */
		int bytes_read = recv(rpc, buffer, 512, 0);

		/* Look at the command byte */
		switch (buffer[0])
		{
		/* Read memory */
		case 0: ;
			/* Copy them into the buffer */
			uint32_t *src = (uint32_t*) buffer[1];
			uint32_t *dest = &buffer[1];
			uint32_t num_words = buffer[2];

			for (int i = 0; i < num_words; i++) dest[i] = src[i];

			/* Send the buffer back */
			send(rpc, buffer, 512, 0);

			break;
		/* Write memory */
		case 1: ;
			/* Copy them into the buffer */
			src = &buffer[3];
			dest = (uint32_t*) buffer[1];
			num_words = buffer[2];

			for (int i = 0; i < num_words; i++) dest[i] = src[i];

			break;
		/* Get symbol */
		case 2: ;
			/* Identify the RPL name and symbol name */
			char *rplname = (char*) &buffer[3];
			char *symname = (char*) ((unsigned char*)&buffer[0]) + buffer[2];

			/* Get the symbol and store it in the buffer */
			uint32_t module_handle, function_address;
			OSDynLoad_Acquire(rplname, &module_handle);
			OSDynLoad_FindExport(module_handle, 0, symname, &function_address);
			buffer[1] = function_address;

			/* Send the buffer back */
			send(rpc, buffer, 512, 0);

			break;
		/* Call function */
		case 3: ;
			/* Create a function pointer */
			uint32_t (*function)(uint32_t r3, uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r7);
			function = (uint32_t (*)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t))buffer[1];

			/* Call it and return the results */
			buffer[1] = function(buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
			send(rpc, buffer, 512, 0);

			break;
		default:
			OSFatal("Invalid RPC command");
		}
	}
}
