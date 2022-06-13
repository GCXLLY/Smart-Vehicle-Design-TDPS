#include "sys.h"
#include "usart.h"	  
#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h" 
#include "communication.h"
#include "DS1302.h"
#include "sys.h"


//在程序初始化时执行
void CommunicationInit(void)
{
	uart_init(9600);//TX为PA9,RX为PA10,波特率为9600
}

void SendMsg(void)
{
	char spare= ':';
	char teamInfo[200]="team member:";//这里输入要发送的信息
	int i=0;
	int j=0;
	DS1302_ReadTime();
	for(;j<7;j++)//7是该数据的长度
	{
		if(DS1302_Time[j])
		{
			//char one=DS1302_Time[j]%10+'0';
			//char ten=DS1302_Time[j]/10+'0';
			USART_SendData(USART1,DS1302_Time[j]);
			delay_ms(1);//防止发送缓存区数据溢出
			//USART_SendData(USART1,one);
			//delay_ms(1);//防止发送缓存区数据溢出
		
			//USART_SendData(USART1,spare);
			//delay_ms(1);//防止发送缓存区数据溢出
		}
		
	}
	/*
	for(;i<200;i++)
	{
		if(teamInfo[i])
		{
			USART_SendData(USART1,teamInfo[i]);
			delay_ms(1);//防止发送缓存区数据溢出
		}
	}*/
}
