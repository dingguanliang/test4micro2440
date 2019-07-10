#ifndef __GPIO_H__
#define __GPIO_H__

#define INPUT_MODE	0x00		
#define OUTPUT_MODE	0x01
#define OTHER_MODE	0x02

#define EN	0
#define DIS	1

#define OFF	0
#define ON	1

inline bool gpio_set_mode_bit(uint32_t gpiox_base, uint32_t gpiox_bit, uint8_t gpiox_mode);
inline bool gpio_set_pullup_bit(uint32_t gpiox_base, uint32_t gpiox_bit, uint8_t gpiox_pullup);
inline bool gpio_set_mode_dw(uint32_t gpiox_base, uint32_t gpiox_dw);
inline bool gpio_set_pullup_dw(uint32_t gpiox_base, uint32_t gpiox_dw);
inline int32_t gpio_read_bit(uint32_t gpiox_base, uint32_t gpiox_bit);
inline void gpio_write_bit(uint32_t gpiox_base, uint32_t gpiox_bit, uint8_t gpiox_sw);
inline int32_t gpio_read_dw(uint32_t gpiox_base);
inline void gpio_write_dw(uint32_t gpiox_base, uint32_t gpiox_dw);

#endif
