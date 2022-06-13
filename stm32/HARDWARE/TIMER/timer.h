#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"



void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM_Config(void);
void PWM_Init(TIM_TypeDef* timer, uint8_t time_channel, u16 arr, uint8_t duty);
void PWM_SetDuty(TIM_TypeDef* timer, uint8_t time_channel, u16 duty);
#endif
