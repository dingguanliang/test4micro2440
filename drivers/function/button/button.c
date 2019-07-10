#include <types.h>
#include <register.h>
#include <io.h>
#include <interrupt.h>
#include <timer.h>
#include <stdio.h>

static uint8_t button;

void timer1_svr()
{
	printf("\n\rext%d handle...\n\r", button);
}

void ext8_svr()
{
	button = 8;
	while(!timer_init(TIMER1_BASE, ONE_SHOT, 0, 10, 10000));
}

void ext11_svr()
{
	button = 11;
	while(!timer_init(TIMER1_BASE, ONE_SHOT, 0, 10, 10000));
}

void ext13_svr()
{
	button = 13;
	while(!timer_init(TIMER1_BASE, ONE_SHOT, 0, 10, 10000));
}

void ext14_svr()
{
	button = 14;
	while(!timer_init(TIMER1_BASE, ONE_SHOT, 0, 10, 10000));
}

void ext15_svr()
{
	button = 15;
	while(!timer_init(TIMER1_BASE, ONE_SHOT, 0, 10, 10000));
}

void ext19_svr()
{
	button = 19;
	while(!timer_init(TIMER1_BASE, ONE_SHOT, 0, 10, 10000));
}

bool button_init()
{
	uint32_t tmp;
	
	// GPIO init.
	tmp = READD(GPG_BASE+GPXCON_OFFSET);
	tmp &= ~((0x03 << 0) | (0x03 << 6) | (0x03 << 10) | (0x03 << 12) | (0x03 << 14) | (0x03 << 22));
	tmp |= (0x02 << 0) | (0x02 << 6) | (0x02 << 10) | (0x02 << 12) | (0x02 << 14) | (0x02 << 22);
	WRITED(GPG_BASE+GPXCON_OFFSET, tmp);
	
	tmp = READD(EXTINT1);
	tmp &= ~((0x0f << 0) | (0x0f << 12) | (0x0f << 20) | (0x0f << 24) | (0x0f << 28));
	tmp |= (0x04 << 0) | (0x04 << 12) | (0x04 << 20) | (0x04 << 24) | (0x04 << 28);
	WRITED(EXTINT1, tmp);

	tmp = READD(EXTINT2);
	tmp &= ~(0x0f << 12);
	tmp |= (0x04 << 12);
	WRITED(EXTINT2, tmp);

	while(!int_init(EINT8,		ext8_svr));
	while(!int_init(EINT11,	ext11_svr));
	while(!int_init(EINT13,	ext13_svr));
	while(!int_init(EINT14,	ext14_svr));
	while(!int_init(EINT15,	ext15_svr));
	while(!int_init(EINT19,	ext19_svr));
	
	while(!int_init(INT_TIMER1, timer1_svr));

	return(TRUE);
}
