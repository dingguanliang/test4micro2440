#ifndef __NAND_H__
#define __NAND_H__

#define BLOCK_NUM				2048
#define BLOCK_SIZE				((128 + 4) * 1024)
#define PAGE_NUM				(BLOCK_NUM * 64)
#define PAGE_SIZE				(MAIN_FIELD_SIZE + SPARE_FIELD_SIZE)
#define MAIN_FIELD_SIZE			2048
#define	SPARE_FIELD_SIZE		64
#define	NAND_ID_SIZE			5

extern uint8_t badblocktab[];

void nand_read_id(uint8_t *buf);
bool nand_read_2nd(uint32_t addr, uint8_t *buf, uint16_t size);
bool nand_erase(uint32_t addr);
bool nand_write(uint32_t addr, uint8_t *buf, uint16_t size);
bool nand_badblock_scan();
bool nand_init();

#endif
