#include <types.h>
#include <register.h>
#include <io.h>

typedef struct rtc_regs{
	uint32_t rtccon;	// 0x40
	uint32_t ticnt;		// 0x44
	uint32_t res1;		// 0x48
	uint32_t res2;		// 0x4c
	uint32_t rtcalm;	// 0x50
	uint32_t almsec;	// 0x54
	uint32_t almmin;	// 0x58
	uint32_t almhour;	// 0x5c
	uint32_t almdate;	// 0x60
	uint32_t almmon;	// 0x64
	uint32_t almyear;	// 0x68
	uint32_t res3;		// 0x6c
	uint32_t bcdsec;	// 0x70
	uint32_t bcdmin;	// 0x74
	uint32_t bcdhour;	// 0x78
	uint32_t bcddate;	// 0x7c
	uint32_t bcdday;	// 0x80
	uint32_t bcdmon;	// 0x84
	uint32_t bcdyear;	// 0x88
}rtc, *prtc;

static prtc rtcx;

bool rtc_init(uint32_t year, uint32_t mon, uint32_t date, uint32_t hour, uint32_t min, uint32_t sec, uint32_t day)
{
	uint32_t tmp;
	rtcx = (prtc)RTC_BASE;	
	
	tmp = READD(&rtcx->rtccon);
	tmp |= 0x01;
	WRITED(&rtcx->rtccon, tmp);
	
	WRITED(&rtcx->bcdsec,	sec);
	WRITED(&rtcx->bcdmin, 	min);
	WRITED(&rtcx->bcdhour, 	hour);
	WRITED(&rtcx->bcddate, 	date);
	WRITED(&rtcx->bcdday, 	day);
	WRITED(&rtcx->bcdmon, 	mon);
	WRITED(&rtcx->bcdyear, 	year);
	
	return(TRUE);
}

void rtc_read(uint8_t *buf)
{
	*buf++ = READD(&rtcx->bcdyear);
	*buf++ = READD(&rtcx->bcdmon);
	*buf++ = READD(&rtcx->bcddate);
	*buf++ = READD(&rtcx->bcdhour);
	*buf++ = READD(&rtcx->bcdmin);
	*buf++ = READD(&rtcx->bcdsec);
	*buf++ = READD(&rtcx->bcdday);
}
