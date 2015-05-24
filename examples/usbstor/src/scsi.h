#ifndef SCSI_H
#define SCSI_H

/* Operation codes */
#define OP_READ16	0x88
#define OP_WRITE16	0x8A

/* READ(16) command */
typedef struct write16_cmd
{
	uint8_t op_code;
	uint8_t flags;
	uint64_t start_lba;
	uint32_t length;
	uint8_t group_num;
	uint8_t control;
} __attribute__((packed));

/* WRITE(16) command */
typedef struct write16_cmd
{
	uint8_t op_code;
	uint8_t flags;
	uint64_t start_lba;
	uint32_t length;
	uint8_t group_num;
	uint8_t control;
} __attribute__((packed));

#endif
