#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "openmv.h"
#include "usart.h"
#include "sys.h"

void OpenMVInit(void)
{
	uart2_init(9600);//TX为PA2,RX为PA3,波特率为9600
}

unsigned char ReadData()
{
	unsigned char Data=0x00;
	Data=USART_ReceiveData(USART2);
	return Data;
}
