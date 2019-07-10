#ifndef __DMA_H__
#define __DMA_H__

#define NO_TYPE		0X00

#define NXDREQ0		0x00
#define UART0_CH0	0x01
#define SDI_CH0		0x02
#define TIMER_CH0 	0x03
#define USB_EP1		0x04
#define I2SSDO_CH0	0x05
#define PCMIN_CH0	0x06

#define NXDREQ1		0x10
#define UART1_CH1	0x11
#define I2SSDI_CH1	0x12
#define SPI0 		0x13
#define USB_EP2		0x14
#define PCMOUT_CH1	0x15
#define SDI_CH1		0x16

#define I2SSDO_CH2	0x20
#define I2SSDI_CH2 	0x21
#define SDI_CH2		0x22
#define TIMER_CH2 	0x23
#define USB_EP3		0x24
#define PCMIN_CH2	0x25
#define MICIN_CH2	0x26

#define UART2_CH3	0x30
#define SDI_CH3 	0x31
#define SPI1 		0x32
#define TIMER_CH3 	0x33
#define USB_EP4		0x34
#define MICIN_CH3	0x35
#define PCMOUT_CH3	0x36

#define SF_MODE		0x00
#define HW_MODE		0x01

bool dma_init(uint8_t src_dev, uint8_t mode, uint32_t src_addr, uint32_t des_addr, uint32_t data_size);

#endif
