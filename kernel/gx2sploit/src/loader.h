#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"
#include "../../../libwiiu/src/socket.h"
#include "../../../libwiiu/src/uhs.h"
#include "../../../libwiiu/src/types.h"

/* Gadget finding addresses */
#define JIT_ADDRESS			        0x01800000
#if VER == 300
	#define CODE_ADDRESS_START		0x0E000000
	#define CODE_ADDRESS_END		0x10000000
#else
	#define CODE_ADDRESS_START		0x0D800000
	#define CODE_ADDRESS_END		0x0F848A0C
#endif

/* Kernel addresses, stolen from Chadderz */
#define KERN_HEAP					0xFF200000
#define KERN_HEAP_PHYS				0x1B800000
#if VER == 200
	#define KERN_SYSCALL_TBL		0x0
	#define KERN_CODE_READ			0x0
	#define KERN_CODE_WRITE			0x0
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0x0)
#elif VER == 210
	#define KERN_SYSCALL_TBL		0x0
	#define KERN_CODE_READ			0x0
	#define KERN_CODE_WRITE			0x0
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0x0)
#elif VER == 300
	#define KERN_SYSCALL_TBL		0xFFE85950
	#define KERN_CODE_READ			0xFFF02214
	#define KERN_CODE_WRITE			0xFFF02234
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0xFFEB66E4)
#elif VER == 310
	#define KERN_SYSCALL_TBL		0x0
	#define KERN_CODE_READ			0x0
	#define KERN_CODE_WRITE			0x0
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0x0)
#elif VER == 400
	#define KERN_SYSCALL_TBL		0x0
	#define KERN_CODE_READ			0x0
	#define KERN_CODE_WRITE			0x0
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0x0)
#elif VER == 410
	#define KERN_SYSCALL_TBL		0xffe85890
	#define KERN_CODE_READ			0xfff02214
	#define KERN_CODE_WRITE			0xfff02234
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0xffeb902c)
#elif VER == 500
	#define KERN_SYSCALL_TBL		0xffea9520
	#define KERN_CODE_READ			0xfff021f4
	#define KERN_CODE_WRITE			0xfff02214
	#define KERN_DRVPTR				0x0
	#define KERN_ADDRESS_TBL		((uint32_t*)0xffea9e4c)
#elif (VER == 532) || (VER == 540)
	#define KERN_SYSCALL_TBL		0xFFEAA0E0
	#define KERN_CODE_READ			0xFFF02274
	#define KERN_CODE_WRITE			0xFFF02294
	#define KERN_DRVPTR				0xFFEAA7A0
	#define KERN_ADDRESS_TBL		((uint32_t*)0xFFEAAA10)
#elif VER == 550
	#define KERN_SYSCALL_TBL		0xFFEAAE60
	#define KERN_CODE_READ			0xFFF023D4
	#define KERN_CODE_WRITE			0xFFF023F4
	#define KERN_DRVPTR				0xFFEAB530
	#define KERN_ADDRESS_TBL		((uint32_t*)0xFFEAB7A0)
#else
#error "Unsupported Wii U software version"
#endif

/* Kernel heap constants */
#define STARTID_OFFSET				0x08
#define METADATA_OFFSET				0x14
#define METADATA_SIZE				0x10

/* Application start */
void _start();

/* Find a ROP gadget by a sequence of bytes */
void *find_gadget(uint32_t code[], uint32_t length, uint32_t gadgets_start);

/* Arbitrary read and write syscalls */
uint32_t kern_read(const void *addr);
void kern_write(void *addr, uint32_t value);

#endif /* LOADER_H */
