#ifndef __LED_H__
#define __LED_H__

// define led.
#define LED_1	(0x01 << 5)
#define LED_2	(0x01 << 6)
#define LED_3	(0x01 << 7)
#define LED_4	(0x01 << 8)

// define led mode.
#define ON	0
#define OFF	1 

bool led_init();
void led_set(uint32_t leds, uint8_t mode);

#endif
