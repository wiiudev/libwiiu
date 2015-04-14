#include "loader.h"
void _start()
{
	/* Get a handle to coreinit.rpl */
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	//OS memory functions
	void* (*memcpy)(void *dest, void *src, uint32_t length);
	void* (*memset)(void * dest, uint32_t value, uint32_t bytes);
	void* (*OSAllocFromSystem)(uint32_t size, int align);
	void (*OSFreeToSystem)(void *ptr);
	OSDynLoad_FindExport(coreinit_handle, 0, "memcpy", &memcpy);
	OSDynLoad_FindExport(coreinit_handle, 0, "memset", &memset);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
	
	void (*OSCoherencyBarrier)();
	OSDynLoad_FindExport(coreinit_handle, 0, "OSCoherencyBarrier", &OSCoherencyBarrier);
	
	void (*_Exit)();
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	
	//DC memory functions
	void (*DCFlushRangeNoSync)(void *buffer, uint32_t length);
	void (*DCInvalidateRange)(void *buffer, uint32_t length);
  	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRangeNoSync", &DCFlushRangeNoSync);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCInvalidateRange", &DCInvalidateRange);
	
	//LC memory functions
	void* (*LCAlloc)( uint32_t bytes );
	void (*LCDealloc)();
	uint32_t (*LCHardwareIsAvailable)();
	uint32_t (*LCIsDMAEnabled)();
	void (*LCEnableDMA)();
	void (*LCDisableDMA)();
	void (*LCLoadDMABlocks)(void* lc_addr, void* src_addr, uint32_t blocks);
	void (*LCStoreDMABlocks)(void* dest_addr, void* lc_addr, uint32_t blocks);
	void (*LCWaitDMAQueue)( uint32_t length );
	OSDynLoad_FindExport(coreinit_handle, 0, "LCAlloc", &LCAlloc);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCDealloc", &LCDealloc);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCHardwareIsAvailable", &LCHardwareIsAvailable);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCIsDMAEnabled", &LCIsDMAEnabled);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCEnableDMA", &LCEnableDMA);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCDisableDMA", &LCDisableDMA);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCLoadDMABlocks", &LCLoadDMABlocks);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCStoreDMABlocks", &LCStoreDMABlocks);
	OSDynLoad_FindExport(coreinit_handle, 0, "LCWaitDMAQueue", &LCWaitDMAQueue);
		
	//Used for keeping track of vairables to print to screen
	char output[1000];
	
	//Alloc 64 byte alligned space
	void* src_addr=OSAllocFromSystem(512,64);
	void* dest_addr=OSAllocFromSystem(512,64);
	
	//Store some debug values
	__os_snprintf(output, 1000, "src_addr:%02x,",(uint32_t)src_addr);
	__os_snprintf(output+strlen(output), 255, "dest_addr: %02x,", (uint32_t)dest_addr);
	
	//Number of 32bit blocks to copy. Must be multiple of 2 between [0,127]
	uint32_t blocks=2; //2 32bit blocks
	
	//Do something to the source
	memset(src_addr,1,64);
	
	//Grab values for debug
	uint32_t * src_val=src_addr;
	uint32_t * dest_val=dest_addr;
	__os_snprintf(output+strlen(output), 255, "Old src_val: %02x\n",src_val[0]);
	__os_snprintf(output+strlen(output), 255, "Old dest_val: %02x,",dest_val[0]);
	
	//Get some locked cache address space
	void *lc_addr=LCAlloc(512); //512 Minmum. Must be multiple of 512.
	
	//Calculate size from blocks to flush/invalidate range properly
	uint32_t size;
	//If blocks is set to 0, the transaction will default to 128 blocks
	if(blocks==0)
	{
		size=32*128;
	}
	else
	{
		size=32*blocks;
	}
	//Flush the range at the source to ensure cache gets written back to memory.
	DCFlushRangeNoSync(src_addr,size);
	//Invalidate the range at the destination
	DCInvalidateRange(dest_addr,size);
	//Sync
	OSCoherencyBarrier();
	
	//Check to see of DMA hardware is avaliable
	if(LCHardwareIsAvailable()!=1)
	{
		OSFatal("Hardware is not avaliable.");
	}

	//Gets the current state of DMA, so we can restore it after our copy
	uint32_t dmaState=LCIsDMAEnabled();

	//Checks to see if DMA is enabled, if not it will try to enable it.
	if(dmaState!=1)
	{
		LCEnableDMA();
		dmaState=LCIsDMAEnabled();
		if(dmaState!=1)
		{
			OSFatal("Can't enable DMA");
		}
	}
	
	//Load memory to locked cache
	LCLoadDMABlocks(lc_addr,src_addr,blocks);
	LCWaitDMAQueue(0);
	
	//Store memory from locked cache
	LCStoreDMABlocks(dest_addr,lc_addr,blocks);
	LCWaitDMAQueue(0);

	//If DMA was not previously enabled, then disable it to restore state.
	if(dmaState!=1)
	{
		LCDisableDMA();
		dmaState=LCIsDMAEnabled();
		//If DMA failed to disable, return error code
		if(dmaState!=1)
		{
			OSFatal("Couldn't Disable DMA");
		}
	}
	__os_snprintf(output+strlen(output), 255, "New src_val: %02x,",src_val[0]);
	__os_snprintf(output+strlen(output), 255, "New dest_val: %02x,",dest_val[0]);
	
	//Cleanup
	LCDealloc(lc_addr);
	OSFreeToSystem(dest_addr);
	OSFreeToSystem(src_addr);
    OSFatal(output);
}
/*
coreinit.rpl LCStoreDMABlocks

Original Instructions           Step 1 (google it)      Conversion
----------------------------------------------------------------------------
clrlslwi  r12, r5, 30,2	=>		rlwinm r12,r5,2,28,29	=>r12=(r5 << 2) & 0xC
extrwi    r11, r5, 5,25	 =>     rlwinm r11,r5,30,27,3	=>r11=(r5<<30) & 0x1F
or        r0, r4, r12									=>		r0=r4|r12
or        r3, r3, r11									=>		r3=r3|r11
ori       r4, r0, 2										=>		r4=r0|2
li, r0, 0xE                                             =>syscall_0xE(r3,r4)
sc
----------------------------------------------------------------------------
Use substitution
----------------------------------------------------------------------------
 r3=r3|((r5<<30) & 0x1F)
 r4=(r4|((r5 << 2) & 0xC))|2
----------------------------------------------------------------------------
C code
----------------------------------------------------------------------------
LCStoreDMABlocks(void *dest_addr,void *lc_addr,uint32_t blockNum)
{
firstParam=dest_addr|((blockNum<<30) & 0x1F);
secondParam=(lc_addr|((r5 << 2) & 0xC))|2; //So, it looks like 0000 0000 0000 0000 0000 0000 000x 0000 bit is set if we're coping to lc_addr. If it's not set, then we're copying from lc_addr to memory. The last two bits of block num are placed here 0000 0000 0000 0000 0000 0000 0000 xx00. The second to last bit is always set 0000 0000 0000 0000 0000 0000 0000 00x0.

syscall_0xE(firstParam,secondParam); __LCLoadDMABlocks
}
----------------------------------------------------------------------------
Notes
----------------------------------------------------------------------------
	//lc_addr: Locked cache destination address. Use LCAlloc to get memory.
	//dest_addr: memory destination address. Must be 64-byte aligned. Should be a multiple of 65 bytes to ensure coherency with L2 64-byte fetching.
	//blocks: Transfer size of cache blocks. Each block is 32B. Range is [0,127] and must be a multiple of 2 (64 bytes). If 0 is specified the transaction size will default to 128 blocks (4KB).
*/

