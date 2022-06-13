#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "sys.h"
#include "openmv.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "timer.h"
#include "key.h"
#include "motor.h"
#include "remote.h"
#include "communication.h"
#include "DS1302.h"
#include "KN15.h"
#ifndef __Z_OUTER_HARDWARE_SUPERSONIC_H
#define __Z_OUTER_HARDWARE_SUPERSONIC_H
#include "z_outer_hardware_supersonic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050.h"
#endif





void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }
 ///////TEST PROGRAM for YAW//////////////

int main(void)
{	
	vu8 key=0;	
	u8 rkey;
	int rst=0;
	int linecount=0;
	int frontAccumulation=0;
	int maxFrontAccumulation=10;
	int turnst=0;
	int turnnum=0;
	float timecount=0;
 int KNcount=0;
	 
	//patio==1 => OpenMV Following Line
	//patio==2 => MPU6050
	//patio==3 => SuperSonic
	//patio==4 => SuperSonicFollowingLine
	//patio==5 => OpenMV USART test
	//patio==6 => Turning Test
	//patio==7 => 
	//patio==8 => PWM control steering
	//patio==9 => Communication test
	
	
	int patio=4;

	int patiost=0;
		 
	u16 distance_cm;
	u16 distance_cm_side;
	u16 distance_cm_side2;
	u16 last_distance_side;
	float pitch,roll,yaw,iyaw,dyaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;
	float p2time=0;
	u16 TEN;
	u16 ONE;
	
	float initYaw=0;

	//u8 BUF[USART_REC_LEN];
	//extern u8 USART_RX_BUF[USART_REC_LEN];
	//extern u16 USART_RX_STA;
	//char speed;
	int state=0;
	 
	 
	uart_init(9600);
	LED_Init();
	//Remote_Init();
	delay_init();
	OpenMVInit();

	Motor_12_Config();
	//KN_15_Config();
	MPU_Init();
	mpu_dmp_init();
	DS1302_Init();
	KEY_Init();
		


	init_hardware_supersonic();

	LED0=1;
	LED1=1;
	
	USART_RX_BUF[0]='0';

	while(1)
	{	
		
		/*if (Remote_Scan() == 56)
		
		{
			rst = (rst+1)%2;
		}*/

		if(KEY_Scan(0)==KEY0_PRES)
		{
			key = (key+1)%2;
		}
		
		if(KEY_Scan(0)==KEY1_PRES)
		{
			patiost = (patiost+1)%20;
		}
		///////////////////////////////
		if(patio ==10){
		if(key ==1){
		func_get_supersonic_echo_signal2(&distance_cm_side);
			if(distance_cm_side < 10){
					LED0=1; //green
					LED1=0;
					delay_ms(500);
			}
			else{
					LED0=0; //red
					LED1=1;
					delay_ms(500);
			}
		}
		}
		/////////////////////////////////
		if (patio ==4)
		{
			if(key==1)
			{
				if(turnst==0)//Start
				{
					int i;
					for(;i<7;i++)
					{
						Motor_Move(10);
						delay_ms(1000);
					}
					
					Motor_STOP();
					delay_ms(1000);
					delay_ms(1000);
					turnst++;
				}
				
				if(turnst==1)//Turn Right To Face Graph
				{
					TurnLeft90();
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					delay_ms(500);
					Motor_STOP();
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					turnst++;
				}
				

				if(turnst==2)//Wait for sig from OpenMV
				{
					if(USART_RX_STA != 0)
					{
						USART_RX_STA = 0;
						ONE= USART_RX_BUF[0] - '0';
					}
					Motor_Turn_After_Scanning(ONE);
					if(ONE==1||ONE==2||ONE==3)
					//if(1)
					{
						//LED0=0;
						//LED1=0;
						frontAccumulation=0;
						turnst++;
					}
					//turnst++;
				}
				
				
				if (turnst==3)//Move to fence
				{
					func_get_supersonic_echo_signal(&distance_cm);
					Motor_Move(13);
					delay_ms(10);
					Motor_Move(14);
					delay_ms(10);
					if(distance_cm <=8)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							TurnLeft90();
							//TurnLeft45();//fix the dif
							Motor_STOP();
							turnst = 4;
							frontAccumulation=0;
						}
					}
					else
					{
						if(frontAccumulation>0)
							frontAccumulation--;
					}
				}
				
				if(turnst==4)//Move till fence dis
				{
					func_get_supersonic_echo_signal2(&distance_cm_side);
					if (linecount <= 50)
					{
						
						if(distance_cm_side <=10)
						{
							if(distance_cm_side <=5){
							LED0=1; //red
							LED1=0;
							Motor_Line(2);
							}
							else{
							LED0=1;
							LED1=0;
							Motor_Line(0);
							}
							
						}					
						if(distance_cm_side >10)
						{
							if(distance_cm_side >15){
							LED0=0; //red
							LED1=1;
							Motor_Line(3);
							}
							else{
							LED0=0;
							LED1=1;
							Motor_Line(1);
							}
							if(distance_cm_side>23)
							{
								linecount = linecount +1;
							}
						}
						
						/*
						if(last_distance_side==0)
						{
							last_distance_side=distance_cm_side;
						}
						Motor_Correct_Accordding_To_Right_Fence(distance_cm_side,last_distance_side);
						last_distance_side=distance_cm_side;*/
					}
					else
					{
						TurnRight90();
						linecount =0;
						turnnum = turnnum+1;
						turnst=5;
					}
				}
				
				if(turnst ==5)//Move till facing wall
				{
					//LED0=0;
					//LED1=0;
					//delay_ms(3000);
					func_get_supersonic_echo_signal(&distance_cm);
					func_get_supersonic_echo_signal2(&distance_cm_side);
					if (distance_cm < 10)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							delay_ms(100);
							TurnLeft90();
							Motor_STOP();
							if(turnnum==1)
							{
								turnst=4;
							}
							else if(turnnum==2)
							{
								turnst =6;
								}
							frontAccumulation=0;
						}
					}
					else 
					{
						if(frontAccumulation>0)
							frontAccumulation--;
						if(distance_cm_side <=10)
						{
							
							if(distance_cm_side <=5){
							LED0=1; //red
							LED1=0;
							Motor_Line(2);
							}
							else{
							LED0=1;
							LED1=0;
							Motor_Line(0);
							}	
						}					
						if(distance_cm_side >10)
						{
							LED0=0; //red
							LED1=1;
							Motor_Line(1);
						}
					}
				}
				
				if (turnst==6)//Move till bottle
				{
					
					func_get_supersonic_echo_signal(&distance_cm);
					func_get_supersonic_echo_signal2(&distance_cm_side);
					
					if (distance_cm < 10)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							Motor_STOP();
							turnst =7;
							frontAccumulation=0;
						}
					}
					else 
					{
						if(frontAccumulation>0)
							frontAccumulation--;
						if(distance_cm_side <=10)
						{
							if(distance_cm_side <=5){
							LED0=1; //red
							LED1=0;
							Motor_Line(2);
							}
							else{
							LED0=1;
							LED1=0;
							Motor_Line(0);
							}	
						}					
						if(distance_cm_side >10)
						{
							LED0=0; //red
							LED1=1;
							Motor_Line(1);
						}
					}
				}
				if(turnst ==7)//TO DO: Release the ball
				{
					TurnLeft90();
					LED0=0;
					LED1=0;
					delay_ms(500);
					TurnLeft90();
					delay_ms(10);
					MoveCounterClockwise(); //石子路的180转向额外补正！！！
					delay_ms(600);
					Motor_STOP();
					delay_ms(10);
					turnst=8;
				}
				if(turnst ==8)//TO DO: Release the ball
				{
					//if (KNcount==0){
					//KN_15_Config();
					//}
					Motor_STOP();
					delay_ms(100);
					Motor_1_NRun();
					Motor_2_NRun();
					Motor_1_Speed(70);
					Motor_2_Speed(70); 
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					Motor_STOP();
					delay_ms(50);
					turnst++;
					KN_15_Open();
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					turnst++;
				}
				
				if(turnst==9)//到头右转；两次
				{
					func_get_supersonic_echo_signal(&distance_cm);
					func_get_supersonic_echo_signal3(&distance_cm_side2);
					if (distance_cm < 10)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							
							if(turnnum==2)
							{
								turnst =9;
								delay_ms(100);
								TurnRight90(); //石子路右转可能需要额外补正！
								Motor_STOP();
								turnnum++;
							}
							else if(turnnum==3)
							{
								delay_ms(100);
								TurnRight90(); //在平滑地面的右转，但是左侧没有障碍会导致车头偏左，可能需要补正
								delay_ms(10);
								//MoveClockwise();
								//delay_ms(100);
								Motor_STOP();
								
								
								turnst=10;
							}
							frontAccumulation=0;
						}
					}
					else 
					{
						if(frontAccumulation>0)
							frontAccumulation--;
						if(distance_cm_side2 <=10)
						{
							if(distance_cm_side2 <=5){
							LED0=1; //red
							LED1=0;
							Motor_Line(3);
							}
							else{
							LED0=1;
							LED1=0;
							Motor_Line(1);
							}
						}					
						if(distance_cm_side2 >10)
						{
							LED0=0; //red
							LED1=1;
							Motor_Line(0);//Left
						}
						/*
						if(last_distance_side==0)
						{
							last_distance_side=distance_cm_side2;
						}
						Motor_Correct_Accordding_To_Right_Fence(distance_cm_side2,last_distance_side);
						last_distance_side=distance_cm_side2;*/
					}
				}
				if(turnst==10)//沿着左边走;没墙左拐
				{
					func_get_supersonic_echo_signal3(&distance_cm_side2);
					if (linecount <= 50)
					{
						
						if(distance_cm_side2 <=10)
						{
							if(distance_cm_side2 <=5){
							LED0=1; //green
							LED1=0;
							Motor_Line(3);
							}
							else{
							LED0=1;
							LED1=0;
							Motor_Line(1);
							}
							if(linecount >0){
							linecount=0;
							}
						}					
						if(distance_cm_side2 >10)
						{
							LED0=0; //red
							LED1=1;
							Motor_Line(0);
							if(distance_cm_side2>25)
							{
								linecount = linecount +1;
							}
						}
						
						/*
						if(last_distance_side==0)
						{
							last_distance_side=distance_cm_side;
						}
						Motor_Correct_Accordding_To_Right_Fence(distance_cm_side,last_distance_side);
						last_distance_side=distance_cm_side;*/
					}
					else
					{
						TurnLeft90();
						linecount =0;
						Motor_Move(13);
						delay_ms(1000);
						delay_ms(1000);
						Motor_Move(14);
						delay_ms(1000);
						Motor_Move(13);
						delay_ms(1000);
						Motor_STOP();
						delay_ms(100);
						SendMsg();
						turnst=11;
					}
				}
			}
		}

		if(patio == 1)
		{
			if(key==1)
			{
				if(turnst==0)
				{
					if(USART_RX_STA != 0)
					{
						USART_RX_STA = 0;
						ONE = USART_RX_BUF[0] - '0';
					}
					rkey = Remote_Scan();
					
					if (timecount <= 220)
					{
						switch(rst)
						{
							case 0 : Motor_Move3(ONE,&timecount);
							case 1 : Motor_Move3(rkey,&timecount);
						}
					}
					else
					{
						switch(rst)
						{
							case 0 : Motor_Move2(ONE);
							case 1 : Motor_Move2(rkey);
						}
					}
					
					
					func_get_supersonic_echo_signal(&distance_cm);
					
					if (distance_cm < 25)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							Motor_STOP();
							turnst =1;
							frontAccumulation=0;
						}
					}
					else
					{
						if(frontAccumulation>0)
							frontAccumulation--;
					}
				}
				if(turnst==1)
				{
					PTurnRight90();
					delay_ms(1500);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);					
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);
					Motor_Move(13);
					delay_ms(1000);
					Motor_Move(14);
					delay_ms(1000);					
					turnst++;
				}
				if(turnst==2)
				{
					
					func_get_supersonic_echo_signal(&distance_cm);
					
					if (distance_cm < 20)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							Motor_STOP();
							delay_ms(1500);
							PTurnLeft90();
					Motor_Move(13);
					delay_ms(10);
					Motor_Move(14);
					delay_ms(10);	
							//USART_SendData(USART1,1);
							turnst =3;
							frontAccumulation=0;
						}
					}
					else
					{
						if(frontAccumulation>0)
							frontAccumulation--;
					}
				}
				if(turnst==3)
				{
					func_get_supersonic_echo_signal2(&distance_cm_side);
					if (distance_cm_side >10){
						if(linecount >0){
					linecount--;
					}
					}
					else {
					linecount++;
					}
					if (linecount <= 5){
					if(USART_RX_STA != 0)
					{
						USART_RX_STA = 0;
						ONE = USART_RX_BUF[0] - '0';
					}
					rkey = Remote_Scan();
					switch(rst)
					{
						case 0 : Motor_Move2(ONE);
						case 1 : Motor_Move(rkey);
					}
					
				}
					else {
					Motor_STOP();
						delay_ms(10);
						turnst=4;
					}
					func_get_supersonic_echo_signal(&distance_cm);
					
					if (distance_cm < 25)
					{
						frontAccumulation++;
						if(frontAccumulation>maxFrontAccumulation)
						{
							Motor_STOP();
							delay_ms(10);
							turnst=4;
							frontAccumulation=0;
						}
					}
					else
					{
						if(frontAccumulation>0)
							frontAccumulation--;
					}
					
				}
			}
			else
			{
				Motor_STOP();
			}
		}

		if(patio == 3)//SuperSonic
		{
			if(key==1)
			{
				func_get_supersonic_echo_signal(&distance_cm);
				if (distance_cm <= 5)
				{
					LED0=1; //green
					LED1=0;
					delay_ms(500);
				}
				else
				{
					LED0=0; //red
					LED1=1;
					delay_ms(500);
				}
			}
			else
			{
				Motor_STOP();
				LED0=1;
				LED1=1;
			}
		}


		if(patio == 2)
		{
			if(key==1)
			{
				/*if((mpu_dmp_get_data(&pitch,&roll,&yaw)==0) && (patiost==9))
				{
					iyaw = yaw;
					patiost = 0;
				}*/
				////first turn/////
				/*
				if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
				{
					//func_get_supersonic_echo_signal(&distance_cm);
					dyaw = iyaw - yaw;
					LED0=0;  //red
					LED1=1;
					delay_ms(500);
					if ((dyaw <= 300) && (patiost == 0))
					{
						Motor_1_NRun();
						Motor_2_PRun();
						Motor_1_Speed(70);
						Motor_2_Speed(20);
					}
					Motor_STOP();
					patiost=8;
					if(patiost ==8)
					{
						Motor_1_PRun();
						Motor_2_PRun();
						Motor_1_Speed(20);
						Motor_2_Speed(20);
						delay_ms(2000);
						patiost=1;
					}
		
					if ((dyaw <= 900) && (patiost == 1))
					{
						Motor_1_NRun();
						Motor_2_PRun();
						Motor_1_Speed(70);
						Motor_2_Speed(20);
					}
					Motor_STOP();
					patiost=2;
		
					if(patiost == 2)
					{
						Motor_1_PRun();
						Motor_2_PRun();
						Motor_1_Speed(20);
						Motor_2_Speed(20);
						delay_ms(5000);
						patiost=3;
					}
					Motor_STOP();
					patiost=3;
		
					if((dyaw >= 50) && (patiost == 3))
					{
						Motor_2_NRun();
						Motor_1_PRun();
						Motor_2_Speed(70);
						Motor_1_Speed(20);
					}
	
					Motor_STOP();
					patiost=4;
					if(patiost == 4)
					{
						Motor_1_PRun();
						Motor_2_PRun();
						Motor_1_Speed(20);
						Motor_2_Speed(20);
						delay_ms(5000);
						patiost=5;
					}
					Motor_STOP();
					patiost=5;
				}*/
				if (patiost == 0){
				Motor_1_PRun();
				Motor_2_PRun();
				Motor_1_Speed(20);
				Motor_2_Speed(23); 
				delay_ms(6000);
				Motor_STOP();
				patiost = 1;
				}
				if (patiost == 2){
				TurnLeft90();
				TurnLeft45();
				Motor_STOP();
				patiost = 3;
				}
				if (patiost == 4){
				Motor_1_PRun();
				Motor_2_PRun();
				Motor_1_Speed(20);
				Motor_2_Speed(20); 
				delay_ms(3000);
				Motor_STOP();
				patiost = 5;
				}
				
				if (patiost == 6){
				TurnRight45();
				TurnRight90();
				Motor_STOP();
				patiost = 7;
				}
				
				if (patiost == 8){
				func_get_supersonic_echo_signal(&distance_cm);
				Motor_Move(0);
					if(distance_cm <=5){
					TurnLeft90();
					Motor_STOP();
					patiost = 9;
					}
				}
				
				if (patiost == 10){
				func_get_supersonic_echo_signal(&distance_cm);
				Motor_Move(0);
					if(distance_cm <=5){
					TurnRight90();
					Motor_STOP();
					patiost = 11;
					}
				}				
				
				if (patiost == 12){
				func_get_supersonic_echo_signal(&distance_cm);
				Motor_Move(0);
					if(distance_cm <=5){
					TurnLeft90();
					Motor_STOP();
					patiost = 13;
					}
				}
				
				if (patiost == 14){
				func_get_supersonic_echo_signal(&distance_cm);
				Motor_Move(0);
					if(distance_cm <=5){
					TurnRight90();
					Motor_STOP();
					patiost = 15;
					}
				}				
				if (patiost == 16){
				func_get_supersonic_echo_signal(&distance_cm);
				Motor_Move(0);
					if(distance_cm <=5){
					TurnLeft90();
					Motor_STOP();
					patiost = 17;
					}
				}		
				if (patiost == 18){
				Motor_1_PRun();
				Motor_2_PRun();
				Motor_1_Speed(20);
				Motor_2_Speed(23); 
				delay_ms(3000);
				Motor_STOP();
				patiost = 19;
				}
			}//key
			else
			{
				Motor_STOP();
			}	
		}	
		
		/////////////////////////////////////////////
		
		if(patio == 5)
		{
			if(USART_RX_STA != 0)
			{
				USART_RX_STA = 0;
				ONE = USART_RX_BUF[0] - '0';
			}
			LED0=1;
			LED1=1;
			switch(ONE)
			{
				case 1:
					//RED
					LED0=0;
					LED1=1;
					delay_ms(80);
				break;
				case 2:
					//GREEN
					LED0=1;
					LED1=0;
					delay_ms(80);
				break;
				case 3:
					LED0=0;
					LED1=0;
					delay_ms(80);
				break;
			}
		}
		if(patio == 6)
		{
			if(1)
			{
					LED0=0;  //red
					LED1=1;
					delay_ms(5000);
					LED0=1;  //green
					LED1=0;
					delay_ms(5000);
				
			}
		}
		if(patio==7)
		{
			if(1)
			{
				Motor_Move(0);

				func_get_supersonic_echo_signal(&distance_cm);
				if (distance_cm <= 20)
				{
					LED0=1; //green
					LED1=0;
					Motor_STOP();
					delay_ms(500);
					MoveClockwise();
					delay_ms(1000);
					Motor_STOP();
					delay_ms(1500);
				}
			}
		}
		if(patio==8)
		{
			if(key==1)
			{
				KN_15_Open();
				delay_ms(2000);
			}
		}
		if(patio==9)
		{
			SendMsg();
			delay_ms(1000);
		}
	}
}
