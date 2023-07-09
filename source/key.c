#ifndef _KEY_C_
#define _KEY_C_
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/ca51f_config.h"		
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"
#include "include/key.h"
#include "include/uart.h"
#include <intrins.h>

uint8_t key_value1,key_value2,key_value3,key_value4=0;
uint8_t FahrenFlag = 0;
//EPCON寄存器定义
#define EPPL(N)	(N<<7)
/*****************************************************************************
 ** \brief	 KEY_Init	
			 KEY_Init初始化GPIO函数,比如按键输入
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void KEY_init(void)
{
/*按键 上拉输入	P40	| S4	P41	| S3	P42	| S2	P43	| S1	*/
	
	P40F = INPUT|PU_EN;				//P40设置为输入模式并使能上拉电阻 
	P40C |= 0x20;					//上拉电阻选择为强上拉
// 	P40C &= ~0x20;					//上拉电阻选择为弱上拉	
	
	P41F = INPUT|PU_EN;				//P41设置为输入模式并使能上拉电阻 
	P41C |= 0x20;					//上拉电阻选择为强上拉
// 	P41C &= ~0x20;					//上拉电阻选择为弱上拉	
	
	P42F = INPUT|PU_EN;				//P42设置为输入模式并使能上拉电阻 
	P42C |= 0x20;					//上拉电阻选择为强上拉
// 	P42C &= ~0x20;					//上拉电阻选择为弱上拉		
	
	P43F = INPUT|PU_EN;				//P43设置为输入模式并使能上拉电阻 
	P43C |= 0x20;					//上拉电阻选择为强上拉
// 	P43C &= ~0x20;					//上拉电阻选择为弱上拉
}

/*外部中断2控制例程****************************************************************************************************/
void INT2_Init(void)
{
//	P10F = INPUT;							//P10设置为输入模式
	INDEX = 0;								//EPCON为带索引的寄存器，设置INDEX=0 对应INT2
	EPCON = EPPL(1) | 32;					//设置P40为INT2中断引脚，下降沿触发
	INT2EN = 1; 							//外部中断2中断使能
	EPIE |= 0x01;							//INT2中断使能
	key_value4 = 0;		
}
void INT2_ISR (void) interrupt 7
{
	if(EPIF & 0x01)
	{
		EPIF = 0x01;
//		int2_flag = 1;
		key_value4 =40;
	}
}

/*外部中断3控制例程****************************************************************************************************/
void INT3_Init(void)
{
//	P11F = INPUT;							//P11设置为输入模式	
	INDEX = 1;								//INDEX为带索引的寄存器，设置INDEX=1 对应INT3
	EPCON = EPPL(1) | 33;					//设置P41为INT3中断引脚，下降沿触发		
	INT3EN = 1; 							//外部中断3中断使能 
	EPIE |= 0x02;							//INT3中断使能
	key_value3 = 0;	
}
void INT3_ISR (void) interrupt 8
{
	if(EPIF & 0x02)
	{
		EPIF = 0x02;	
	key_value3 = 41;	
	}
}
/*********************************************************************************************************************/

/*外部中断4控制例程****************************************************************************************************/
void INT4_Init(void)
{
//	P12F = INPUT;							//P12设置为输入模式		
	INDEX = 2;								//INDEX为带索引的寄存器，设置INDEX=2 对应INT4
	EPCON = EPPL(1) | 34;					//设置P42为INT4中断引脚，下降沿触发				
	INT4EN = 1;  							//外部中断4中断使能
	EPIE |= 0x04;							//INT4中断使能
	key_value2 = 0;	
}
void INT4_ISR (void) interrupt 9
{
	if(EPIF & 0x04)
	{
		EPIF = 0x04;
	key_value2 = 42;	
	}
}
/*********************************************************************************************************************/

/*外部中断5控制例程****************************************************************************************************/
void INT5_Init(void)
{
//	P13F = INPUT;							//P13设置为输入模式		
	INDEX = 3;								//INDEX为带索引的寄存器，设置INDEX=3 对应INT5
	EPCON = EPPL(1) | 35;					//设置P43为INT5中断引脚，下降沿触发		
	INT5EN = 1; 							//外部中断5中断使能 
	EPIE |= 0x08;							//INT5中断使能
	key_value1 = 0;	
}
void INT5_ISR (void) interrupt 10
{
	if(EPIF & 0x08)
	{
		EPIF = 0x08;	
	key_value1 = 43;	
	}
}

