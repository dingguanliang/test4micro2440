#include <types.h>
#include <register.h>
#include <io.h>

typedef struct dma_regs{
	uint32_t disrc;		// 0x00.
	uint32_t disrcc;	// 0x04.
	uint32_t didst;		// 0x08.
	uint32_t didstc;	// 0x0c.
	uint32_t dcon;		// 0x10.
	uint32_t dstat;		// 0x14.
	uint32_t dcsrc;		// 0x18.
	uint32_t dcdst;		// 0x1c.
	uint32_t dmasktrig;	// 0x20.
}dma, *pdma;

static pdma dmax;

static inline void dma_start_sf()
{
	uint32_t tmp;
	
	tmp = READD(&dmax->dmasktrig);
	tmp |= (0x01 << 0) | (0x01 << 1);
	WRITED(&dmax->dmasktrig, tmp);
}

inline void dma_stop_sf()
{
	uint32_t tmp;
	
	tmp = READD(&dmax->dmasktrig);
	tmp |= (0x01 << 2);
	WRITED(&dmax->dmasktrig, tmp);
}

bool dma_init(uint8_t src_dev, uint8_t mode, uint32_t src_addr, uint32_t des_addr, uint32_t data_size)
{
	uint32_t tmp;
	
	switch(src_dev & 0xf0)
	{
		case 0x00:
			dmax = (pdma)DMA0_BASE;
			break;
		case 0x10:
			dmax = (pdma)DMA1_BASE;
			break;
		case 0x20:
			dmax = (pdma)DMA2_BASE;
			break;
		case 0x30:
			dmax = (pdma)DMA3_BASE;
			break;
		default:
			return(FALSE);
	};
		
	if(0x00 == (READD(&dmax->dstat) & (0x03 << 20)))
	{
		// source init.
		WRITED(&dmax->disrc, src_addr);
		
		// destination init.
		WRITED(&dmax->didst, des_addr);
		
		// dma controller init.
		tmp = READD(&dmax->dcon);
		tmp |= (data_size << 0) | (0x01 << 22) | (mode << 23) | ((src_dev & 0x0f) << 24) | (0x01 << 27) | (0x01 << 29);
		WRITED(&dmax->dcon, tmp);
		
		if(0x00 == mode)
			dma_start_sf();
		
		return(TRUE);
	}
	
	return(FALSE);
}
