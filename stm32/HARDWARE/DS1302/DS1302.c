#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "DS1302.h"

//引脚定义
#define DS1302_SCLK   GPIO_Pin_11
#define DS1302_IO     GPIO_Pin_12
#define DS1302_CE     GPIO_Pin_13

#define ON    1
#define OFF    0

#define PIN_x(x,b)  if(b)  GPIO_ResetBits(GPIOA,x); else  GPIO_SetBits(GPIOA,x);

//寄存器写入地址/指令定义
#define DS1302_SECOND		0x80
#define DS1302_MINUTE		0x82
#define DS1302_HOUR			0x84
#define DS1302_DATE			0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY			0x8A
#define DS1302_YEAR			0x8C
#define DS1302_WP			0x8E

//时间数组，索引0~6分别为年、月、日、时、分、秒、星期
unsigned char DS1302_Time[]={21,11,30,00,00,00,2};

/**
  * @brief  DS1302初始化
  * @param  无
  * @retval 无
  */
void DS1302_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin =DS1302_SCLK|DS1302_IO|DS1302_CE;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	PIN_x(DS1302_CE,OFF);
	PIN_x(DS1302_SCLK,OFF);
}

/**
  * @brief  DS1302写一个字节
  * @param  Command 命令字/地址
  * @param  Data 要写入的数据
  * @retval 无
  */
void DS1302_WriteByte(unsigned char Command,unsigned char Data)
{
	unsigned char i;
	PIN_x(DS1302_CE,ON);
	
	for(i=0;i<8;i++)
	{
		PIN_x(DS1302_IO,Command&(0x01<<i));
		PIN_x(DS1302_SCLK,ON);
		PIN_x(DS1302_SCLK,OFF);
	}
	for(i=0;i<8;i++)
	{
		PIN_x(DS1302_IO,Data&(0x01<<i));
		PIN_x(DS1302_SCLK,ON);
		PIN_x(DS1302_SCLK,OFF);
	}
	PIN_x(DS1302_CE,OFF);
}

/**
  * @brief  DS1302读一个字节
  * @param  Command 命令字/地址
  * @retval 读出的数据
  */
unsigned char DS1302_ReadByte(unsigned char Command)
{
	unsigned char i,Data=0x00;
	Command|=0x01;	//将指令转换为读指令
	PIN_x(DS1302_CE,ON);
	for(i=0;i<8;i++)
	{
		PIN_x(DS1302_IO,Command&(0x01<<i));
		PIN_x(DS1302_SCLK,ON);
		PIN_x(DS1302_SCLK,OFF);
	}
	for(i=0;i<8;i++)
	{
		PIN_x(DS1302_SCLK,ON);
		PIN_x(DS1302_SCLK,OFF);
		if(DS1302_IO){Data|=(0x01<<i);}
	}
	PIN_x(DS1302_CE,OFF);
	PIN_x(DS1302_IO,OFF);	//读取后将IO设置为0，否则读出的数据会出错
	return Data;
}

/**
  * @brief  DS1302设置时间，调用之后，DS1302_Time数组的数字会被设置到DS1302中
  * @param  无
  * @retval 无
  */
void DS1302_SetTime(void)
{
	DS1302_WriteByte(DS1302_WP,0x00);
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);//十进制转BCD码后写入
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x80);
}

/**
  * @brief  DS1302读取时间，调用之后，DS1302中的数据会被读取到DS1302_Time数组中
  * @param  无
  * @retval 无
  */
void DS1302_ReadTime(void)
{
	unsigned char Temp;
	Temp=DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0]=Temp/16*10+Temp%16;//BCD码转十进制后读取
	Temp=DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6]=Temp/16*10+Temp%16;
}
