#include <types.h>
#include <register.h>
#include <io.h>
#include <gpio.h>
#include <timer.h>

void buzzer_start(uint32_t fre)
{
	// GPIO init.
	WRITED(GPB_BASE+GPXCON_OFFSET, (READD(GPB_BASE+GPXCON_OFFSET) & ~(0x03 << 0)) | (0x02 << 0));
	WRITED(GPB_BASE+GPXUP_OFFSET, READD(GPB_BASE+GPXUP_OFFSET) & ~BIT0);

	// timer init.
	while(!timer_init(TIMER0_BASE, AUTO_RELOAD, 50, 10, 1000000 / (fre * 10)));
}

void buzzer_stop()
{
	timer_stop(TIMER0_BASE);
	gpio_set_mode_bit(GPB_BASE, BIT0, OUTPUT_MODE);
	gpio_set_pullup_bit(GPB_BASE, BIT0, DIS);
	gpio_write_bit(GPB_BASE, BIT0, OFF);
}
