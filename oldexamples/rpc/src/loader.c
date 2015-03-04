#include "loader.h"

/* Exception callback */
char ex_cb(void *context);

/* Start of our code */
void _start()
{
  /* Load a good stack */
  asm(
      "lis %r1, 0x1ab5 ;"
      "ori %r1, %r1, 0xd138 ;"
  );
 
  /* Get a handle to coreinit.rpl and nsysnet.rpl */
  unsigned int coreinit_handle, nsysnet_handle;
  OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
  OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);

  /* OS exception functions */
  char (*OSSetExceptionCallback)(unsigned char ex, char (*callback)(void*));

  /* Socket functions */
  int (*socket)(int family, int type, int proto);
  int (*connect)(int fd, struct sockaddr *addr, int addrlen);
  int (*recv)(int fd, void *buffer, int len, int flags);
  int (*send)(int fd, const void *buffer, int len, int flags);

  /* Read the address of OSSetExceptionCallback() */
  OSDynLoad_FindExport(coreinit_handle, 0, "OSSetExceptionCallback", &OSSetExceptionCallback);

  /* Read the addresses of socket(), connect(), and send() */
  OSDynLoad_FindExport(nsysnet_handle, 0, "socket", &socket);
  OSDynLoad_FindExport(nsysnet_handle, 0, "connect", &connect);
  OSDynLoad_FindExport(nsysnet_handle, 0, "recv", &recv);
  OSDynLoad_FindExport(nsysnet_handle, 0, "send", &send);

  /* Set up DSI, ISI, and program exception callbacks */
  OSSetExceptionCallback(2, &ex_cb);
  OSSetExceptionCallback(3, &ex_cb);
  OSSetExceptionCallback(6, &ex_cb);

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
  int rpc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int status = connect(rpc, &sin, 0x10);
  if (status) OSFatal("Error connecting to RPC server");

  /* Block waiting for data */
  unsigned int buffer[128];
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
	  unsigned int *src = (unsigned int*) buffer[1];
	  unsigned int *dest = &buffer[1];
	  unsigned int num_words = buffer[2];

	  for (i = 0; i < num_words; i++)
	  {
	    dest[i] = src[i];
	  }

	  /* Send the buffer back */
	  send(rpc, buffer, 512, 0);

	  break;
	/* Write memory */
	case 1: ;
	  /* Copy them into the buffer */
	  src = &buffer[3];
	  dest = (unsigned int*) buffer[1];
	  num_words = buffer[2];

	  for (i = 0; i < num_words; i++)
	  {
	    dest[i] = src[i];
	  }

	  break;
	/* Get symbol */
	case 2: ;
	  /* Identify the RPL name and symbol name */
	  char *rplname = (char*) &buffer[3];
	  char *symname = (char*) ((unsigned char*)&buffer[0]) + buffer[2];

	  /* Get the symbol and store it in the buffer */
	  unsigned int module_handle, function_address;
	  OSDynLoad_Acquire(rplname, &module_handle);
	  OSDynLoad_FindExport(module_handle, 0, symname, &function_address);
	  buffer[1] = function_address;

	  /* Send the buffer back */
      send(rpc, buffer, 512, 0);

	  break;
    /* Call function */
	case 3: ;
	  /* Create a function pointer */
	  unsigned int (*function)(unsigned int r3, unsigned int r4, unsigned int r5, unsigned int r6, unsigned int r7);
	  function = (void*)buffer[1];

	  /* Call it and return the results */
	  buffer[1] = function(buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
	  send(rpc, buffer, 512, 0);

	  break;
	default:
	  OSFatal("Invalid RPC command");
	}
  }
}

/* Exception callback */
char ex_cb(void *context)
{
  /* Get a handle to coreinit.rpl */
  unsigned int coreinit_handle;
  OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

  /* Read the address of _Exit and call it */
  void (*__PPCExit)();
  OSDynLoad_FindExport(coreinit_handle, 0, "__PPCExit", &__PPCExit);
  __PPCExit();
}
