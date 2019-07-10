#include <types.h>
#include <register.h>
#include <io.h>

#define PRE	(3300 / 1024)

#define ADC_START()		WRITED(ADCCON, READD(ADCCON) | (0x01 << 0))
#define CHECK_START()	(READD(ADCCON) & (0x01 << 0))
#define CHECK_OVER()	(READD(ADCCON) & (0x01 << 15))

inline uint16_t adc_read(uint8_t chx)	
{
	WRITED(ADCCON, (chx << 3) | (0x22 << 6) | (0x01 << 14));
	
	ADC_START();
	while(CHECK_START());
	while(!CHECK_OVER());
	
	return(PRE * (READD(ADCDAT0) & 0x03ff));
}
