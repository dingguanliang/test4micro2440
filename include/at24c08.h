#ifndef __AT24C08_H__
#define __AT24C08_H__

bool at24c08_init();
void at24c08_writeb(uint16_t addr, const uint8_t ch);
uint8_t at24c08_readb(uint16_t addr);

#endif
