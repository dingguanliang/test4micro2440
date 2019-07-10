#include <types.h>
#include <register.h>
#include <io.h>
#include <config.h>

bool wt_init(uint8_t mode, uint32_t time, uint16_t cnt)
{
	uint8_t i;
	const uint8_t div[4] = {16, 32, 64, 128};
	uint32_t pre, tmp;
	
	tmp = READD(WTCON);
	if(0 == mode)
		WRITED(WTCON, READD(WTCON) & ~BIT5);
	else
	{
		for(i = 0; i < 4; i++)
		{
			pre = (uint32_t)((time * (PCLK / 1000000)) / div[i]);
			if(pre > 256)
				continue;
			else if(pre < 1)
				return(FALSE);
			else 
				break;
		}
		tmp = (tmp & ~(BIT3 | BIT4)) & 0x00ff;
		tmp = ((tmp | (i << 3)) | ((pre - 1) << 8)) | BIT5;
		if(1 == mode)
		{
			WRITED(WTDAT, cnt);
			WRITED(WTCON, ~BIT0 & (BIT2 | tmp));
		}
		else if(2 == mode)
		{
			WRITED(WTCNT, cnt);
			WRITED(WTCON, BIT0 | (~BIT2 & tmp));
		}
		else
			return(FALSE);
	}
	
	return(TRUE);
}

inline void wt_feeddog(uint16_t food)
{
	WRITED(WTCNT, food);
}
