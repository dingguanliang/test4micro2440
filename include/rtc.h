#ifndef	__RTC_H__
#define	__RTC_H__

bool rtc_init(uint32_t year, uint32_t mon, uint32_t date, uint32_t hour, uint32_t min, uint32_t sec, uint32_t day);
void rtc_read(uint8_t *buf);

#endif
