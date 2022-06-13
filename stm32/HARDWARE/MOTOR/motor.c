#include "motor.h"  //导入头文件
#include "stm32f10x.h"  //导入STM32官方库
#include "stm32f10x_rcc.h"  //导入STM32的RCC时钟库
#include "delay.h"       //延时库
#include "timer.h"
#include "led.h"


void Motor_12_Config(void)  //定义初始化函数
{
GPIO_InitTypeDef GPIO_InitStructure;   //定义GPIO_InitTypeDef结构体
RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);  //开启引脚时
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //定义IN引脚
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置输出功率
GPIO_Init(GPIOF, &GPIO_InitStructure);  //初始化GPIOF的引脚参数,写进
GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3); //所有引脚拉低
	
	TIM_Config();
	PWM_Init(TIM3,1,20000,99);//PA6
	PWM_Init(TIM3,2,20000,50);//PA7
	//PWM_Init(TIM3,4,2000,50);//PB1
} 
void Motor_STOP(void)//Left FA1
{
	Enable(0);
} 
void Motor_1_NRun(void)//Left FA0
{
	Enable(1);
	IN1(0);
} 
void Motor_1_PRun(void)//Left FA0
{
	Enable(1);
	IN1(1);
}
void Motor_2_PRun(void)//Right FA2
{
	Enable(1);
	IN3(0);
}
void Motor_2_NRun(void)//Right FA2
{
	Enable(1);
	IN3(1);
}

void Motor_1_Speed(u16 percentage)//Left
{
	u16 duty;
	u16 arr = 20000;//默认freq=20
	duty =arr*(percentage+4)/100-1;
	PWM_SetDuty(TIM3, 1, duty);//PA6=>TIM3 Channal1
}

void Motor_2_Speed(u16 percentage)//Right
{
	u16 duty;
	u16 arr = 20000;//默认freq=20
	duty =arr*(100-percentage)/100-1;
	PWM_SetDuty(TIM3, 2, duty);//PA7=>TIM3 Channal2
}

void MoveClockwise()
{
	Motor_1_PRun();
	Motor_1_Speed(30);
	Motor_2_NRun();
	Motor_2_Speed(70);
}

void TurnRight90()
{
	MoveClockwise();
	delay_ms(1100);
	delay_ms(500);
	Motor_STOP();
	delay_ms(1000);
}

void TurnRight45()//P1P2共用
{
	MoveClockwise();
	delay_ms(420);
	Motor_STOP();
	delay_ms(1000);
}


void MoveCounterClockwise()
{
	Motor_1_NRun();
	Motor_1_Speed(70);
	Motor_2_PRun();
	Motor_2_Speed(30);
}

void TurnLeft90()
{
	MoveCounterClockwise();
	delay_ms(1200);
	Motor_STOP();
	delay_ms(1000);
}

void PTurnLeft90()//用于P1的左转
{
	MoveCounterClockwise();
	delay_ms(1600);
	delay_ms(400);
	Motor_STOP();
	delay_ms(1000);
}


void PTurnRight90()//用于P1的右转
{
	MoveClockwise();
	delay_ms(1800);
	delay_ms(450);
	Motor_STOP();
	delay_ms(1000);
}

void TurnLeft45()//P1P2共用
{
	MoveCounterClockwise();
	delay_ms(600);
	Motor_STOP();
	delay_ms(1000);
}

void Turn180()//未使用
{
	MoveCounterClockwise();
	delay_ms(1500);
	delay_ms(1500);
	delay_ms(800);
	Motor_STOP();
	delay_ms(1000);
}

void Motor_Line(u16 tyy)
{
	Motor_1_PRun();
	Motor_2_PRun();
	switch(tyy)
	{
		case 0:   //left(Smoothly)
			Motor_1_Speed(20);
			Motor_2_Speed(25); 
			delay_ms(20);
		break;
		case 1:  //right(Smoothly)
			Motor_1_Speed(22);
			Motor_2_Speed(21);
			delay_ms(20);
		break;
		case 2:   //left(Ex)
			Motor_1_Speed(20);
			Motor_2_Speed(29); 
			delay_ms(20);
		break;
		case 3:  //right(Ex)
			Motor_1_Speed(26);
			Motor_2_Speed(21);
			delay_ms(20);
		break;
	}
}

