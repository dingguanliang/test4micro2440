#include <types.h>
#include <register.h>
#include <io.h>
#include <led.h>
#include <at24c08.h>
#include <button.h>
#include <uart.h>
#include <dma.h>
#include <rtc.h>
#include <wdt.h>
#include <nand.h>

extern void main();

// 2nd step enter function.
void start_armboot(void)
{
	while(!led_init());
	while(!at24c08_init());
	while(!button_init());
	while(!uart_init(UART0, 115200, NOMAL_MODE, NP, OSB, _8BIT));
	while(!dma_init(NO_TYPE, SF_MODE, 0x31000000, 0x33000000, 256));
	while(!rtc_init(0x14, 0x11, 0x24, 0x22, 0x39, 0x00, 0x01));
	//while(!wt_init(WATCHDOG_MODE, WDT_TIME, DOG_FOOD));
	if(READD(BWSCON) & 0x06)
		while(!nand_init());
	while(!nand_badblock_scan());
	
	while(1)
		main();
}
