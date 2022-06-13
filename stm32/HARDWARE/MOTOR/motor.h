#ifndef __MOTOR1_H
#define __MOTOR1_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h" 
#define High    1
#define Low     0
#define IN1(a) if (a)  \
	GPIO_SetBits(GPIOF,GPIO_Pin_0);\
else  \
	GPIO_ResetBits(GPIOF,GPIO_Pin_0)
 
#define Enable(a) if (a)  \
	GPIO_SetBits(GPIOF,GPIO_Pin_1);\
else  \
	GPIO_ResetBits(GPIOF,GPIO_Pin_1)
 
#define IN3(a) if (a)  \
	GPIO_SetBits(GPIOF,GPIO_Pin_2);\
else  \
	GPIO_ResetBits(GPIOF,GPIO_Pin_2)
#define IN4(a) if (a)  \
	GPIO_SetBits(GPIOF,GPIO_Pin_3);\
else  \
	GPIO_ResetBits(GPIOF,GPIO_Pin_3)
void Motor_12_Config(void);
void Motor_STOP(void);
void Motor_1_PRun(void);
void Motor_1_NRun(void);
void Motor_2_PRun(void);
void Motor_2_NRun(void);
void MoveClockwise(void);
void TurnRight90(void);
void PTurnRight90(void);
void TurnRight45(void);
void MoveCounterClockwise(void);
void TurnLeft90(void);
void PTurnLeft90(void);
void TurnLeft45(void);
void Turn180(void);
void Motor_1_Speed(u16 percentage);
void Motor_2_Speed(u16 percentage);
void Motor_Line(u16 tyy);
void Motor_Move3(u16 type,float *timecount);
void Motor_Move2(u16 type);
void Motor_Move(u16 type);
void Motor_Turn_After_Scanning(u16 type);
void Motor_Correct_Accordding_To_Right_Fence(float dis,float lastDis);
void Motor_Correct_Accordding_To_Left_Fence(float dis,float lastDis);
#endif
