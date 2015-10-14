#include "loader.h"
#if VER == 200
	#include "codehandler310.h" //TODO
	#define INSTALL_ADDR 0xBD1D3000
	#define PATCH_ADDR	 0xBD01894C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x0C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xBC000000
#elif VER == 210
	#include "codehandler310.h" //TODO
	#define INSTALL_ADDR 0xBD1D3000
	#define PATCH_ADDR	 0xBD01894C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x0C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xBC000000
#elif VER == 300
	#include "codehandler310.h" //TODO ???
	#define INSTALL_ADDR 0xBD1D3000
	#define PATCH_ADDR	 0xBD01894C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x0C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xBC000000
#elif VER == 310
	#include "codehandler310.h"
	#define INSTALL_ADDR 0xBD1D3000
	#define PATCH_ADDR	 0xBD01894C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x0C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xBC000000
#elif VER == 400
	#include "codehandler410.h" //TODO
	#define INSTALL_ADDR 0xA11DD000
	#define PATCH_ADDR	 0xA101C55C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x1C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xA0000000
#elif VER == 410
	#include "codehandler410.h"
	#define INSTALL_ADDR 0xA11DD000
	#define PATCH_ADDR	 0xA101C55C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x1C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xA0000000
#elif VER == 500
	#include "codehandler500.h"
	#define INSTALL_ADDR 0xA11DD000
	#define PATCH_ADDR	 0xA101C55C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x1C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xA0000000
#elif VER == 532
	#include "codehandler532.h"
	#define INSTALL_ADDR 0xA11DD000
	#define PATCH_ADDR	 0xA101C55C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x1C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xA0000000
#elif VER == 550
	#include "codehandler550.h"
	#define INSTALL_ADDR 0xA11DD000
	#define PATCH_ADDR	 0xA101C56C
	#define FLUSH_ADDR	 PATCH_ADDR - 0x1C
	#define BRANCH_ADDR  INSTALL_ADDR - 0xA0000000
#endif

#define assert(x) \
    do { \
        if (!(x)) \
            OSFatal("Assertion failed " #x ".\n"); \
    } while (0)
		
#define ALIGN_BACKWARD(x,align) \
	((typeof(x))(((unsigned int)(x)) & (~(align-1))))

int doBL( unsigned int dst, unsigned int src );

void _start()
{
    /* Load a good stack */
    asm(
        "lis %r1, 0x1ab5 ;"
        "ori %r1, %r1, 0xd138 ;"
    );

    /* Get a handle to coreinit.rpl. */
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

	/* Get for later socket patch */
    unsigned int nsysnet_handle;
    OSDynLoad_Acquire("nsysnet.rpl", &nsysnet_handle);

    /* Load a few useful symbols. */
	void (*memcpy)(void *dst, const void *src, int bytes);
	void*(*OSAllocFromSystem)(uint32_t size, int align);
	void (*memset)(void *dst, char val, int bytes);
    void (*_Exit)(void) __attribute__ ((noreturn));
	void (*ICInvalidateRange)(const void *, int);
    void*(*OSEffectiveToPhysical)(const void *);
    void (*DCFlushRange)(const void *, int);
	void (*OSFreeToSystem)(void *ptr);

    OSDynLoad_FindExport(coreinit_handle, 0, "OSEffectiveToPhysical", &OSEffectiveToPhysical);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "ICInvalidateRange", &ICInvalidateRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
    OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	OSDynLoad_FindExport(coreinit_handle, 0, "memcpy", &memcpy);
	OSDynLoad_FindExport(coreinit_handle, 0, "memset", &memset);
    OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);

	assert(OSEffectiveToPhysical);
	assert(OSAllocFromSystem);
	assert(ICInvalidateRange);
	assert(OSFreeToSystem);
    assert(DCFlushRange);
    assert(memcpy);
    assert(memset);
    assert(_Exit);

	//IM functions
	int(*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b);
	int(*IM_Close)(int fd);
	int(*IM_Open)();

	OSDynLoad_FindExport(coreinit_handle, 0, "IM_SetDeviceState", &IM_SetDeviceState);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Close", &IM_Close);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Open", &IM_Open);

	assert(IM_SetDeviceState);
	assert(IM_Close);
	assert(IM_Open);

	//Restart system to get lib access
	int fd = IM_Open();
	void *mem = OSAllocFromSystem(0x100, 64);
	memset(mem, 0, 0x100);
	//set restart flag to force quit browser
	IM_SetDeviceState(fd, mem, 3, 0, 0); 
	IM_Close(fd);
	OSFreeToSystem(mem);
	//wait a bit for browser end
	unsigned int t1 = 0x1FFFFFFF;
	while(t1--) ;

    /* Make sure the kernel exploit has been run */
#if (VER < 410)
    if (OSEffectiveToPhysical((void *)0xA0000000) != (void *)0x30000000)
#else
	if (OSEffectiveToPhysical((void *)0xA0000000) != (void *)0x31000000)
#endif
	{
        OSFatal("You must run ksploit before installing PyGecko.");
    }
	else
	{
		/* Get the socket function to patch */
		unsigned int *topatch;
		OSDynLoad_FindExport(nsysnet_handle, 0, "socket_lib_finish", &topatch);
#if (VER < 410)
		topatch = (unsigned int*)((unsigned int)topatch + 0xBC000000);
#else
		topatch = (unsigned int*)((unsigned int)topatch + 0xA0000000);
#endif

        /* Install codehandler */
		memcpy((void*)INSTALL_ADDR, codehandler_text_bin, codehandler_text_bin_len);
		DCFlushRange((void*)INSTALL_ADDR, codehandler_text_bin_len);
		ICInvalidateRange((void*)INSTALL_ADDR, codehandler_text_bin_len);

		/* Patch coreinit jump */
		*((uint32_t *)PATCH_ADDR) = doBL(BRANCH_ADDR, PATCH_ADDR - 0xA0000000);
		DCFlushRange((void*)FLUSH_ADDR, 0x20);
		ICInvalidateRange((void*)FLUSH_ADDR, 0x20);

		/* Patch Socket Function */
		topatch[0] = 0x38600000;
		topatch[1] = 0x4E800020;
		unsigned int *faddr = ALIGN_BACKWARD(topatch, 32);
		DCFlushRange(faddr, 0x40);
		ICInvalidateRange(faddr, 0x40);

        /* The fix for Splatoon and such */
		kern_write((void*)(KERN_ADDRESS_TBL + (0x12 * 4)), 0x00000000);
		kern_write((void*)(KERN_ADDRESS_TBL + (0x13 * 4)), 0x14000000);     
	}

    _Exit();
}

int doBL( unsigned int dst, unsigned int src )
{
	unsigned int newval = (dst - src);
	newval &= 0x03FFFFFC;
	newval |= 0x48000001;
	return newval;
}

void* memcpy(void* dst, const void* src, uint32_t size)
{
	uint32_t i;
	for (i = 0; i < size; i++)
		((uint8_t*) dst)[i] = ((const uint8_t*) src)[i];
	return dst;
}

/* Write a 32-bit word with kernel permissions */
void kern_write(void *addr, uint32_t value)
{
	asm(
		"li 3,1\n"
		"li 4,0\n"
		"mr 5,%1\n"
		"li 6,0\n"
		"li 7,0\n"
		"lis 8,1\n"
		"mr 9,%0\n"
		"mr %1,1\n"
		"li 0,0x3500\n"
		"sc\n"
		"nop\n"
		"mr 1,%1\n"
		:
		:	"r"(addr), "r"(value)
		:	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
			"11", "12"
		);
}
