#include <types.h>
#include <register.h>
#include <io.h>

// nand flash chip (K9F2G08U0B) parameter.
#define BLOCK_NUM				2048
#define BLOCK_SIZE				((128 + 4) * 1024)
#define PAGE_NUM				(BLOCK_NUM * 64)
#define PAGE_SIZE				(MAIN_FIELD_SIZE + SPARE_FIELD_SIZE)
#define MAIN_FIELD_SIZE			2048
#define	SPARE_FIELD_SIZE		64
#define	NAND_ID_SIZE			5

// nand flash chip (K9F2G08U0B) command.
#define READ_1ST				0x00
#define READ_2ND				0x30
#define READ_FOR_COPY_BACK_1ST	0x00
#define READ_FOR_COPY_BACK_2ND	0x35
#define READ_ID					0x90
#define RESET					0xff
#define PAGE_PROGRAM_1ST		0x80
#define PAGE_PROGRAM_2ND		0x10
#define BLOCK_ERASE_1ST			0x60
#define BLOCK_ERASE_2ND			0xd0
#define READ_STATUS				0x70
#define READ_STATUS_2			0xf1

// nand flash chip (K9F2G08U0B) status.
#define BUSY	0
#define READY	1

#define NAND_IS_BADBLOCK(addr)		(badblocktab[addr >> 18])
#define READ_IO0()	 	 			(READB(NFDATA) & BIT0)
#define NAND_CHIP_SELECT()			(WRITED(NFCONT, READD(NFCONT) & ~BIT1))
#define NAND_CHIP_UNSELECT()		(WRITED(NFCONT, READD(NFCONT) | BIT1))
#define NAND_RNB_CHECK()			((READD(NFSTAT) & BIT0)?READY:BUSY)

#define NAND_RESET() \
do \
{ \
	WRITED(NFCMMD, RESET); \
	while(!NAND_RNB_CHECK()); \
}while(0)

#define NAND_READ_DATA(buf, size) ({ \
	uint16_t cnt = size; \
	while(cnt--) \
		*buf++ = READB(NFDATA); \
	TRUE; \
})

#define NAND_WRITE_DATA(buf, size) ({ \
	while(size--) \
		WRITEB(NFDATA, *buf++); \
	TRUE; \
})

uint8_t badblocktab[BLOCK_NUM] = {0};

// nand flash read id.
void nand_read_id(uint8_t *buf)
{
	NAND_CHIP_SELECT();
	while(!NAND_RNB_CHECK());
	WRITED(NFCMMD, READ_ID);
	WRITED(NFADDR, 0x00);
	while(!NAND_READ_DATA(buf, NAND_ID_SIZE));
	NAND_CHIP_UNSELECT();
}

// nand flash read whitout ecc.
inline void nand_read(uint32_t addr, uint8_t *buf, uint16_t size)
{
	NAND_CHIP_SELECT();
	while(!NAND_RNB_CHECK());
	WRITED(NFCMMD, READ_1ST);
	WRITED(NFADDR, (addr >> 0)	& 0xff);
	WRITED(NFADDR, (addr >> 8)	& 0x0f);
	WRITED(NFADDR, (addr >> 12)	& 0xff);
	WRITED(NFADDR, (addr >> 20) & 0xff);
	WRITED(NFADDR, (addr >> 28) & 0x01);
	WRITED(NFCMMD, READ_2ND);
	while(!NAND_RNB_CHECK());
	while(!NAND_READ_DATA(buf, size));	
	NAND_CHIP_UNSELECT();
}

// nand flash read for fast read.
bool nand_read_2nd(uint32_t addr, uint8_t *buf, uint16_t size)
{
	if(!NAND_IS_BADBLOCK(addr))
	{
		nand_read(addr, buf, size);

		return(TRUE);
	}
	else
		return(FALSE);
}

// nand flash erase block.
bool nand_erase(uint32_t addr)
{
	if(!NAND_IS_BADBLOCK(addr))
	{
		NAND_CHIP_SELECT();
		while(!NAND_RNB_CHECK());
		WRITED(NFCMMD, BLOCK_ERASE_1ST);
		WRITED(NFADDR, (addr >> 12)	& 0xff);
		WRITED(NFADDR, (addr >> 20) & 0xff);
		WRITED(NFADDR, (addr >> 28) & 0x01);
		WRITED(NFCMMD, BLOCK_ERASE_2ND);
		while(!NAND_RNB_CHECK());
		if(!READ_IO0())
		{
			NAND_CHIP_UNSELECT();
			return(TRUE);
		}
		else
		{
			NAND_CHIP_UNSELECT();
			return(FALSE);
		}
	}
	else
		return(FALSE);
}

// nand flash write.
bool nand_write(uint32_t addr, uint8_t *buf, uint16_t size)
{
	if(!NAND_IS_BADBLOCK(addr))
	{
		NAND_CHIP_SELECT();
		while(!NAND_RNB_CHECK());
		WRITED(NFCMMD, PAGE_PROGRAM_1ST);
		WRITED(NFADDR, (addr >> 0)	& 0xff);
		WRITED(NFADDR, (addr >> 8)	& 0x0f);
		WRITED(NFADDR, (addr >> 12)	& 0xff);
		WRITED(NFADDR, (addr >> 20) & 0xff);
		WRITED(NFADDR, (addr >> 28) & 0x01);
		NAND_WRITE_DATA(buf, size);
		WRITED(NFCMMD, PAGE_PROGRAM_2ND);
		while(!NAND_RNB_CHECK());
		if(!READ_IO0())
		{
			NAND_CHIP_UNSELECT();
			return(TRUE);
		}
		else
		{
			NAND_CHIP_UNSELECT();
			return(FALSE);
		}
	}
	else
		return(FALSE);
}

// nand flash bad block check.
inline bool nand_badblock_check(uint32_t block_addr)
{
	uint8_t buf[2];
	
	nand_read((block_addr << 18) + 2048, &buf[0], 1);
	nand_read((block_addr << 18) + (1 << 12) + 2048, &buf[1], 1);
	if((0xff != buf[0]) || (0xff != buf[1]))
		return(FALSE);
		
	return(TRUE);
}

// create a bad block table, initial step2.
bool nand_badblock_scan()
{
	
	uint32_t block_addr;
	
	for(block_addr = 0; block_addr < BLOCK_NUM; block_addr++)
	{	
		if(!nand_badblock_check(block_addr))
			badblocktab[block_addr] = 0x01;
	}
	
	return(TRUE);
}

// nand flash initial, initial step1.
bool nand_init()
{
	uint32_t tmp;
	
	// initial GPIO.
	tmp = READD(GPA_BASE+GPXCON_OFFSET);
	tmp |= 0x2f << 17;
	WRITED(GPA_BASE+GPXCON_OFFSET, tmp);
	
	// nand flash contraller initial.
	tmp = READD(NFCONF);
	tmp |= (2 << 12) | (1 << 8) | (1 << 4);
	WRITED(NFCONF, tmp);
	
	tmp = READD(NFCONT);
	tmp |= (1 << 6) | (1 << 5) | (1 << 1) | (1 << 0);
	WRITED(NFCONT, tmp);
	
	NAND_CHIP_SELECT();
	NAND_RESET();
	NAND_CHIP_UNSELECT();

	return(TRUE);
}
