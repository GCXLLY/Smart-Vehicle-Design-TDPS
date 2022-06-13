#ifndef __STM32F10X_H
#define __STM32F10X_H
#include "stm32f10x.h"
#endif



void init_hardware_supersonic(void);

u8 func_get_supersonic_echo_signal(u16 *distance_cm);
u8 func_get_supersonic_echo_signal2(u16 *distance_cm);
u8 func_get_supersonic_echo_signal3(u16 *distance_cm);