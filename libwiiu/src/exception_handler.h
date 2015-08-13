#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H
#include "coreinit.h"
void exception_disassembly_helper(char *fmt, int addr,int opcode, char* s)
{
    char* *store = (char*)0x1ab5d140;
    char *buffer = (char *)store[0];
    store[0] += __os_snprintf(buffer,512,fmt, addr,opcode,s);
}
/* Exception callback */
char exception_handler(int *context)
{
  /* Get a handle to coreinit.rpl */
  unsigned int coreinit_handle;
  OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
  void (*DisassemblePPCRange)(void *start, void *end, void *printf_func, int *find_symbol_func, int flags);
  OSDynLoad_FindExport(coreinit_handle, 0, "DisassemblePPCRange", &DisassemblePPCRange);  
  char buffer2[512];
  int *store = (int *)0x1ab5d140;
  store[0] = (int)buffer2;
  char buffer[1024];
  DisassemblePPCRange(context[38]-0x10,context[38]+0x10,exception_disassembly_helper,0,0);
  __os_snprintf(buffer, 1024, 
  "r0 :%08X r1 :%08X r2 :%08X r3 :%08X r4 :%08X \nr5 :%08X r6 :%08X r7 :%08X r8 :%08X r9 :%08X \nr10:%08X r11:%08X r12:%08X r13:%08X r14:%08X \nr15:%08X r16:%08X r17:%08X r18:%08X r19:%08X \nr20:%08X r21:%08X r22:%08X r23:%08X r24:%08X \nr25:%08X r26:%08X r27:%08X r28:%08X r29:%08X \nr30:%08X r31:%08X \nCR :%08X LR :%08X CTR:%08X XER:%08X\nSSR0:%08X SSR1:%08X EX0:%08X EX1:%08X\n%s",
    context[2],
    context[3],
    context[4],
    context[5],
    context[6],
    context[7],
    context[8],
    context[9],
    context[10],
    context[11],
    context[12],
    context[13],
    context[14],
    context[15],
    context[16],
    context[17],
    context[18],
    context[19],
    context[20],
    context[21],
    context[22],
    context[23],
    context[24],
    context[25],
    context[26],
    context[27],
    context[28],
    context[29],
    context[30],
    context[31],
    context[32],
    context[33],
    context[34],
    context[35],
    context[36],
    context[37],
    context[38],
    context[39],
    context[40],
    context[41],
    buffer2
    );
  
    if(true)
    {
    
        OSFatal(buffer);
    }
    else
    {

        
    }
   
  return 0;
}
void install_exception_handler_global()
{

  unsigned int coreinit_handle = 0;
  OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
  char (*OSSetExceptionCallbackEx)(unsigned char t,unsigned char et, char (*callback)(int*));
  OSDynLoad_FindExport(coreinit_handle, 0, "OSSetExceptionCallbackEx", &OSSetExceptionCallbackEx);
  OSSetExceptionCallbackEx(4,2, &exception_handler);
  OSSetExceptionCallbackEx(4,3, &exception_handler);
  OSSetExceptionCallbackEx(4,6, &exception_handler);
  
}
void install_exception_handler()
{
  unsigned int coreinit_handle = 0;
  OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
  /* OS exception functions */
  char (*OSSetExceptionCallback)(unsigned char et, char (*callback)(int*));
  /* Read the address of OSSetExceptionCallback() */
  OSDynLoad_FindExport(coreinit_handle, 0, "OSSetExceptionCallback", &OSSetExceptionCallback);
  OSSetExceptionCallback(2, &exception_handler);
  OSSetExceptionCallback(3, &exception_handler);
  OSSetExceptionCallback(6, &exception_handler);
}

#endif /* EXCEPTION_HANDLER_H */