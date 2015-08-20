
OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)

ENTRY(_start)

SECTIONS {
	. = 0x01808000;

	__rx_start = .;

	.start : { crt0.o(*) }
	.text : { *(.text) }
	.rodata : { *(.rodata .rodata.*)}

	__rx_end = .;

	. = 0xF4C00000;

	__rw_start = .;

	.data : { *(.data) }

	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = .;

	. = ALIGN(0x40);
	.stack : {
		. += 0x8000;
		_stack_top = .;
	}

	__rw_end = .;
}