/*
coreinit.rpl LCLoadDMABlocks

Original Instructions           Step 1 (google it)      Conversion
----------------------------------------------------------------------------
clrlslwi r0,r5,30,2     =>      rlwinm r0,r5,2,28,29    =>r0=(r5 << 2) & 0xC
 
ori r11,r3,0x10                                         =>r11=r3|0x10
 
extrwi r12,r5,5,25      =>      rlwinm r12,r5,30,27,31  =>r12=(r5<<30) & 0x1F
 
or r10,r11,r0                                           =>r10=r11|r0
 
or r3,r4,r12                                            =>r3=r4|r12
 
ori r4, r10, 2                                          =>r4=r10|2
 
li, r0, 0xE                                             =>syscall_0xE(r3,r4)
sc
----------------------------------------------------------------------------
Use substitution
----------------------------------------------------------------------------
r3=r4|((r5<<30) & 0x1F)
 
r4=((r3|0x10)|((r5 << 2) & 0xC))|2
----------------------------------------------------------------------------
C code
----------------------------------------------------------------------------
LCLoadDMABlocks(void *lc_addr,void *src_addr, uint32_tblockNum)
{
firstParam=src_addr|((blockNum<<30) & 0x1F);
secondParam=((lc_addr|0x10)|(((blockNum << 2)) & 0xC))|2;//So, it looks like 0000 0000 0000 0000 0000 0000 000x 0000 bit is set if we're coping to lc_addr. If it's not set, then we're copying from lc_addr to memory. The last two bits of block num are placed here 0000 0000 0000 0000 0000 0000 0000 xx00. The second to last bit is always set 0000 0000 0000 0000 0000 0000 0000 00x0.
syscall_0xE(firstParam,secondParam); // __LCLoadDMABlocks
}
----------------------------------------------------------------------------
Notes
----------------------------------------------------------------------------
	//lc_addr: Locked cache destination address. Use LCAlloc to get memory.
	//src_addr: memory source address. Must be 64-byte aligned. Should be a multiple of 65 bytes to ensure coherency with L2 64-byte fetching.
	//blocks: Transfer size of cache blocks. Each block is 32B. Range is [0,127] and must be a multiple of 2 (64 bytes). If 0 is specified the transaction size will default to 128 blocks (4KB).
*/

