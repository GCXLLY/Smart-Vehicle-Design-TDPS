#ifndef __Z_UTIL_TIME_H
#define __Z_UTIL_TIME_H
#include "z_util_time.h"
#endif
 
void delay_us(uint32_t time_us)
{
	SysTick->LOAD = AHB_INPUT * time_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x00000005;
	for(;!(SysTick->CTRL & 0x00010000););
	SysTick->CTRL = 0x00000004;
}
 
void delay_ms(uint32_t time_ms)
{
	for(;time_ms-- > 0;)
	{
		delay_us(1000);
	}
}
