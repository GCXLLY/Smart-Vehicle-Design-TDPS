#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h" 
#include "sys.h"

void CommunicationInit(void);
void SendMsg(void);
#endif
