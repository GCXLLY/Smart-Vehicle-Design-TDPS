#include "KN15.h"
#include "stm32f10x.h"  //??STM32???
#include "stm32f10x_rcc.h"  //??STM32?RCC???
#include "delay.h"       //???
#include "timer.h"

void KN_15_Config()  //???????
{
	TIM_Config();
	PWM_Init(TIM3,4,20000,99);//PB1
} 

void KN_15_Open()
{
	delay_ms(10);
	PWM_SetDuty(TIM3, 4, 18000);
}
