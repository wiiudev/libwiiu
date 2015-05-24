#ifndef PACKET_H
#define PACKET_H

/* Command Block Wrapper (CBW) */
typedef struct cbw
{
	uint32_t dCBWSignature;
	uint32_t dCBWTag;
	uint32_t dCBWDataTransferLength;
	uint8_t bmCBWFlags;
	uint8_t bCBWLUN;
	uint8_t bCBWBLength;
	char CBWCB[0x1e-0xe];
} __attribute__((packed));

/* Command Status Wrapper (CSW) */
typedef struct csw
{
	uint32_t dCSWSignature;
	uint32_t dCSWTag;
	uint32_t dCSWDataResidue;
	uint8_t bCSWStatus;
} __attribute__((packed));

#endif
