#ifndef __WDT_H__
#define __WDT_H__

#define WDT_OFF			0
#define TIMER_MODE		1
#define WATCHDOG_MODE	2

#define WDT_TIME		100
#define DOG_FOOD		30000

bool wt_init(uint8_t mode, uint32_t time, uint16_t cnt);
void wt_feeddog(uint16_t food);

#endif
