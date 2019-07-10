#include <types.h>
#include <register.h>
#include <gpio.h>

#define	LED0	 GPB	

bool led_init()
{	
	gpio_set_mode_bit(LED0, BIT5, OUTPUT_MODE);
	gpio_set_mode_bit(LED0, BIT6, OUTPUT_MODE);
	gpio_set_mode_bit(LED0, BIT7, OUTPUT_MODE);
	gpio_set_mode_bit(LED0, BIT8, OUTPUT_MODE);
	gpio_set_pullup_bit(LED0, BIT5, DIS);
	gpio_set_pullup_bit(LED0, BIT6, DIS);
	gpio_set_pullup_bit(LED0, BIT7, DIS);
	gpio_set_pullup_bit(LED0, BIT8, DIS);
	
	return(TRUE);
}

void led_set(uint32_t ledx, uint8_t mode)
{
	gpio_write_bit(LED0, ledx, mode);
}
