#ifndef ELF_MACROS_H
#define ELF_MACROS_H

/* ELF header and general info */
#define ELF_CLASS_32						1
#define ELF_CLASS_64						2

#define ELF_ENCODING_LITTLE_ENDIAN			1
#define ELF_ENCODING_BIG_ENDIAN				2

#define ELF_TYPE_RELOCATABLE_FILE			1
#define ELF_TYPE_EXECUTABLE_FILE			2
#define ELF_TYPE_SHARED_OBJ_FILE			3
#define ELF_TYPE_CORE_FILE					4

#define ELF_MACHINE_80386					3

#define ELF_OBJ_FILE_VERSION_INVALID		0
#define ELF_OBJ_FILE_VERSION_CURRENT		1

/* ELF program headers */
#define ELF_PT_NULL							0
#define ELF_PT_LOAD							1
#define ELF_PT_DYNAMIC						2
#define ELF_PT_INTERP						3
#define ELF_PT_NOTE							4
#define ELF_PT_SHLIB						5
#define ELF_PT_PHDR							6
#define ELF_PT_LOPROC						0x70000000
#define ELF_PT_HIPROC						0x7FFFFFFF

#define ELF_PF_EXECUTE						0x01
#define ELF_PF_WRITE						0x02
#define ELF_PF_READ							0x04

/* ELF section headers */
#define ELF_SN_UNDEF						0
#define ELF_SN_LORESERVE					0xff00
#define ELF_SN_LOPROC						0xff00
#define ELF_SN_HIPROC						0xff1f
#define ELF_SN_ABS							0xfff1
#define ELF_SN_COMMON						0xfff2
#define ELF_SN_HIRESERVE					0xffff

#define ELF_ST_NULL							0
#define ELF_ST_PROGBITS						1
#define ELF_ST_SYMTAB						2
#define ELF_ST_STRTAB						3
#define ELF_ST_RELA							4
#define ELF_ST_HASH							5
#define ELF_ST_DYNAMIC						6
#define ELF_ST_NOTE							7
#define ELF_ST_NOBITS						8
#define ELF_ST_REL							9
#define ELF_ST_SHLIB						10
#define ELF_ST_DYNSYM						11
#define ELF_ST_INIT_ARRAY					14
#define ELF_ST_FINI_ARRAY					15
#define ELF_ST_PREINIT_ARRAY				16
#define ELF_ST_GROUP						17
#define ELF_ST_SYMTAB_SHNDX					18
#define ELF_ST_LOOS							0x60000000
#define ELF_ST_HIOS							0x6fffffff
#define ELF_ST_LOPROC						0x70000000
#define ELF_ST_HIPROC						0x7fffffff
#define ELF_ST_LOUSER						0x80000000
#define ELF_ST_HIUSER						0xffffffff

#define ELF_SF_WRITE						0x01
#define ELF_SF_ALLOC						0x02
#define ELF_SF_EXECINSTR					0x04
#define ELF_SF_EXECPROC						0xf0000000

