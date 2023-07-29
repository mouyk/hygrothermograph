#ifndef _KEY_C_
#define _KEY_C_
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/ca51f_config.h"		
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"
#include "include/key.h"
#include "include/time.h"
#include "include/rtc.h"
#include "include/uart.h"
#include <intrins.h>

uint8_t key_value1,key_value2,key_value3,key_value4=0;
bit FahrenFlag = 0;        //华氏度标志
bit HourFlag = 0;          //小时制标志 0：24hour    1：12hour
uint8_t Interface = 0;         //界面

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
	P32	 = 1;													//开启背光
	}
}
/***********************************************************************************
函数名：		Key_Scanf
功能说明： 		按键扫描函数，判断按键长短之分
输入参数： 		无
返回值：		无
***********************************************************************************/
uint8_t ShortKey1 = 0,ShortKey2 = 0,ShortKey3 = 0,ShortKey4 = 0;
uint8_t LongKey1 = 0,LongKey2 = 0,LongKey3 = 0,LongKey4 = 0;
uint8_t Hold_down = 0;
uint16_t times4 = 0,times3 = 0,times2 = 0,times1 = 0;
uint8_t Key1Flag = 0,Key2Flag = 0,Key3Flag = 0,Key4Flag = 0;
void Key_Scanf(void)
{
		if(40==key_value4)																						//key4   S4     减号
		{
			if(P40 == 0)
			{
				times4++;
				if(times4>=Maxnum)
				{
					times4 =Maxnum;
					if(Key4Flag == 0)
					{
						Key4Flag = 1;
						LongKey4 = 1;
						if(Interface != 0)
							Hold_down = 1;
					}
				}
			}
			else
			{
				key_value4=0;
				if((times4 <= Shortnum)&&(Key4Flag == 0))
				{
					ShortKey4 = 1;
					times4 = 0;
					Key4Flag = 0;
				}
				else if((times4 >Shortnum)&&(Key4Flag == 0))
				{
					LongKey4 = 1;
					times4 = 0;
					Key4Flag = 0;
				}
				else if(times4 == Maxnum)
				{
					times4 = 0;
				}
			}
		}
		if(41==key_value3)																						//key3   S3        模式
		{
			if(P41 == 0)
			{
				times3++;
				if(times3>=Maxnum)
				{
					times3 =Maxnum;
					if(Key3Flag == 0)
					{
						Key3Flag = 1;
						LongKey3 = 1;
					}
				}
			}
			else
			{
				key_value3=0;
				if((times3 <= Shortnum)&&(Key3Flag == 0))
				{
					ShortKey3 = 1;
					times3 = 0;
					Key3Flag = 0;
				}
				else if((times3 >Shortnum)&&(Key3Flag == 0))
				{
					LongKey3 = 1;
					times3 = 0;
					Key3Flag = 0;
				}
				else if(times3 == Maxnum)
				{
					times3 = 0;
					Key3Flag = 0;
				}
			}
		}
		if(42==key_value2)																						//key2   S2        加号
		{
			if(P42 == 0)
			{
				times2++;
				if(times2>=Maxnum)
				{
					times2 =Maxnum;
					if(Key2Flag == 0)
					{
						Key2Flag = 1;
						LongKey2 = 1;
						if(Interface != 0)
							Hold_down = 1;
					}
				}
			}
			else
			{
				key_value2=0;
				if((times2 <= Shortnum)&&(Key2Flag == 0))
				{
					ShortKey2 = 1;
					times2 = 0;
					Key2Flag = 0;
				}
				else if((times2 >Shortnum)&&(Key2Flag == 0))
				{
					LongKey2 = 1;
					times2 = 0;
					Key2Flag = 0;
				}
				else if(times2 == Maxnum)
				{
					times2 = 0;
				}
			}
		}
		if(43==key_value1)																						//key1   S1          贪睡/背光
		{
			if(P43 == 0)
			{
				times1++;
				if(times1>=Maxnum)
				{
					times1 =Maxnum;
					if(Key1Flag == 0)
					{
						Key1Flag = 1;
						LongKey1 = 1;
					}
				}
			}
			else
			{
				key_value1=0;
				if((times1 <= Shortnum)&&(Key1Flag == 0))
				{
					ShortKey1 = 1;
					times1 = 0;
					Key1Flag = 0;
				}
				else if((times1 >Shortnum)&&(Key1Flag == 0))
				{
					LongKey1 = 1;
					times1 = 0;
					Key1Flag = 0;
				}
				else if(times1 == Maxnum)
				{
					times1 = 0;
				}
			}
		}	
}
/***********************************************************************************
函数名：		Key_HandleFunction
功能说明： 		按键处理函数，内包含按键逻辑
输入参数： 		无
返回值：		无
***********************************************************************************/
uint8_t ZigbeeFlag = 0,DelAlarmFlag = 0;
void Key_HandleFunction(void)
{
	uint8_t i = 0;
	uint8_t ALarmnum = 0;
	uint16_t shorttime = 0;
	if((ShortKey2 == 1)&&(Interface == 0))         //℃与℉切换
	{
		ShortKey2 = 0;
		FahrenFlag = ~FahrenFlag;
	}
	else if((ShortKey4 == 1)&&(Interface == 0))     //12hour与24hour切换
	{
		ShortKey4 = 0;
		HourFlag = ~HourFlag;
		RTC_Alarm_init(1,0,0,0);
	}
	if((ShortKey3 == 1)&&(Interface == 0))
	{
		ShortKey3 = 0;
		Interface = 1;
	}
	else if((LongKey3 == 1)&&(Interface == 0)&&(times2 == 0))
	{
		LongKey3 = 0;
		Interface = 2;
	}
	else if((LongKey3 ==1)&&(Interface == 2))
	{
		LongKey3 = 0;
		times3 = 0;
		Interface = 0;
		if(RTC_num <= 4)
		{
			RTC_num = 0;
			RTC_Array[5] = calendar.sec;
			RTC_Set(RTC_Array[0],RTC_Array[1],RTC_Array[2],RTC_Array[3],RTC_Array[4],RTC_Array[5]);
			for(i = 0;i <= 5;i++)
			{
				RTC_Array[i] = 0;
			}
		}
		else
		{
			ALarmnum = Alarm1 + Alarm2 + Alarm3;
			RTC_num = 0;
			RTC_AlarmHandle(ALarmnum);
			RTC_AlarmCompare(ALarmnum);
		}
	}
	
	if((((LongKey3 ==1)&&(LongKey2 == 1))||((LongKey3 ==1)&&(times2 > Shortnum))||((LongKey2 ==1)&&(times3 > Shortnum)))&&(Interface == 0))       //zigbee组网
	{
		LongKey3 = 0;
		LongKey2 = 0;
		times2 = 0;
		times3 = 0;
		ZigbeeFlag = 1;
	}
	else if((((LongKey3 ==1)&&(LongKey4 == 1))||((LongKey3 ==1)&&(times4 > Shortnum))||((LongKey4 ==1)&&(times3 > Shortnum)))&&(Interface == 2)) //
	{
		LongKey3 = 0;
		LongKey4 = 0;
		times4 = 0;
		times3 = 0;
		DelAlarmFlag = 1;
	}

	if((ShortKey3 == 1)&&(RTC_num != 5))
	{
		ShortKey3 = 0;
		if(Interface == 2)
		{
			RTC_num++;
			if(RTC_num > 5)
			{
				RTC_num = 5;
			}
			if(RTC_num == 5)
			uart_printf("RTC_num = %d\n",RTC_num);
		}
		else if(Interface == 1)
		{
			Timer_num++;
			shorttime = Timer_Array[0]*60 + Timer_Array[1];
			if((Timer_num > 2)&&(shorttime != 0))
			{
				Timer_num = 0;
				Time_start = 1;
			}
			else if((Timer_num > 2)&&(shorttime == 0))
			{
				Timer_num = 0;
				Time_start = 0;
				Interface = 0;
			}
		}
	}
	Key_timedate(RTC_num);
	Key_Alarm(RTC_num);
	Key_Countdown(Timer_num);
}
/***********************************************************************************
函数名：		Key_timedate
功能说明： 		按键在界面2时，对于相应数据的控制
输入参数： 		flag ：0~5表示不同闪烁位置的数字加减及范围控制
返回值：		无
***********************************************************************************/
void Key_timedate(uint8_t flag)
{
	uint8_t Days =0;
	if((LongKey4 == 1||ShortKey4 == 1)&&(Interface == 2)&&(flag != 5))
	{
		ShortKey4 = 0;
		LongKey4 = 0;
		if(flag == 0)
		{
			RTC_Array[3]--;
			if(RTC_Array[3] == -1)
			{
				RTC_Array[3]=23;
			}
		}
		else if(flag == 1)
		{
			RTC_Array[4]--;
			if(RTC_Array[4] == -1)
				RTC_Array[4] = 59;
		}
		else if(flag == 2)
		{
			RTC_Array[0]--;
			Days = RTC_Daysmonth(RTC_Array[0],RTC_Array[1]);
			if(RTC_Array[2]> Days)
				RTC_Array[2] = Days;
			if(RTC_Array[0] <= 1999)
				RTC_Array[0] = 2099;
		}
		else if(flag == 3)
		{
			RTC_Array[1]--;
			Days = RTC_Daysmonth(RTC_Array[0],RTC_Array[1]);
			if(RTC_Array[2]> Days)
				RTC_Array[2] = Days;
			if(RTC_Array[1] == 0)
				RTC_Array[1] = 12;
		}
		else if(flag == 4)
		{
			Days = RTC_Daysmonth(RTC_Array[0],RTC_Array[1]);
			RTC_Array[2]--;
			if(RTC_Array[2] == 0)
				RTC_Array[2] = Days;
		}
	}
	if((LongKey2== 1||ShortKey2 == 1)&&(Interface == 2)&&(flag != 5))
	{
		ShortKey2 = 0;
		LongKey2 = 0;
		if(flag == 0)
		{
			RTC_Array[3]++;
			if(RTC_Array[3]>23)
				RTC_Array[3] = 0;
		}
		else if(flag == 1)
		{
			RTC_Array[4]++;
			if(RTC_Array[4]>59)
				RTC_Array[4] = 0;
		}
		else if(flag == 2)
		{
			RTC_Array[0]++;
			Days = RTC_Daysmonth(RTC_Array[0],RTC_Array[1]);
			if(RTC_Array[2]> Days)
				RTC_Array[2] = Days;
			if(RTC_Array[0] >= 2100)
				RTC_Array[0] = 2000;
		}
		else if(flag == 3)
		{
			RTC_Array[1]++;
			Days = RTC_Daysmonth(RTC_Array[0],RTC_Array[1]);
			if(RTC_Array[2]> Days)
				RTC_Array[2] = Days;
			if(RTC_Array[1] > 12)
				RTC_Array[1] = 1;
		}
		else if(flag == 4)
		{
			Days = RTC_Daysmonth(RTC_Array[0],RTC_Array[1]);
			RTC_Array[2]++;
			if(RTC_Array[2] > Days)
				RTC_Array[2] = 1;
		}
	}
}
void Key_Countdown(uint8_t flag)
{
	if((LongKey4 == 1||ShortKey4 == 1)&&(Interface == 1))
	{
		ShortKey4 = 0;
		LongKey4 = 0;
		if(flag == 1)
		{
			Timer_Array[0]--;
			if(Timer_Array[0] == -1)
			{
				Timer_Array[0] = 60;
			}		
			
		}
		else if(flag == 2)
		{
			Timer_Array[1]--;
			if((Timer_Array[1] == -1)&&(Timer_Array[0] < 60))
			{
				Timer_Array[1] = 59;
			}
			else if((Timer_Array[1] == -1)&&(Timer_Array[0] == 60))
			{
				Timer_Array[1] = 0;
			}
		}
	}
	if((LongKey2 == 1||ShortKey2 == 1)&&(Interface == 1))
	{
		ShortKey2 = 0;
		LongKey2 = 0;
		if(flag == 1)
		{
			Timer_Array[0]++;
			if(Timer_Array[0] > 60)
			{
				Timer_Array[0] = 0;
			}			
		}
		else if(flag == 2)
		{
			Timer_Array[1]++;
			if((Timer_Array[0] == 60)&&(Timer_Array[1] >= 1))
			{
				Timer_Array[1] = 0;
			}
			else if((Timer_Array[0] < 60)&&(Timer_Array[1] >= 60))
			{
				Timer_Array[1] = 0;
			}
		}
	}
}
void Key_Alarm(uint8_t flag)
{
	if(flag == 5)
	{
		if(ShortKey3 == 1)
		{
			ShortKey3 = 0;
			Alarm_flag++;
			if(Alarm_flag > 2)
			{
				Alarm_flag = 0;
				if(Alarm_num == 0)
					Alarm1 =1;
				else if(Alarm_num == 1)
					Alarm2 =1;
				else if(Alarm_num == 2)
					Alarm3 =1;
			}
		}
		if(ShortKey4 == 1)
		{
			ShortKey4 = 0;
			if(Alarm_flag == 0)
			{
				Alarm_num--;
				if(Alarm_num <= -1)
				{
					Alarm_num = 0;
				}
			}
			else if(Alarm_flag == 1)
			{
				if(Alarm_num == 0)
				{
					Alarm_Array[0]--;
					if(Alarm_Array[0] <= -1)
					{
						Alarm_Array[0] = 23;
					}
				}
				else if(Alarm_num == 1)
				{
					Alarm_Array[2]--;
					if(Alarm_Array[2] <= -1)
					{
						Alarm_Array[2] = 23;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[4]--;
					if(Alarm_Array[4] <= -1)
					{
						Alarm_Array[4] = 23;
					}
				}
			}
			else if(Alarm_flag == 2)
			{
				if(Alarm_num == 0)
				{
					Alarm_Array[1]--;
					if(Alarm_Array[1] <= -1)
					{
						Alarm_Array[1] = 59;
					}
				}
				else if(Alarm_num == 1)
				{
					Alarm_Array[3]--;
					if(Alarm_Array[3] <= -1)
					{
						Alarm_Array[3] = 59;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[5]--;
					if(Alarm_Array[5] <= -1)
					{
						Alarm_Array[5] = 59;
					}
				}
			}
		}
		if(ShortKey2 == 1)
		{
			ShortKey2 = 0;
			if(Alarm_flag == 0)
			{
				Alarm_num++;
				if(Alarm_num >= 3)
				{
					Alarm_num = 2;
				}
			}
			else if(Alarm_flag == 1)
			{
				if(Alarm_num == 0)
				{
					Alarm_Array[0]++;
					if(Alarm_Array[0] >= 24)
					{
						Alarm_Array[0] = 0;
					}
				}
				else if(Alarm_num == 1)
				{
					Alarm_Array[2]++;
					if(Alarm_Array[2] >= 24)
					{
						Alarm_Array[2] = 0;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[4]++;
					if(Alarm_Array[4] >= 24)
					{
						Alarm_Array[4] = 0;
					}
				}
			}
			else if(Alarm_flag == 2)
			{
				if(Alarm_num == 0)
				{
					Alarm_Array[1]++;
					if(Alarm_Array[1] >= 60)
					{
						Alarm_Array[1] = 0;
					}
				}
				else if(Alarm_num == 1)
				{
					Alarm_Array[3]++;
					if(Alarm_Array[3] >= 60)
					{
						Alarm_Array[3] = 0;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[5]++;
					if(Alarm_Array[5] >= 60)
					{
						Alarm_Array[5] = 0;
					}
				}
			}
		}
	}
}

#endif
