#include "loader.h"

#define OSTHREAD_SIZE	0x1000
#define OS_THREAD_ATTR_AFFINITY_NONE  0x0007u        // affinity to run on every core
#define OS_THREAD_ATTR_AFFINITY_CORE0 0x0001u        // run only on core0
#define OS_THREAD_ATTR_AFFINITY_CORE1 0x0002u        // run only on core1
#define OS_THREAD_ATTR_AFFINITY_CORE2 0x0004u        // run only on core2
#define OS_THREAD_ATTR_DETACH         0x0008u        // detached
#define OS_THREAD_ATTR_LAST           (OS_THREAD_ATTR_DETACH | OS_THREAD_ATTR_AFFINITY_NONE)

#define PRINT_SCREEN(str){  OSScreenFlipBuffersEx(1); \
                            OSScreenPutFontEx(1, screen_x, screen_y++, str); \
                            OSScreenFlipBuffersEx(1); }

/* Exception callback */
char ex_cb(void *context);

static bool CreateThread();
void* thread_ptr;
static int Thread_Socket(int intArg, void *ptrArg);

void _start()
{
    /****************************>            Fix Stack            <****************************/
    //Load a good stack
    asm(
        "lis %r1, 0x1ab5 ;"
        "ori %r1, %r1, 0xd138 ;"
    );

    /****************************>           Get Handles           <****************************/
    unsigned int coreinit_handle, vpad_handle, nsysnet_handle, dmae_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    OSDynLoad_Acquire("vpad.rpl", &vpad_handle);
    OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);
 
    /****************************>       External Prototypes       <****************************/
    int (*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);
    void (*_Exit)();
    char (*OSSetExceptionCallback)(unsigned char ex, char (*callback)(void*));
    void (*DCFlushRange)(void *buffer, uint32_t length);
    void* (*OSAllocFromSystem)(uint32_t size, int align);
    void (*OSResetEvent)();

    void (*OSScreenInit)();
    unsigned int (*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
    unsigned int (*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
    unsigned int (*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
    unsigned int (*OSScreenFlipBuffersEx)(unsigned int bufferNum);
    unsigned int (*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, void * buffer);

    bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t stack, uint32_t stack_size, int32_t priority, uint16_t attr);
    int32_t (*OSResumeThread)(void *thread);
    long (*OSCheckActiveThreads)();
    void* (*OSGetThreadName)(void* thread);

    /****************************>             Exports             <****************************/
    
    OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);
    OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSSetExceptionCallback", &OSSetExceptionCallback);
    OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSResetEvent", &OSResetEvent);
    
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenFlipBuffersEx", &OSScreenFlipBuffersEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);

    OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSCheckActiveThreads", &OSCheckActiveThreads);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSGetThreadName", &OSGetThreadName);

    /* Set up DSI, ISI, and program exception callbacks */
    OSSetExceptionCallback(2, &ex_cb);
    OSSetExceptionCallback(3, &ex_cb);
    OSSetExceptionCallback(6, &ex_cb);
    
    /****************************>         Clear screen            <****************************/
    int screen_buf0_size;
    int screen_buf1_size;
    
    // Init screen
    OSScreenInit();

    // Set screens buffers
    screen_buf0_size = OSScreenGetBufferSizeEx(0);
    screen_buf1_size = OSScreenGetBufferSizeEx(1);

    OSScreenSetBufferEx(0, (void *)0xF4000000);
    OSScreenSetBufferEx(1, (void *)0xF4000000 + screen_buf0_size);

    // Clear screens
    uint32_t num = 0; // (r << 24) | (g << 16) | (b << 8) | a;
    OSScreenClearBufferEx(0, num);
    OSScreenClearBufferEx(1, num);

    // Flush the cache
    DCFlushRange((void *)0xF4000000,screen_buf0_size);
    DCFlushRange((void *)0xF4000000 + screen_buf0_size, screen_buf1_size);

    // Flip buffers
    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    // Text position
    int screen_x = 0;
    int screen_y = 0;

    /****************************>              vars               <****************************/
    char msg[512];

    /****************************>              Thread             <****************************/
    bool thread_quit = true;
    long nb_threads_orig = OSCheckActiveThreads();
    long nb_threads_cur = nb_threads_orig;
    __os_snprintf(msg, 512, "nb threads : %d\0", nb_threads_orig);
    PRINT_SCREEN(msg);

    /****************************>              Test               <****************************/
    
    /****************************>            Main Loop            <****************************/
    int error;
    int button_pressed = 0;
    VPADData vpad_data;
    VPADRead(0, &vpad_data, 1, &error); //Read initial vpad status

    while(1)
    {
        VPADRead(0, &vpad_data, 1, &error);
        if(vpad_data.btn_hold & BUTTON_HOME)
        {
            break;
        }

        if(vpad_data.btn_hold & BUTTON_B && !button_pressed)
        {
        }

        if(vpad_data.btn_hold & BUTTON_A && !button_pressed)
        {
            PRINT_SCREEN("Create thread ...");

            if (!CreateThread())
                PRINT_SCREEN("Failed to create thread");

            nb_threads_cur = OSCheckActiveThreads();
            thread_quit = false;
            if (nb_threads_cur > nb_threads_orig)
                PRINT_SCREEN("Thread running")
            else
                thread_quit = true;

            __os_snprintf(msg, 512, "nb threads : %d\0", nb_threads_cur);
            PRINT_SCREEN(msg);
        }

        if(vpad_data.btn_hold & BUTTON_Y && !button_pressed)
        {
            int* test = *_Exit;
            __os_snprintf(msg, 512, "_Exit:0x%08x\0", (int)_Exit);
            PRINT_SCREEN(msg);
            int i;
            for (i = 0; i < 16; i = i + 4)
            {
                __os_snprintf(msg, 512, "0x%08x 0x%08x 0x%08x 0x%08x\0", (int)test[i + 0], (int)test[i + 1], (int)test[i + 2], (int)test[i + 3]);
                PRINT_SCREEN(msg);
            }
        }
        
        if(vpad_data.btn_hold & BUTTON_X && !button_pressed)
        {

        }

        // Check thread
        if (!thread_quit)
        {
            nb_threads_cur = OSCheckActiveThreads();
            if (nb_threads_cur <= nb_threads_orig)
            {
                PRINT_SCREEN("Thread stopped");
                thread_quit = true;
            }
        }

        // Button pressed ?
        button_pressed = (vpad_data.btn_hold) ? 1 : 0;
    }

    _Exit();
}