/* ELF architectures */
#define ELF_ARCH_NONE						0
#define ELF_ARCH_M32						1
#define ELF_ARCH_SPARC						2
#define ELF_ARCH_386						3
#define ELF_ARCH_68K						4
#define ELF_ARCH_88K						5
#define ELF_ARCH_860						7
#define ELF_ARCH_MIPS						8
#define ELF_ARCH_S370						9
#define ELF_ARCH_MIPS_RS3_LE				10
#define ELF_ARCH_PARISC						15
#define ELF_ARCH_VPP500						17
#define ELF_ARCH_SPARC32PLUS				18
#define ELF_ARCH_960						19
#define ELF_ARCH_PPC						20
#define ELF_ARCH_PPC64						21
#define ELF_ARCH_S390						22
#define ELF_ARCH_SPU						23
#define ELF_ARCH_V800						36
#define ELF_ARCH_FR20						37
#define ELF_ARCH_RH32						38
#define ELF_ARCH_RCE						39
#define ELF_ARCH_ARM						40
#define ELF_ARCH_ALPHA						41
#define ELF_ARCH_SH							42
#define ELF_ARCH_SPARCV9					43
#define ELF_ARCH_TRICORE					44
#define ELF_ARCH_ARC						45
#define ELF_ARCH_H8_300						46
#define ELF_ARCH_H8_300H					47
#define ELF_ARCH_H8S						48
#define ELF_ARCH_H8_500						49
#define ELF_ARCH_IA_64						50
#define ELF_ARCH_MIPS_X						51
#define ELF_ARCH_COLDFIRE					52
#define ELF_ARCH_68HC12						53
#define ELF_ARCH_MMA						54
#define ELF_ARCH_PCP						55
#define ELF_ARCH_NCPU						56
#define ELF_ARCH_NDR1						57
#define ELF_ARCH_STARCORE					58
#define ELF_ARCH_ME16						59
#define ELF_ARCH_ST100						60
#define ELF_ARCH_TINYJ						61
#define ELF_ARCH_X86_64						62
#define ELF_ARCH_PDSP						63
#define ELF_ARCH_PDP10						64
#define ELF_ARCH_PDP11						65
#define ELF_ARCH_FX66						66
#define ELF_ARCH_ST9PLUS					67
#define ELF_ARCH_ST7						68
#define ELF_ARCH_68HC16						69
#define ELF_ARCH_68HC11						70
#define ELF_ARCH_68HC08						71
#define ELF_ARCH_68HC05						72
#define ELF_ARCH_SVX						73
#define ELF_ARCH_ST19						74
#define ELF_ARCH_VAX						75
#define ELF_ARCH_CRIS						76
#define ELF_ARCH_JAVELIN					77
#define ELF_ARCH_FIREPATH					78
#define ELF_ARCH_ZSP						79
#define ELF_ARCH_MMIX						80
#define ELF_ARCH_HUANY						81
#define ELF_ARCH_PRISM						82
#define ELF_ARCH_AVR						83
#define ELF_ARCH_FR30						84
#define ELF_ARCH_D10V						85
#define ELF_ARCH_D30V						86
#define ELF_ARCH_V850						87
#define ELF_ARCH_M32R						88
#define ELF_ARCH_MN10300					89
#define ELF_ARCH_MN10200					90
#define ELF_ARCH_PJ							91
#define ELF_ARCH_OPENRISC					92
#define ELF_ARCH_ARC_COMPACT				93
#define ELF_ARCH_XTENSA						94
#define ELF_ARCH_VIDEOCORE					95
#define ELF_ARCH_TMM_GPP					96
#define ELF_ARCH_NS32K						97
#define ELF_ARCH_TPC						98
#define ELF_ARCH_SNP1K						99
#define ELF_ARCH_ST200						100
#define ELF_ARCH_IP2K						101
#define ELF_ARCH_MAX						102
#define ELF_ARCH_CR							103
#define ELF_ARCH_F2MC16						104
#define ELF_ARCH_MSP430						105
#define ELF_ARCH_BLACKFIN					106
#define ELF_ARCH_SE_C33						107
#define ELF_ARCH_SEP						108
#define ELF_ARCH_ARCA						109
#define ELF_ARCH_UNICORE					110
#define ELF_ARCH_EXCESS						111
#define ELF_ARCH_DXP						112
#define ELF_ARCH_ALTERA_NIOS2				113
#define ELF_ARCH_CRX						114
#define ELF_ARCH_XGATE						115
#define ELF_ARCH_C166						116
#define ELF_ARCH_M16C						117
#define ELF_ARCH_DSPIC30F					118
#define ELF_ARCH_CE							119
#define ELF_ARCH_M32C						120
#define ELF_ARCH_TSK3000					131
#define ELF_ARCH_RS08						132
#define ELF_ARCH_SHARC						133
#define ELF_ARCH_ECOG2						134
#define ELF_ARCH_SCORE7						135
#define ELF_ARCH_DSP24						136
#define ELF_ARCH_VIDEOCORE3					137
#define ELF_ARCH_LATTICEMICO32				138
#define ELF_ARCH_SE_C17						139
#define ELF_ARCH_TI_C6000					140
#define ELF_ARCH_TI_C2000					141
#define ELF_ARCH_TI_C5500					142
#define ELF_ARCH_MMDSP_PLUS					160
#define ELF_ARCH_CYPRESS_M8C				161
#define ELF_ARCH_R32C						162
#define ELF_ARCH_TRIMEDIA					163
#define ELF_ARCH_QDSP6						164
#define ELF_ARCH_8051						165
#define ELF_ARCH_STXP7X						166
#define ELF_ARCH_NDS32						167
#define ELF_ARCH_ECOG1X						168
#define ELF_ARCH_MAXQ30						169
#define ELF_ARCH_XIMO16						170
#define ELF_ARCH_MANIK						171
#define ELF_ARCH_CRAYNV2					172
#define ELF_ARCH_RX							173
#define ELF_ARCH_METAG						174
#define ELF_ARCH_MCST_ELBRUS				175
#define ELF_ARCH_ECOG16						176
#define ELF_ARCH_CR16						177
#define ELF_ARCH_ETPU						178
#define ELF_ARCH_SLE9X						179
#define ELF_ARCH_L10M						180
#define ELF_ARCH_K10M						181
#define ELF_ARCH_AARCH64					183
#define ELF_ARCH_AVR32						185
#define ELF_ARCH_STM8						186
#define ELF_ARCH_TILE64						187
#define ELF_ARCH_TILEPRO					188
#define ELF_ARCH_MICROBLAZE					189
#define ELF_ARCH_CUDA						190
#define ELF_ARCH_TILEGX						191
#define ELF_ARCH_CLOUDSHIELD				192
#define ELF_ARCH_COREA_1ST					193
#define ELF_ARCH_COREA_2ND					194
#define ELF_ARCH_ARC_COMPACT2				195
#define ELF_ARCH_OPEN8						196
#define ELF_ARCH_RL78						197
#define ELF_ARCH_VIDEOCORE5					198
#define ELF_ARCH_78KOR						199
#define ELF_ARCH_56800EX					200
#define ELF_ARCH_BA1						201
#define ELF_ARCH_BA2						202
#define ELF_ARCH_XCORE						203
#define ELF_ARCH_MCHP_PIC					204