void Motor_Move(u16 type)
{
	Motor_1_PRun();
	Motor_2_PRun();
	
	switch(type)
	{
		case 0:
			Motor_1_Speed(20);
			Motor_2_Speed(21); 
			delay_ms(6);
		break;
		case 1:
			Motor_1_Speed(4);
			Motor_2_Speed(60);
		delay_ms(30);
		break;
		case 2:
			Motor_1_Speed(18);
			Motor_2_Speed(43);
		delay_ms(30);
		break;
		case 3:
			Motor_1_Speed(20);
			Motor_2_Speed(23);
		delay_ms(30);
		break;
		case 4:
			Motor_1_Speed(23);
			Motor_2_Speed(20);
		delay_ms(40);
		break;
		case 5:
			Motor_1_Speed(42);
			Motor_2_Speed(20);
		delay_ms(40);
		break;
		case 6:
			Motor_1_Speed(60);
			Motor_2_Speed(10);
		delay_ms(40);
		break;
		case 10:
			Motor_1_Speed(50);
			Motor_2_Speed(53);
		break;
		case 11://左倾
			Motor_1_Speed(50);
			Motor_2_Speed(54);
		break;
		case 12://右倾
			Motor_1_Speed(50);
			Motor_2_Speed(52);
		break;
		case 13://偏右
			Motor_1_Speed(20);
			Motor_2_Speed(23);
		break;
			case 14://偏左
			Motor_1_Speed(20);
			Motor_2_Speed(25);
		break;
					case 15:
			Motor_1_Speed(50);
			Motor_2_Speed(55);
		break;
	}
}

void Motor_Move3(u16 type,float *timecount)
{
	Motor_1_PRun();
	Motor_2_PRun();
	
	switch(type)
	{
		case 0:
			Motor_1_Speed(20);
			Motor_2_Speed(21); 
			delay_ms(6);
			*timecount = *timecount+0.006;
		break;
		case 1:
			Motor_1_Speed(4);
			Motor_2_Speed(60);
		delay_ms(30);
		*timecount = *timecount+0.03;
		break;
		case 2:
			Motor_1_Speed(18);
			Motor_2_Speed(43);
		delay_ms(30);
		*timecount = *timecount+0.03;
		break;
		case 3:
			Motor_1_Speed(20);
			Motor_2_Speed(23);
		delay_ms(30);
		*timecount = *timecount+0.03;
		break;
		case 4:
			Motor_1_Speed(23);
			Motor_2_Speed(20);
		delay_ms(40);
		*timecount = *timecount+0.04;
		break;
		case 5:
			Motor_1_Speed(42);
			Motor_2_Speed(20);
		delay_ms(40);
		*timecount = *timecount+0.04;
		break;
		case 6:
			Motor_1_Speed(60);
			Motor_2_Speed(10);
		delay_ms(40);
		*timecount = *timecount+0.04;
		break;
		case 10:
			Motor_1_Speed(50);
			Motor_2_Speed(53);
		break;
		case 11://左倾
			Motor_1_Speed(50);
			Motor_2_Speed(54);
		break;
		case 12://右倾
			Motor_1_Speed(50);
			Motor_2_Speed(52);
		break;
		case 13://zhi
			Motor_1_Speed(20);
			Motor_2_Speed(23);
		break;
			case 14://zhi
			Motor_1_Speed(20);
			Motor_2_Speed(25);
		break;
	}
}

