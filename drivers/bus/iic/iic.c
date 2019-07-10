#include <types.h>
#include <register.h>
#include <io.h>

#define START_MT() 			WRITED(IICSTAT, (0x01 << 4) | (0x01 << 5) | (0x03 << 6))
#define STOP_MT()			WRITED(IICSTAT, (0x01 << 4) | (0x00 << 5) | (0x03 << 6))
#define START_MR() 			WRITED(IICSTAT, (0x01 << 4) | (0x01 << 5) | (0x02 << 6))
#define STOP_MR()			WRITED(IICSTAT, (0x01 << 4) | (0x00 << 5) | (0x02 << 6))
#define WRITE_ADDR(addr)	WRITED(IICDS, addr)
#define WRITE_DATA(data)	WRITED(IICDS, data)
#define READ_DATA()			READD(IICDS)
#define HAVE_ACK()			WRITED(IICCON, READD(IICCON) | BIT7)
#define NO_ACK()			WRITED(IICCON, READD(IICCON) & ~BIT7)
#define CHECK_INT()			(READD(IICCON) & BIT4)
#define CLEAR_INT()			WRITED(IICCON, READD(IICCON) & ~BIT4)
#define CHECK_BUSY()		(READD(IICSTAT) & BIT5)

bool iic_init()
{
	uint32_t tmp;

	// GPIO init.
	tmp = READD(GPE_BASE+GPXCON_OFFSET);
	tmp = (tmp & ~(0x0f << 28)) | (0x0a << 28);
	WRITED(GPE_BASE+GPXCON_OFFSET, tmp);

	tmp = READD(GPE_BASE+GPXUP_OFFSET);
	tmp |= (0x03 << 14);
	WRITED(GPE_BASE+GPXUP_OFFSET, tmp);
	
	// IICCON init.
	WRITED(IICCON, (0x0f << 0) | (0x01 << 5));

	// IICLC init.
	WRITED(IICLC, (0x01 << 2));
	
	return(TRUE);
}

void iic_mt_poll(uint8_t dev_addr, uint8_t *data, uint32_t size, uint8_t mode)
{	
	HAVE_ACK();
	
	WRITE_ADDR(dev_addr);
	START_MT();
	CLEAR_INT();
	while(!CHECK_INT());
	
	while(size--)
	{
		WRITE_DATA(*data++);
		CLEAR_INT();
		while(!CHECK_INT());
	}
	
	if(1 == mode)
	{
		STOP_MT();
		CLEAR_INT();
		while(CHECK_BUSY());
	}
}

void iic_mr_poll(uint8_t dev_addr, uint8_t *data, uint32_t size)
{
	HAVE_ACK();
	
	WRITE_ADDR(dev_addr);
	START_MR();
	CLEAR_INT();
	while(!CHECK_INT());
	
	while(size--)
	{	
		if(1 == size)
			NO_ACK();
		*data++ = READ_DATA();
		CLEAR_INT();
		while(!CHECK_INT());
	}
	
	STOP_MR();
	CLEAR_INT();
	while(CHECK_BUSY());
}
