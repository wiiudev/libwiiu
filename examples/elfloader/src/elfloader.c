/*
 * Copyright (c) 2001 William L. Pitts
 * Modifications (c) 2004 Felix Domke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are freely
 * permitted provided that the above copyright notice and this
 * paragraph and the following disclaimer are duplicated in all
 * such forms.
 *
 * This software is provided "AS IS" and without any express or
 * implied warranties, including, without limitation, the implied
 * warranties of merchantability and fitness for a particular
 * purpose.
 */
#include "loader.h"
#include "elf_abi.h"

/* ======================================================================
 * Determine if a valid ELF image exists at the given memory location.
 * First looks at the ELF header magic field, the makes sure that it is
 * executable and makes sure that it is for a PowerPC.
 * ====================================================================== */
int valid_elf_image ()
{
    Elf32_Ehdr *ehdr; /* Elf header structure pointer */

    ehdr = (Elf32_Ehdr *) 0xF5800000;

    if (!IS_ELF (*ehdr))
        return 0;

    if (ehdr->e_type != ET_EXEC)
        return -1;

    if (ehdr->e_machine != EM_PPC)
        return -1;

    return 1;
}


/* ======================================================================
 * A very simple elf loader, assumes the image is valid, jumps to entry point.
 * ====================================================================== */
typedef void (*entrypoint) (void);
void load_elf_image () {

    Elf32_Ehdr *ehdr;
    Elf32_Phdr *phdrs;
    unsigned char *image;

    unsigned int elfstart = 0xF5800000;
    ehdr = (Elf32_Ehdr *) elfstart;

    if(ehdr->e_phoff == 0 || ehdr->e_phnum == 0)
        return;

    if(ehdr->e_phentsize != sizeof(Elf32_Phdr))
        return;

    phdrs = (Elf32_Phdr*)(elfstart + ehdr->e_phoff);

    int i = *((int*)0xF5FFFFFC);
    while(i < ehdr->e_phnum) {
        i = *(int*)0xF5FFFFFC;
        *(int*)0xF5FFFFFC = i+1;

        if(phdrs[i].p_type != PT_LOAD)
            continue;

        if(phdrs[i].p_filesz > phdrs[i].p_memsz)
            return;

        if(!phdrs[i].p_filesz)
            continue;

        image = (unsigned char *) (elfstart + phdrs[i].p_offset);
        if(phdrs[i].p_paddr >= 0x01800000 && phdrs[i].p_paddr < 0x02000000)
            dorop (phdrs[i].p_paddr, (unsigned int)image, phdrs[i].p_filesz);
        else
            memcpy ((void *) phdrs[i].p_paddr, (const void *) image, phdrs[i].p_filesz);
    }
    entrypoint exeEntryPoint = (entrypoint)ehdr->e_entry;
    //prepare and execute
    *(unsigned int*)0xF5FFFFFC = (unsigned int)OSDynLoad_Acquire;
    *(unsigned int*)0xF5FFFFF8 = (unsigned int)OSDynLoad_FindExport;
    *(unsigned int*)0xF5FFFFF4 = (unsigned int)OSFatal;
    *(unsigned int*)0xF5FFFFF0 = (unsigned int)__os_snprintf;
    exeEntryPoint();
}
