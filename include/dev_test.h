#ifndef __DEV_TEST_H__
#define __DEV_TEST_H__

typedef void (*func)();

void led_test();
void buzzer_test();
void at24c08_test();
void dma_test();
void uart_test();
void rtc_test();
void wdt_test();
void adc_test();
void nand_test();

struct test{
	func test_func;
	uint8_t *head;
}test_t[] = {
				{(func)led_test, 		(uint8_t *)"led test."},
				{(func)buzzer_test, 	(uint8_t *)"buzzer test."},
				{(func)at24c08_test, 	(uint8_t *)"at24c08 test."},
				{(func)dma_test, 		(uint8_t *)"dma test."},
				{(func)uart_test, 		(uint8_t *)"uart test."},
				{(func)rtc_test,	 	(uint8_t *)"rtc test."},
				{(func)wdt_test, 		(uint8_t *)"wdt test."},
				{(func)adc_test, 		(uint8_t *)"adc test."},
				{(func)nand_test, 		(uint8_t *)"nand test."},	
				{NULL, NULL},					
			};

#endif