/* ELF symbols */
#define ELF_SYMBOL_TYPE_NOTYPE				0
#define ELF_SYMBOL_TYPE_OBJECT				1
#define ELF_SYMBOL_TYPE_FUNC				2
#define ELF_SYMBOL_TYPE_SECTION				3
#define ELF_SYMBOL_TYPE_FILE				4
#define ELF_SYMBOL_TYPE_COMMON				5
#define ELF_SYMBOL_TYPE_TLS					6
#define ELF_SYMBOL_TYPE_LOOS				10
#define ELF_SYMBOL_TYPE_HIOS				12
#define ELF_SYMBOL_TYPE_LOPROC				13
#define ELF_SYMBOL_TYPE_HIPROC				15

#define ELF_SYMBOL_BIND_LOCAL				0
#define ELF_SYMBOL_BIND_GLOBAL				1
#define ELF_SYMBOL_BIND_WEAK				2
#define ELF_SYMBOL_BIND_LOOS				10
#define ELF_SYMBOL_BIND_HIOS				12
#define ELF_SYMBOL_BIND_LOPROC				13
#define ELF_SYMBOL_BIND_HIPROC				15

#define ELF32_SYMBOL_BIND(i)				((i)>>4)
#define ELF32_SYMBOL_TYPE(i)				((i)&0xf)
#define ELF32_SYMBOL_INFO(b,t)				(((b)<<4)+((t)&0xf))

/* ELF relocations */
#define ELF_R_386_NONE						0
#define ELF_R_386_32						1
#define ELF_R_386_PC32						2
#define ELF_R_386_GOT32						3
#define ELF_R_386_GLT32						4

#define ELF32_R_SYM(i)						((i)>>8)
#define ELF32_R_TYPE(i)						((unsigned char)(i))
#define ELF32_R_INFO(s,t)					(((s)<<8)+(unsigned char)(t))

#endif