uint8_t ShortKey1 = 0,ShortKey2 = 0,ShortKey3 = 0,ShortKey4 = 0;
uint8_t LongKey1 = 0,LongKey2 = 0,LongKey3 = 0,LongKey4 = 0;
uint8_t Hold_down1 = 0,Hold_down2 = 0;
void Key_Scanf(void)
{
	uint8_t Key1Flag = 0,Key2Flag = 0,Key3Flag = 0,Key4Flag = 0;
	static uint16_t times4 = 0,times3 = 0,times2 = 0,times1 = 0;
	if(40==key_value4)																						//key4   S4
		{
			if(P40 == 0)
			{
				times4++;
				Key4Flag = 0;
				if(times4>=Maxnum)
				{
					times4 =Maxnum;
				}
			}
			else
			{
				key_value4=0;
				if(times4<Maxnum)
				Key4Flag = 1;
				if((times4 <= Shortnum)&&(Key4Flag == 1))
				{
					ShortKey4 = 1;
					times4 = 0;
					Key4Flag = 0;
				}
				else if((times4 >Shortnum)&&(Key4Flag == 1))
				{
					LongKey4 = 1;
					times4 = 0;
					Key4Flag = 0;
				}
				uart_printf("%Key1 times=%d\n",times4);	
			}
		}
		if(41==key_value3)																						//key3   S3
		{
			if(P41 == 0)
			{
				times3++;
				Key3Flag = 0;
				if(times3>=Maxnum)
				{
					times3 =Maxnum;
					Key3Flag = 2;
				}
			}
			else
			{
				key_value3=0;
				if(times3<Maxnum)
				Key3Flag = 1;
				if((times3 <= Shortnum)&&(Key3Flag == 1))
				{
					ShortKey3 = 1;
					times3 = 0;
					Key3Flag = 0;
				}
				else if((times3 >Shortnum)&&(Key3Flag == 1))
				{
					LongKey3 = 1;
					times3 = 0;
					Key3Flag = 0;
				}
				uart_printf("%Key2 times=%d\n",times3);	
			}
		}
		if((times3 == Maxnum)&&(Key3Flag == 2))
		{
			Hold_down1 = 1;
		}
		if(42==key_value2)																						//key2   S2
		{
			if(P42 == 0)
			{
				times2++;
				Key2Flag = 0;
				if(times2>=Maxnum)
				{
					times2 =Maxnum;
					Key2Flag = 2;
				}
			}
			else
			{
				key_value2=0;
				if(times2<Maxnum)
				Key2Flag = 1;
				if((times2 <= Shortnum)&&(Key2Flag == 1))
				{
					ShortKey2 = 1;
					times2 = 0;
					Key2Flag = 0;
				}
				else if((times2 >Shortnum)&&(Key2Flag == 1))
				{
					LongKey2 = 1;
					times2 = 0;
					Key2Flag = 0;
				}
				uart_printf("%Key3 times=%d\n",times2);	
			}
		}
		if((times2 == Maxnum)&&(Key2Flag == 2))
		{
			Hold_down2 = 1;
		}	
		if(43==key_value1)																						//key1   S1
		{
			if(P43 == 0)
			{
				times1++;
				Key1Flag = 0;
				if(times1>=Maxnum)
				{
					times1 =Maxnum;
				}
			}
			else
			{
				key_value1=0;
				if(times1<Maxnum)
				Key1Flag = 1;
				if((times1 <= Shortnum)&&(Key1Flag == 1))
				{
					ShortKey1 = 1;
					times1 = 0;
					Key1Flag = 0;
				}
				else if((times1 >Shortnum)&&(Key1Flag == 1))
				{
					LongKey1 = 1;
					times1 = 0;
					Key1Flag = 0;
				}
				uart_printf("%Key4 times=%d\n",times1);	
			}
		}		
}
#endif