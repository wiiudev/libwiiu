#define OSFatal ((void (*)(char* msg))0x1031368)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x102f09c)
#define rop ((void (*)())0xCAFECAFE)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x1035a6c)

#define BUFFER_ADDR 0x1b201b20
#define CODE_START	0xCAFECAFE
#define OSPANIC_CB	0x1003AAAC

void start()
{
	/* Set up a different stack */
    asm("stwu %r1,-0x2000(%r1)");

	/* Remove the OSPanic() callback */
	unsigned int *ospanic_cb = (unsigned int*)OSPANIC_CB;
	ospanic_cb[0] = ospanic_cb[1] = 0;

	/* Test printing */
	OSFatal("Hello");
    /*unsigned int * start = (unsigned int*)BUFFER_ADDR;
    int i;
  
    for (i = 0; i < 150; start += 1) {
        if (*start == CODE_START) i++;
    }
    
    memcpy((void *)BUFFER_ADDR, (void *)BUFFER_ADDR + 0x800, 0x600);
    
    unsigned int * code_addr = (unsigned int *)(BUFFER_ADDR + 0x94);
    //unsigned int * jump_addr = (unsigned int *)(BUFFER_ADDR + 0x19C);
    //unsigned int * jump_memcpy = (unsigned int *)(BUFFER_ADDR + 0x540);
    *code_addr = (int)start;
    //*jump_memcpy = 0;
    //*jump_addr = 0x0102D01C;

    asm(
        "lis %r1, 0x1dd7 ;"
        "ori %r1, %r1, 0xb814 ;"
    );
    rop();*/
}