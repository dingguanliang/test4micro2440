#include <types.h>
#include <io.h>

typedef struct gpio_regs{
	uint32_t gpcon;
	uint32_t gpdat;
	uint32_t gpup;
	uint32_t res;
}gpio, *pgpio;

static pgpio gpiox;

inline bool gpio_set_mode_bit(uint32_t gpiox_base, uint32_t gpiox_bit, uint8_t gpiox_mode)
{
	uint32_t tmp1, tmp2;
	gpiox = (pgpio)gpiox_base;
	
	tmp1 = gpiox_bit * gpiox_bit;
	tmp2 = READD(&gpiox->gpcon);
	if(0 == gpiox_mode)
		tmp2 = (~tmp1 & tmp2) & ~(tmp1 << 1);
	else if(1 == gpiox_mode)
		tmp2 = (tmp1 | tmp2) & ~(tmp1 << 1);
	else if(2 == gpiox_mode)
		tmp2 = (~tmp1 & tmp2) | (tmp1 << 1);
	else
		return(FALSE);
	WRITED(&gpiox->gpcon, tmp2);
	
	return(TRUE);
}

inline bool gpio_set_pullup_bit(uint32_t gpiox_base, uint32_t gpiox_bit, uint8_t gpiox_pullup)
{
	uint32_t tmp;
	gpiox = (pgpio)gpiox_base;
	
	tmp = READD(&gpiox->gpup);
	if(1 == gpiox_pullup)
		tmp |= gpiox_bit;
	else
		tmp &= ~gpiox_bit;
	WRITED(&gpiox->gpup, tmp);
	
	return(TRUE);
}

inline bool gpio_set_mode_dw(uint32_t gpiox_base, uint32_t gpiox_dw)
{
	gpiox = (pgpio)gpiox_base;
	
	WRITED(&gpiox->gpcon, gpiox_dw);
	
	return(TRUE);
}

inline bool gpio_set_pullup_dw(uint32_t gpiox_base, uint32_t gpiox_dw)
{
	gpiox = (pgpio)gpiox_base;
	
	WRITED(&gpiox->gpup, gpiox_dw);
	
	return(TRUE);
}

inline int32_t gpio_read_bit(uint32_t gpiox_base, uint32_t gpiox_bit)
{
	uint32_t tmp;
	gpiox = (pgpio)gpiox_base;
	
	tmp = READD(&gpiox->gpdat);
	tmp |= gpiox_bit;
	
	return(tmp);
}

inline void gpio_write_bit(uint32_t gpiox_base, uint32_t gpiox_bit, uint8_t gpiox_sw)
{
	uint32_t tmp;
	gpiox = (pgpio)gpiox_base;
	
	tmp = READD(&gpiox->gpdat);
	if(1 == gpiox_sw)
		tmp |= gpiox_bit;
	else
		tmp &= ~gpiox_bit;
	WRITED(&gpiox->gpdat, tmp);
}

inline int32_t gpio_read_dw(uint32_t gpiox_base)
{
	uint32_t tmp;
	gpiox = (pgpio)gpiox_base;
	
	tmp = READD(&gpiox->gpdat);
	
	return(tmp);
}

inline void gpio_write_dw(uint32_t gpiox_base, uint32_t gpiox_dw)
{
	gpiox = (pgpio)gpiox_base;
	
	WRITED(&gpiox->gpdat, gpiox_dw);
}