void Motor_Move2(u16 type)
{
	Motor_1_PRun();
	Motor_2_PRun();
	
	switch(type)
	{
		case 0:
			Motor_1_Speed(20);
			Motor_2_Speed(21); 
			delay_ms(6);
		break;
		case 1:
			Motor_1_Speed(14);
			Motor_2_Speed(35);
		delay_ms(30);
		break;
		case 2:
			Motor_1_Speed(18);
			Motor_2_Speed(28);
		delay_ms(30);
		break;
		case 3:
			Motor_1_Speed(20);
			Motor_2_Speed(23);
		delay_ms(30);
		break;
		case 4:
			Motor_1_Speed(23);
			Motor_2_Speed(20);
		delay_ms(40);
		break;
		case 5:
			Motor_1_Speed(30);
			Motor_2_Speed(20);
		delay_ms(40);
		break;
		case 6:
			Motor_1_Speed(40);
			Motor_2_Speed(10);
		delay_ms(40);
		break;
		case 10:
			Motor_1_Speed(50);
			Motor_2_Speed(53);
		break;
		case 11://左倾
			Motor_1_Speed(50);
			Motor_2_Speed(54);
		break;
		case 12://右倾
			Motor_1_Speed(50);
			Motor_2_Speed(52);
		break;
		case 13://zhi
			Motor_1_Speed(20);
			Motor_2_Speed(23);
		break;
			case 14://zhi
			Motor_1_Speed(20);
			Motor_2_Speed(25);
		break;
	}
}


void Motor_Turn_After_Scanning(u16 type)
{
	switch(type)
	{
		int i=0;
		case 1:
			LED0=0;//RED
			LED1=1;
			delay_ms(10);
			TurnRight45();
			delay_ms(500);
			//MoveClockwise();
			//delay_ms(50);
			Motor_Move(15);
			for(;i<9;i++)
			{
				delay_ms(1000);
			}
			Motor_STOP();
			TurnRight45(); 
		break;
		case 2:
			LED0=1;//GREEN
			LED1=0;
			delay_ms(10);
			//MoveClockwise();
			//delay_ms(200);
			//Motor_STOP();
			//delay_ms(20);
			/*Motor_Move(10);
			for(;i<14;i++)
			{
				delay_ms(1000);
			}
			Motor_STOP();
			TurnRight90();
			delay_ms(50);*/
			TurnLeft45();
			delay_ms(500);
			//MoveCounterClockwise();
			//delay_ms(50);
			Motor_Move(10);
			for(;i<10;i++)
			{
				delay_ms(1000);
			}
			Motor_STOP();
			TurnRight90();
			delay_ms(100);
			Motor_Move(10);
			i=0;
			for(;i<10;i++)
			{
				delay_ms(1000);
			}
			Motor_STOP();
			delay_ms(10);
			TurnRight45();
			delay_ms(10);
			
		break;
		case 3:
			LED0=0;
			LED1=0;
			delay_ms(10);
			TurnLeft45();
			delay_ms(500);
			//MoveCounterClockwise();
			//delay_ms(50);
			Motor_Move(10);
			for(;i<10;i++)
			{
				delay_ms(1000);
			}
			Motor_STOP();
			TurnRight45();
			delay_ms(100);
			//MoveClockwise();
			//delay_ms(50);
			TurnRight90();
		break;
	}	
}

void Motor_Correct_Accordding_To_Right_Fence(float dis,float lastDis)
{
	if(1)
	{
		if(lastDis-dis>0.05f)//Left
		{
			LED0=1; //green
			LED1=0;
			Motor_Line(2);
		}
		else if(lastDis-dis>0)
		{
			LED0=1; //green
			LED1=0;
			Motor_Line(0);
		}
		else if(lastDis-dis>-0.05)
		{
			Motor_Line(1);
		}
		else
		{
			Motor_Line(3);
		}
	}
}

void Motor_Correct_Accordding_To_Left_Fence(float dis,float lastDis)
{
	if(1)
	{
		if(lastDis-dis>0.05f)//Danger&Turn Right
		{
			LED0=1; //green
			LED1=0;
			Motor_Line(2);
		}
		else if(lastDis-dis>0)//Smoothly Turn right
		{
			LED0=1; //green
			LED1=0;
			Motor_Line(0);
		}
		else if(lastDis-dis>-0.05) //Smoothly Turn Left
		{
			Motor_Line(1);
		}
		else
		{
			Motor_Line(3); //Danger&Turn Left
		}
	}
}
