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
#include "include/pwm.h"
#include "include/buzzer.h"
#include "include/uart.h"
#include <intrins.h>

//uint8_t key_value1,key_value2,key_value3,key_value4=0;
bit FahrenFlag = 0;        //华氏度标志
bit HourFlag = 0;          //小时制标志 0：24hour    1：12hour
uint8_t Interface = 0;         //界面
Key Key1,Key2,Key3,Key4;
xdata Key Key1=/*!< 默认设备参数*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
xdata Key Key2=/*!< 默认设备参数*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
xdata Key Key3=/*!< 默认设备参数*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
xdata Key Key4=/*!< 默认设备参数*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
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
	Key4.key_value = 0;		
}
void INT2_ISR (void) interrupt 7
{
	if(EPIF & 0x01)
	{
		EPIF = 0x01;
//		int2_flag = 1;
		Key4.key_value =40;
		keyclearnum = 0;
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
	Key3.key_value = 0;	
}
void INT3_ISR (void) interrupt 8
{
	if(EPIF & 0x02)
	{
		EPIF = 0x02;	
		Key3.key_value = 41;	
		keyclearnum = 0;
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
	Key2.key_value = 0;	
}
void INT4_ISR (void) interrupt 9
{
	if(EPIF & 0x04)
	{
		EPIF = 0x04;
		Key2.key_value = 42;	
		keyclearnum = 0;
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
	Key1.key_value = 0;	
}
void INT5_ISR (void) interrupt 10
{
	if(EPIF & 0x08)
	{
		EPIF = 0x08;	
		Key1.key_value = 43;
		P32	 = 1;													//开启背光
		keyclearnum = 0;
	}
}
/***********************************************************************************
函数名：		Key_Scanf
功能说明： 		按键扫描函数，判断按键长短之分
输入参数： 		无
返回值：		无
***********************************************************************************/
uint8_t Hold_down = 0;
void Key_Scanf(void)
{
		if(40==Key4.key_value)																						//key4   S4     减号
		{
			if(P40 == 0)
			{
				if(DelAlarmFlag == 0)
					Key4.times = Key4.times + 1;
				if(Key4.times>=Maxnum)
				{
					Key4.times =Maxnum;
					if(Key4.KeyFlag == 0)
					{
						Key4.KeyFlag = 1;
						Key4.LongKey = 1;
						keyclearnum = 0;
						if(Interface != 0)
							Hold_down = 1;
					}
				}
			}
			else
			{
				Key4.key_value=0;
				keyclearnum = 0;
				if((Key4.times <= Shortnum)&&(Key4.KeyFlag == 0)&&(Key4.times != 0))
				{
					Key4.ShortKey = 1;
					Key4.times = 0;
				}
				else if((Key4.times >Shortnum)&&(Key4.KeyFlag == 0))
				{
					Key4.LongKey = 1;
					Key4.times = 0;
				}
				else if(Key4.times == Maxnum);
				{
					Key4.times = 0;
					Key4.KeyFlag = 0;
				}
			}
		}
		if(41==Key3.key_value)																						//key3   S3        模式
		{
			if(P41 == 0)
			{
				if((ZigbeeFlag == 0)&&(DelAlarmFlag == 0))
				{
					Key3.times = Key3.times + 1;
				}
				if(Key3.times>=Maxnum)
				{
					Key3.times =Maxnum;
					if(Key3.KeyFlag == 0)
					{
						Key3.KeyFlag = 1;
						keyclearnum = 0;
						Key3.LongKey = 1;
					}
				}
			}
			else
			{
				Key3.key_value=0;
				keyclearnum = 0;
				DelAlarmFlag = 0;
				if((Key3.times <= Shortnum)&&(Key3.KeyFlag == 0)&&(Key3.times != 0))
				{
					Key3.ShortKey = 1;
					Key3.times = 0;
				}
				else if((Key3.times >Shortnum)&&(Key3.KeyFlag == 0))
				{
					Key3.LongKey = 1;
					Key3.times = 0;
				}
				else if(Key3.times == Maxnum)
				{
					Key3.times = 0;
					Key3.KeyFlag = 0;
				}
			}
		}
		if(42==Key2.key_value)																						//key2   S2        加号
		{
			if(P42 == 0)
			{
				if(ZigbeeFlag == 0)
					Key2.times = Key2.times + 1;
				if(Key2.times>=Maxnum)
				{
					Key2.times =Maxnum;
					if(Key2.KeyFlag == 0)
					{
						Key2.KeyFlag = 1;
						Key2.LongKey = 1;
						keyclearnum = 0;
						if(Interface != 0)
							Hold_down = 1;
					}
				}
			}
			else
			{
				Key2.key_value=0;
				keyclearnum = 0;
				if((Key2.times <= Shortnum)&&(Key2.KeyFlag == 0)&&(Key2.times != 0))
				{
					Key2.ShortKey = 1;
					Key2.times = 0;
				}
				else if((Key2.times >Shortnum)&&(Key2.KeyFlag == 0))
				{
					Key2.LongKey = 1;
					Key2.times = 0;
				}
				else if(Key2.times == Maxnum)
				{
					Key2.times = 0;
					Key2.KeyFlag = 0;
				}
			}
		}
		if(43==Key1.key_value)																						//key1   S1          贪睡/背光
		{
			if(P43 == 0)
			{
				Key1.times = Key1.times + 1;
				if(Key1.times>=Maxnum)
				{
					Key1.times =Maxnum;
					if(Key1.KeyFlag == 0)
					{
						Key1.KeyFlag = 1;
						Key1.LongKey = 1;
						keyclearnum = 0;
					}
				}
			}
			else
			{
				Key1.key_value=0;
				keyclearnum = 0;
				if((Key1.times <= Shortnum)&&(Key1.KeyFlag == 0))
				{
					Key1.ShortKey = 1;
					Key1.times = 0;
				}
				else if((Key1.times >Shortnum)&&(Key1.KeyFlag == 0))
				{
					Key1.LongKey = 1;
					Key1.times = 0;
				}
				else if(Key1.times == Maxnum)
				{
					Key1.times = 0;
					Key1.KeyFlag = 0;
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
	if((Key2.ShortKey == 1)&&(Interface == 0))         //℃与℉切换
	{
		Key2.ShortKey = 0;
		FahrenFlag = ~FahrenFlag;
	}
	else if((Key4.ShortKey == 1)&&(Interface == 0))     //12hour与24hour切换
	{
		Key4.ShortKey = 0;
		HourFlag = ~HourFlag;
	}
	if((Key3.ShortKey == 1)&&(Interface == 0))
	{
		Key3.ShortKey = 0;
		Interface = 1;
	}
	else if((Key3.LongKey == 1)&&(Interface == 0)&&(Key2.times == 0))
	{
		Key3.LongKey = 0;
		Interface = 2;
	}
	else if((Key3.LongKey ==1)&&(Interface == 2)&&(Key4.times == 0))
	{
		Key3.LongKey = 0;
		Key3.KeyFlag = 0;
		Key3.times = 0;
		Interface = 0;
		if(RTC_num <= RTC_Status_Data)
		{
			RTC_num = RTC_Status_Hour;
			RTC_Array.sec = calendar.sec;
			RTC_Set(RTC_Array.w_year,RTC_Array.w_month,RTC_Array.w_date,RTC_Array.hour,RTC_Array.min,RTC_Array.sec);
//			for(i = 0;i <= 5;i++)
//			{
//				RTC_Array[i] = 0;
//			}
		}
		else
		{
			ALarmnum = Alarm1 + Alarm2 + Alarm3;
			RTC_num = RTC_Status_Hour;
			Alarm_flag = 0;
			Alarm_num = 1;
			RTC_AlarmHandle(ALarmnum);
			RTC_AlarmCompare(ALarmnum);
		}
	}
		
	if((((Key3.LongKey ==1)&&(Key2.LongKey == 1))||((Key3.LongKey ==1)&&(Key2.times > Shortnum))||((Key2.LongKey ==1)&&(Key3.times > Shortnum)))&&(Interface == 0))       //zigbee组网
	{
		Key3.LongKey = 0;
		Key2.LongKey = 0;
		Key2.times = 0;
		Key3.times = 0;
		Key2.KeyFlag = 0;
		Key3.KeyFlag = 0;
		ZigbeeFlag = 1;
		
	}
	else if((((Key3.LongKey ==1)&&(Key4.LongKey == 1))||((Key3.LongKey ==1)&&(Key4.times > Shortnum))||((Key4.LongKey ==1)&&(Key3.times > Shortnum)))&&(Interface == 2)) //
	{
		Key3.LongKey = 0;
		Key4.LongKey = 0;
		Key4.times = 0;
		Key3.times = 0;
		Key4.KeyFlag = 0;
		Key3.KeyFlag = 0;
		DelAlarmFlag = 1;
	}

	if((Key3.ShortKey == 1)&&(RTC_num != RTC_Status_Alarm))
	{
		Key3.ShortKey = 0;
		if(Interface == 2)
		{
			RTC_num++;
			if(RTC_num > RTC_Status_Alarm)
			{
				RTC_num = RTC_Status_Alarm;
			}
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
	if(AlarmEvFlag == 1)
	{
		if(Key1.ShortKey == 1)
		{
			Key1.ShortKey = 0;
			BuzNum = 0;
			BuzNum1 = 0;
			AlarmEvFlag = 0;
			BeepStart = 0;
			PWMEN  = ~(1<<PWM_CH6);		//PWM6禁用
			sleepnum++;
			if(sleepnum > 3)
			{
				sleepnum = 0;
				RTC_AlarmCompare(Alarm1+Alarm2+Alarm3);
			}
			else
			{
				calendar.min = calendar.min +5;
				if(calendar.min >= 60)
				{
					calendar.min = calendar.min -60;
					calendar.hour = calendar.hour + 1;
					if(calendar.hour >= 24)
					{
						calendar.hour = calendar.hour -24;
					}
				}
				RTC_Alarm_init(1,calendar.hour,calendar.min,calendar.sec);
			}
		}
		if(Key1.LongKey == 1)
		{
			sleepnum = 0;
			Key1.LongKey = 0;
			BuzNum = 0;
			BuzNum1 = 0;
			AlarmEvFlag = 0;
			BeepStart = 0;
			PWMEN  = ~(1<<PWM_CH6);		//PWM6禁用
			RTC_AlarmCompare(Alarm1+Alarm2+Alarm3);
		}
	}
	else
	{
		Key1.ShortKey = 0;
		Key1.LongKey = 0;
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
	if((Key4.LongKey == 1||Key4.ShortKey == 1)&&(Interface == 2)&&(flag != RTC_Status_Alarm))
	{
		Key4.ShortKey = 0;
		Key4.LongKey = 0;
		if(flag == RTC_Status_Hour)
		{
			RTC_Array.hour--;
			if(RTC_Array.hour == -1)
			{
				RTC_Array.hour=23;
			}
		}
		else if(flag == RTC_Status_Min)
		{
			RTC_Array.min--;
			if(RTC_Array.min == -1)
				RTC_Array.min = 59;
		}
		else if(flag == RTC_Status_Year)
		{
			RTC_Array.w_year--;
			Days = RTC_Daysmonth(RTC_Array.w_year,RTC_Array.w_month);
			if(RTC_Array.w_date> Days)
				RTC_Array.w_date = Days;
			if(RTC_Array.w_year <= 1999)
				RTC_Array.w_year = 2099;
		}
		else if(flag == RTC_Status_Mon)
		{
			RTC_Array.w_month--;
			Days = RTC_Daysmonth(RTC_Array.w_year,RTC_Array.w_month);
			if(RTC_Array.w_date> Days)
				RTC_Array.w_date = Days;
			if(RTC_Array.w_month == 0)
				RTC_Array.w_month = 12;
		}
		else if(flag == RTC_Status_Data)
		{
			Days = RTC_Daysmonth(RTC_Array.w_year,RTC_Array.w_month);
			RTC_Array.w_date--;
			if(RTC_Array.w_date == 0)
				RTC_Array.w_date = Days;
		}
	}
	if((Key2.LongKey== 1||Key2.ShortKey == 1)&&(Interface == 2)&&(flag != RTC_Status_Alarm))
	{
		Key2.ShortKey = 0;
		Key2.LongKey = 0;
		if(flag == RTC_Status_Hour)
		{
			RTC_Array.hour++;
			if(RTC_Array.hour>23)
				RTC_Array.hour = 0;
		}
		else if(flag == RTC_Status_Min)
		{
			RTC_Array.min++;
			if(RTC_Array.min>59)
				RTC_Array.min = 0;
		}
		else if(flag == RTC_Status_Year)
		{
			RTC_Array.w_year++;
			Days = RTC_Daysmonth(RTC_Array.w_year,RTC_Array.w_month);
			if(RTC_Array.w_date> Days)
				RTC_Array.w_date = Days;
			if(RTC_Array.w_year >= 2100)
				RTC_Array.w_year = 2000;
		}
		else if(flag == RTC_Status_Mon)
		{
			RTC_Array.w_month++;
			Days = RTC_Daysmonth(RTC_Array.w_year,RTC_Array.w_month);
			if(RTC_Array.w_date> Days)
				RTC_Array.w_date = Days;
			if(RTC_Array.w_month > 12)
				RTC_Array.w_month = 1;
		}
		else if(flag == RTC_Status_Data)
		{
			Days = RTC_Daysmonth(RTC_Array.w_year,RTC_Array.w_month);
			RTC_Array.w_date++;
			if(RTC_Array.w_date > Days)
				RTC_Array.w_date = 1;
		}
	}
}
void Key_Countdown(uint8_t flag)
{
	if((Key4.LongKey == 1||Key4.ShortKey == 1)&&(Interface == 1))
	{
		Key4.ShortKey = 0;
		Key4.LongKey = 0;
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
	if((Key2.LongKey == 1||Key2.ShortKey == 1)&&(Interface == 1))
	{
		Key2.ShortKey = 0;
		Key2.LongKey = 0;
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
	if(flag == RTC_Status_Alarm)
	{
		if(Key3.ShortKey == 1)
		{
			Key3.ShortKey = 0;
			Alarm_flag++;
			if(Alarm_flag > 0)
			{
				if(Alarm_num == 1)
					Alarm1 =1;
				else if(Alarm_num == 2)
					Alarm2 =1;
				else if(Alarm_num == 3)
					Alarm3 =1;
			}
			if(Alarm_flag > 2)
			{
				Alarm_flag = 0;
			}
		}
		if(Key4.LongKey == 1||Key4.ShortKey == 1)
		{
			Key4.ShortKey = 0;
			Key4.LongKey =0;
			if(Alarm_flag == 0)
			{
				Alarm_num--;
				if(Alarm_num <= 0)
				{
					Alarm_num = 1;
				}
			}
			else if(Alarm_flag == 1)
			{
				if(Alarm_num == 1)
				{
					Alarm_Array[0]--;
					if(Alarm_Array[0] <= -1)
					{
						Alarm_Array[0] = 23;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[2]--;
					if(Alarm_Array[2] <= -1)
					{
						Alarm_Array[2] = 23;
					}
				}
				else if(Alarm_num == 3)
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
				if(Alarm_num == 1)
				{
					Alarm_Array[1]--;
					if(Alarm_Array[1] <= -1)
					{
						Alarm_Array[1] = 59;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[3]--;
					if(Alarm_Array[3] <= -1)
					{
						Alarm_Array[3] = 59;
					}
				}
				else if(Alarm_num == 3)
				{
					Alarm_Array[5]--;
					if(Alarm_Array[5] <= -1)
					{
						Alarm_Array[5] = 59;
					}
				}
			}
		}
		if(Key2.LongKey == 1||Key2.ShortKey == 1)
		{
			Key2.ShortKey = 0;
			Key2.LongKey = 0;
			if(Alarm_flag == 0)
			{
				Alarm_num++;
				if(Alarm_num >= 4)
				{
					Alarm_num = 3;
				}
			}
			else if(Alarm_flag == 1)
			{
				if(Alarm_num == 1)
				{
					Alarm_Array[0]++;
					if(Alarm_Array[0] >= 24)
					{
						Alarm_Array[0] = 0;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[2]++;
					if(Alarm_Array[2] >= 24)
					{
						Alarm_Array[2] = 0;
					}
				}
				else if(Alarm_num == 3)
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
				if(Alarm_num == 1)
				{
					Alarm_Array[1]++;
					if(Alarm_Array[1] >= 60)
					{
						Alarm_Array[1] = 0;
					}
				}
				else if(Alarm_num == 2)
				{
					Alarm_Array[3]++;
					if(Alarm_Array[3] >= 60)
					{
						Alarm_Array[3] = 0;
					}
				}
				else if(Alarm_num == 3)
				{
					Alarm_Array[5]++;
					if(Alarm_Array[5] >= 60)
					{
						Alarm_Array[5] = 0;
					}
				}
			}
		}
		if(DelAlarmFlag == 1)
		{
			if(Alarm_num == 1)
			{
				Alarm1 =0;
				Alarm_Array[0] = 0;
				Alarm_Array[1] = 0;
			}
			else if(Alarm_num == 2)
			{
				Alarm2 =0;
				Alarm_Array[2] = 0;
				Alarm_Array[3] = 0;
			}
			else if(Alarm_num == 3)
			{
				Alarm3 =0;
				Alarm_Array[4] = 0;
				Alarm_Array[5] = 0;
			}
		}
	}
}
uint8_t keyclearnum = 0;
void Key_RockonTime(void)
{
	uint8_t i =0;
	keyclearnum++;
	if(keyclearnum>= 10)
	{
		keyclearnum = 0;
		if((Interface == 1)&&(Time_start == 0))
		{
			Interface = 0;
			Timer_num = 0;
			for(i = 0;i <= 1;i++)
			{
				Timer_Array[i] = 0;
			}
		}
		else if(Interface == 2)
		{
			Interface = 0;
			if(RTC_num == RTC_Status_Alarm)
			{
				Alarm1 = LastAlarm1;
				Alarm2 = LastAlarm2;
				Alarm3 = LastAlarm3;
				for(i = 0;i <= 5;i++)
				{
					Alarm_Array[i] = LastAlarm_Array[i];
				}
			}
			RTC_num = RTC_Status_Hour;

		}
	}
}
#endif
#endif
