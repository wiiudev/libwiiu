
.extern _memset
.extern _doInit
.extern _main
.extern _doExit

	.globl _start
_start:
	# Setup stack.
	lis 1,_stack_top@ha ; addi 1,1,_stack_top@l ; li 0,0 ; stwu 0,-64(1)

	# Clear BSS.
	lis 3,__bss_start@ha ; addi 3,3,__bss_start@l
	li 4,0
	lis 5,__bss_end@ha ; addi 5,5,__bss_end@l ; sub 5,5,3
	bl _memset

	bl _doInit
	bl _main
	# Return to old stack for exit
	lis 1, 0x1ab5 ; ori 1, 1, 0xd138
	bl _doExit