/* Exception callback */
char ex_cb(void *context)
{
    /* Get a handle to coreinit.rpl */
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

    void (*__Exit)();
    OSDynLoad_FindExport(coreinit_handle, 0, "__Exit", &__Exit);
    __Exit();
}

static bool CreateThread()
{
    /****************************>           Get Handles           <****************************/
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
 
    /****************************>       External Prototypes       <****************************/
    void* (*OSAllocFromSystem)(uint32_t size, int align);
    bool (*OSCreateThread)(void *thread, void *entry, int argc, void *args, uint32_t stack, uint32_t stack_size, int32_t priority, uint16_t attr);
    int32_t (*OSResumeThread)(void *thread);
    
    /****************************>             Exports             <****************************/
    OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSCreateThread", &OSCreateThread);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSResumeThread", &OSResumeThread);

    /* Allocate a stack for the thread */
    uint32_t stack = (uint32_t) OSAllocFromSystem(0x1000, 0x10);
    stack += 0x1000;
    
    /* Create the thread */
    void *thread = OSAllocFromSystem(OSTHREAD_SIZE, 8);
    bool ret = OSCreateThread(thread, Thread_Socket, 0, null, stack, 0x1000, 0, OS_THREAD_ATTR_AFFINITY_CORE1 | OS_THREAD_ATTR_DETACH);
    if (ret == false)
        return false;

    /* Schedule it for execution */
    OSResumeThread(thread);

    return true;
}

static int Thread_Socket(int intArg, void *ptrArg)
{
    unsigned int coreinit_handle, nsysnet_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);

    /* Socket functions */
    int (*socket)(int family, int type, int proto);
    int (*connect)(int fd, struct sockaddr *addr, int addrlen);
    int (*recv)(int fd, void *buffer, int len, int flags);
    int (*send)(int fd, const void *buffer, int len, int flags);
    int (*close)(int fd);

    OSDynLoad_FindExport(nsysnet_handle, 0, "socket", &socket);
    OSDynLoad_FindExport(nsysnet_handle, 0, "connect", &connect);
    OSDynLoad_FindExport(nsysnet_handle, 0, "recv", &recv);
    OSDynLoad_FindExport(nsysnet_handle, 0, "send", &send);
    OSDynLoad_FindExport(nsysnet_handle, 0, "close", &close);

    //
    struct sockaddr sin;
    sin.sin_family = AF_INET;
    sin.sin_port = 12345;
    sin.sin_addr.s_addr = PC_IP;
    int i, j, k;
    for (i = 0; i < 8; i++)
        sin.sin_zero[i] = 0;
    
    int rpc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (rpc < 0)
        return 1;

    int status = connect(rpc, &sin, 0x10);
    if (status != 0)
        return 1;

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
            return 1;
        }
    }

    return 0;
}