/*
kernel.img syscall_0xE

# =============== S U B	R O U T	I N E =======================================

# Attributes: thunk

LCLoadDMABlocks:
		b	  __LCLoadDMABlocks	//Jump immediately to helper function
# End of function LCLoadDMABlocks

# ---------------------------------------------------------------------------
		.align 5
# =============== S U B	R O U T	I N E =======================================
__LCLoadDMABlocks:			# CODE XREF: LCLoadDMABlocksj
upir = 0x3EF //Some register used to get/set state of DMA hardware
		mfspr	  r6, upir	# upir //Setting up some sort of check before DMA is started by accessing the mtsprs. Probably has to do with checks to msfpr 0x3EF to ensure DMA hardware is in enabled state.
		mfspr	  r11, fpecr # Floating-point exception	cause register
		rotrwi	  r11, r11, 11 //Do something to the floating point exeception cause register
		cmpwi	  r6, 2	//This instruction happens to value of upir too in LCHardwareIsAvailable.
		lwz	  r5, 0xB8(r11) //Do something to the floating point exeception cause register
		lwz	  r7, 0xC8(r11) //Do something to the floating point exeception cause register
		cmpwi	  cr1, r5, 0xF //Do something to the floating point exeception cause register
		cmpwi	  cr7, r7, 0 //Do something to the floating point exeception cause register
		bne	  loc_FFF02488	//Check for failure
		bne	  cr1, loc_FFF024AC	//Failure
		b	  loc_FFF0248C	//Ready to start DMA
# ---------------------------------------------------------------------------

loc_FFF02488:				# CODE XREF: __LCLoadDMABlocks+20j
		bne	  cr7, loc_FFF024AC	//Failure

loc_FFF0248C:				# CODE XREF: __LCLoadDMABlocks+28j
		mtspr	  0x38A, r3 //Load source/destination address in virtual address space
		mtspr	  0x38B, r4 //Load locked cache address, control code for load/store, and number of blocks for the operation

loc_FFF02494:				# CODE XREF: __LCLoadDMABlocks+44j
		//Busywait for DMA transactions to complete
		mfspr	  r4, 0x388 //Get number of DMA transactions in the queue
		extrwi	  r4, r4, 4,4 //=>rlwinm r4,r4,8,28,31=>r4=(r4<<8)|0xF
		cmpwi	  r4, 0			//Compaire last 8 bits=2bytes to see if there are 0 transactions left. This is exactly what the LCWaitDMAQueue function does. Looking back at older firmware versions, it looks like there were asynchronus load/store functions. Now there is just synchronous as implemented in kernel.
		bgt	  loc_FFF02494
		li	  r3, 1	//return success
		rfi
# ---------------------------------------------------------------------------

loc_FFF024AC:				# CODE XREF: __LCLoadDMABlocks+24j
					# __LCLoadDMABlocks:loc_FFF02488j
		li	  r3, 0 //return failure
		rfi
# End of function __LCLoadDMABlocks
*/
