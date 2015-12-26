
ici_stub.o:     file format elf32-powerpc


Disassembly of section .text:

00000000 <ici_stub>:
   0:	38 60 10 40 	li      r3,4160
   4:	7c 60 01 24 	mtmsr   r3
   8:	4c 00 01 2c 	isync
   c:	7c 79 fa a6 	mfl2cr  r3
  10:	3c 80 7f ff 	lis     r4,32767
  14:	60 84 ff ff 	ori     r4,r4,65535
  18:	7c 63 20 38 	and     r3,r3,r4
  1c:	7c 79 fb a6 	mtl2cr  r3
  20:	7c 00 04 ac 	sync    
  24:	7c 70 fa a6 	mfspr   r3,1008
  28:	3c 80 ff ff 	lis     r4,-1
  2c:	60 84 3f ff 	ori     r4,r4,16383
  30:	7c 63 20 38 	and     r3,r3,r4
  34:	7c 70 fb a6 	mtspr   1008,r3
  38:	7c 00 04 ac 	sync    

0000003c <.loop>:
  3c:	48 00 00 00 	b       3c <.loop>
