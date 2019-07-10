#include <types.h>
#include <iic.h>

bool at24c08_init()
{
	while(!iic_init());

	return(TRUE);
}

void at24c08_writeb(uint16_t addr, const uint8_t ch)
{
	uint8_t dev_addr, data[2];
	
	dev_addr = 0xa0 | ((addr & 0x300) >> 7);
	data[0] = addr & 0xff;
	data[1] = ch;
	
	iic_mt_poll(dev_addr, data, 2, HAVE_END);
}

uint8_t at24c08_readb(uint16_t addr)
{	
	uint8_t dev_addr, data_addr, data[2];
	
	dev_addr = 0xa0 | ((addr & 0x300) >> 7);
	data_addr = addr & 0xff;
	
	iic_mt_poll(dev_addr, &data_addr, 1, NO_END);
	iic_mr_poll(dev_addr, data, 2);

	return(data[1]);
}
