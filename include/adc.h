#ifndef __ADC_H__
#define __ADC_H__

#define AIN0	0x00
#define AIN1	0x01
#define AIN2	0x02
#define AIN3	0x03
#define YM		0x04
#define YP		0x05
#define XM		0x06
#define XP		0x07

inline uint16_t adc_read(uint8_t chx);

#endif
