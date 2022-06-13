#ifndef __Z_OUTER_HARDWARE_SUPERSONIC_H
#define __Z_OUTER_HARDWARE_SUPERSONIC_H
#include "z_outer_hardware_supersonic.h"
#include "delay.h"
#endif
 
void init_hardware_supersonic()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 
void func_trig_supersonic()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	delay_us(12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
}
void func_trig_supersonic2()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	delay_us(12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void func_trig_supersonic3()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	delay_us(12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
}
 
u8 func_get_supersonic_echo_signal(u16 *distance_cm)
{
	u16 counter_high=0;
	u16 timeout=0;
	
	func_trig_supersonic();
	for(;;)
	{
		delay_us(1);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == Bit_SET)
		{
			counter_high ++;
			if(counter_high > 13235)
			{
				return 2;
			}
		}
		else
		{
			if(counter_high > 1)
			{
				*distance_cm = ((counter_high)*10)/294;
				return 0;
			}
			timeout++;
		}
		if(timeout > 13235)
		{
			return 2;
		}
	}
}
u8 func_get_supersonic_echo_signal2(u16 *distance_cm_side)
{
	u16 counter_high=0;
	u16 timeout=0;
	
	func_trig_supersonic2();
	for(;;)
	{
		delay_us(1);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == Bit_SET)
		{
			counter_high ++;
			if(counter_high > 13235)
			{
				return 2;
			}
		}
		else
		{
			if(counter_high > 1)
			{
				*distance_cm_side = ((counter_high)*10)/294;
				return 0;
			}
			timeout++;
		}
		if(timeout > 13235)
		{
			return 2;
		}
	}
}
u8 func_get_supersonic_echo_signal3(u16 *distance_cm_side2)
{
	u16 counter_high=0;
	u16 timeout=0;
	
	func_trig_supersonic3();
	for(;;)
	{
		delay_us(1);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == Bit_SET)
		{
			counter_high ++;
			if(counter_high > 13235)
			{
				return 2;
			}
		}
		else
		{
			if(counter_high > 1)
			{
				*distance_cm_side2 = ((counter_high)*10)/294;
				return 0;
			}
			timeout++;
		}
		if(timeout > 13235)
		{
			return 2;
		}
	}
}
