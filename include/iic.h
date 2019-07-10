#ifndef __IIC_H__
#define __IIC_H__

#define NO_END 		0
#define HAVE_END	1

bool iic_init();
void iic_mt_poll(uint8_t dev_addr, uint8_t *data, uint32_t size, uint8_t mode);
void iic_mr_poll(uint8_t dev_addr, uint8_t *data, uint32_t size);

#endif
