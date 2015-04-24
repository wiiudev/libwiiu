#ifndef ELF_STRUCTS_H
#define ELF_STRUCTS_H

/* ELF program header */
typedef struct elf_program_header
{
	uint32_t type;
	uint32_t offset;
	uint32_t virtual_address;
	uint32_t physical_address;
	uint32_t file_size;
	uint32_t mem_size;
	uint32_t flags;
	uint32_t align;
} __attribute__((packed)) elf_program_header_t;

/* ELF section header */
typedef struct elf_section_header
{
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t address;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t align;
	uint32_t subentry_size;
} __attribute__((packed)) elf_section_header_t;

/* ELF file structure */
typedef struct elf_header
{
	uint8_t magic[4];
	uint8_t elf_class;
	uint8_t encoding;
	uint8_t version;
	uint8_t pad[9];
	uint16_t type;
	uint16_t machine;
	uint32_t obj_file_version;
	uint32_t entry_point;
	uint32_t program_header_table_offset;
	uint32_t section_header_table_offset;
	uint32_t flags;
	uint16_t header_size;
	uint16_t program_header_size;
	uint16_t num_program_headers;
	uint16_t section_header_size;
	uint16_t num_section_headers;
	uint16_t section_string_table_index;
} __attribute__((packed)) elf_header_t;

/* ELF symtab entry */
typedef struct elf_symbol
{
	uint32_t name;
	uint32_t value;
	uint32_t size;
	uint8_t info;
	uint8_t other;
	uint16_t section_index;
} __attribute__((packed)) elf_symbol_t;

/* ELF rel entry */
typedef struct elf_rel32
{
	uint32_t offset;
	uint32_t info;
} __attribute__((packed)) elf_rel32_t;

#endif
