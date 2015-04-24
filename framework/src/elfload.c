#include "types.h"
#include "coreinit.h"

/* ELF loader source buffer */
#define ELFLOADER_SRC_ADDR	0x1dd7b814

/* Magic number for start of ELF */
#define ELF_MAGIC			0xCAFEC0DE

void _start()
{
	/* Load a good stack */
	asm(
		"lis %r1, 0x1ab5 ;"
		"ori %r1, %r1, 0xd138 ;"
		);

	/* Start searching memory for the ELF */
	uint32_t *elf_addr = (uint32_t*) ELFLOADER_SRC_ADDR;
	while (elf_addr < 0x1e000000)
	{
		if (*elf_addr == ELF_MAGIC) break;
		elf_addr++;
	}

	/* If the ELF wasn't found, fail, otherwise get to the actual data */
	if (elf_addr == (uint32_t*)0x1e000000) OSFatal("Failed to find ELF");
	else elf_addr++;

	OSFatal("Did not crash");
}
