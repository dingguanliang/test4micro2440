#include <types.h>
#include <led.h>
#include <buzzer.h>
#include <at24c08.h>
#include <dma.h>
#include <uart.h>
#include <rtc.h>
#include <wdt.h>
#include <adc.h>
#include <nand.h>
#include <stdio.h>

void led_test()
{
	uint8_t leds[4] = {1, 0, 0, 1};
	 
	puts("#### led test ####\n\r");
	led_set(LED_1, !leds[0]);
	led_set(LED_2, !leds[1]);
	led_set(LED_3, !leds[2]);
	led_set(LED_4, !leds[3]);
	printf("led stat:%1d%1d%1d%1d\n\r", leds[3], leds[2], leds[1], leds[0]);
}

void buzzer_test()
{
	puts("#### buzzer test ####\n\r");
	buzzer_start(1000);
	puts("input any key to stop!\n\r");
	getc();
	buzzer_stop();
}

void at24c08_test()
{
	uint16_t i;
	uint8_t buf[256];

	puts("#### eeprom test ####\n\r");
	for(i = 0; i < 256; i++)
		at24c08_writeb(i, i);
	puts("eeprom write ok!\n\r");
	for(i = 0; i < 256; i++)
		buf[i] = at24c08_readb(i);
	puts("read data:\n\r");
	printf_matrix(16, 256, buf);
}

void dma_test()
{
	puts("#### dma test ####\n\r");
	printf_matrix(16, 256, (uint8_t *)0x33000000);
}

void uart_test()
{
	uint8_t buf[32];
	
	puts("#### uart test ####\n\r");
	puts("send:");
	scanf(buf);
	printf("\n\rrev:%s\n\r", buf);
}

void rtc_test()
{
	uint8_t buf[128];
	
	puts("#### rtc test ####\n\r");
	rtc_read(buf);
	printf("now buf is 20%2x-%2x-%2x %2x:%2x:%2x week:%1x\n\r", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
}

void wdt_test()
{
	puts("#### wdt test ####\n\r");
	wt_feeddog(DOG_FOOD);
	puts("feed dog ok!\n\r");
}

void adc_test()
{
	puts("#### adc test ####\n\r");
	printf("adc:%d mV\n\r", adc_read(AIN3));
}

void nand_test()
{
	uint8_t buf[PAGE_SIZE];
	
	puts("#### nand test ####\n\r");
	nand_read_id(buf);
	printf("nand id = 0x%x-0x%x-0x%x-0x%x-0x%x\n\r", buf[0], buf[1], buf[2], buf[3], buf[4]);
	puts("bad block table[] = \n\r");
	printf_matrix(16, BLOCK_NUM, badblocktab);
	// nand_erase(0x00);
	// printf("erase block %d ok!\n\r", 0x00 >> 18);
	// nand_write(0x00, buf, 10);
	// printf("write page %d ok!\n\r", 0x00 >> 12);
	nand_read_2nd(0x00, buf, MAIN_FIELD_SIZE);
	printf("read page %d ok!\n\r", 0x00 >> 12);
	printf_matrix(16, MAIN_FIELD_SIZE, buf);
}
