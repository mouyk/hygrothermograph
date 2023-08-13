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

bit FahrenFlag = 0;        //���϶ȱ�־
bit HourFlag = 0;          //Сʱ�Ʊ�־ 0��24hour    1��12hour
uint8_t Interface = 0;         //����
Key Key1,Key2,Key3,Key4;
xdata Key Key1=/*!< Key1Ĭ���豸����*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
xdata Key Key2=/*!< Key2Ĭ���豸����*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
xdata Key Key3=/*!< Key3Ĭ���豸����*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
xdata Key Key4=/*!< Key4Ĭ���豸����*/
{ 
	0,
	0,
	0,  
	0,
	0,
};
//EPCON�Ĵ�������
#define EPPL(N)	(N<<7)
/*****************************************************************************
 ** \brief	 KEY_Init	
			 KEY_Init��ʼ��GPIO����,���簴������
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void KEY_init(void)
{
/*���� ��������	P40	| S4	P41	| S3	P42	| S2	P43	| S1	*/
	
	P40F = INPUT|PU_EN;				//P40����Ϊ����ģʽ��ʹ���������� 
	P40C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P40C &= ~0x20;					//��������ѡ��Ϊ������	
	
	P41F = INPUT|PU_EN;				//P41����Ϊ����ģʽ��ʹ���������� 
	P41C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P41C &= ~0x20;					//��������ѡ��Ϊ������	
	
	P42F = INPUT|PU_EN;				//P42����Ϊ����ģʽ��ʹ���������� 
	P42C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P42C &= ~0x20;					//��������ѡ��Ϊ������		
	
	P43F = INPUT|PU_EN;				//P43����Ϊ����ģʽ��ʹ���������� 
	P43C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P43C &= ~0x20;					//��������ѡ��Ϊ������
}

/*�ⲿ�ж�2��������****************************************************************************************************/
void INT2_Init(void)
{
//	P10F = INPUT;							//P10����Ϊ����ģʽ
	INDEX = 0;								//EPCONΪ�������ļĴ���������INDEX=0 ��ӦINT2
	EPCON = EPPL(1) | 32;					//����P40ΪINT2�ж����ţ��½��ش���
	INT2EN = 1; 							//�ⲿ�ж�2�ж�ʹ��
	EPIE |= 0x01;							//INT2�ж�ʹ��
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

/*�ⲿ�ж�3��������****************************************************************************************************/
void INT3_Init(void)
{
//	P11F = INPUT;							//P11����Ϊ����ģʽ	
	INDEX = 1;								//INDEXΪ�������ļĴ���������INDEX=1 ��ӦINT3
	EPCON = EPPL(1) | 33;					//����P41ΪINT3�ж����ţ��½��ش���		
	INT3EN = 1; 							//�ⲿ�ж�3�ж�ʹ�� 
	EPIE |= 0x02;							//INT3�ж�ʹ��
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

/*�ⲿ�ж�4��������****************************************************************************************************/
void INT4_Init(void)
{
//	P12F = INPUT;							//P12����Ϊ����ģʽ		
	INDEX = 2;								//INDEXΪ�������ļĴ���������INDEX=2 ��ӦINT4
	EPCON = EPPL(1) | 34;					//����P42ΪINT4�ж����ţ��½��ش���				
	INT4EN = 1;  							//�ⲿ�ж�4�ж�ʹ��
	EPIE |= 0x04;							//INT4�ж�ʹ��
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

/*�ⲿ�ж�5��������****************************************************************************************************/
void INT5_Init(void)
{
//	P13F = INPUT;							//P13����Ϊ����ģʽ		
	INDEX = 3;								//INDEXΪ�������ļĴ���������INDEX=3 ��ӦINT5
	EPCON = EPPL(1) | 35;					//����P43ΪINT5�ж����ţ��½��ش���		
	INT5EN = 1; 							//�ⲿ�ж�5�ж�ʹ�� 
	EPIE |= 0x08;							//INT5�ж�ʹ��
	Key1.key_value = 0;	
}
void INT5_ISR (void) interrupt 10
{
	if(EPIF & 0x08)
	{
		EPIF = 0x08;	
		Key1.key_value = 43;
		P32	 = 1;													//��������
		keyclearnum = 0;
	}
}
/***********************************************************************************
��������		Key_Scanf
����˵���� 		����ɨ�躯�����жϰ�������֮��
��������� 		��
����ֵ��		��
***********************************************************************************/
bit menulock = 0, Hold_down = 0;

void Key_Scanf(void)
{
		if(40==Key4.key_value)																						//key4   S4     ����
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
		if(41==Key3.key_value)																						//key3   S3        ģʽ
		{
			if(P41 == 0)
			{
				if((ZigbeeFlag == 0)&&(DelAlarmFlag == 0)&&(menulock == 0))
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
				menulock = 0;
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
		if(42==Key2.key_value)																						//key2   S2        �Ӻ�
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
		if(43==Key1.key_value)																						//key1   S1          ̰˯/����
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
��������		Key_HandleFunction
����˵���� 		�������������ڰ��������߼�
��������� 		��
����ֵ��		��
***********************************************************************************/
bit ZigbeeFlag = 0,DelAlarmFlag = 0;
void Key_HandleFunction(void)
{
	uint8_t i = 0;
	uint8_t ALarmnum = 0;
	uint16_t shorttime = 0;
	if((Key2.ShortKey == 1)&&(Interface == 0))         //����H�л�
	{
		Key2.ShortKey = 0;
		FahrenFlag = ~FahrenFlag;
	}
	else if((Key4.ShortKey == 1)&&(Interface == 0))     //12hour��24hour�л�
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
		menulock = 1;
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
			ALarmnum = AlarmTimes.Alarmnum.Alarm1 + AlarmTimes.Alarmnum.Alarm2 + AlarmTimes.Alarmnum.Alarm3;
			RTC_num = RTC_Status_Hour;
			AlarmTimes.Alarm_flag = 0;
			AlarmTimes.Alarm_num = 1;
			RTC_AlarmHandle(ALarmnum);
			RTC_AlarmCompare(ALarmnum);
		}
	}
		
	if((((Key3.LongKey ==1)&&(Key2.LongKey == 1))||((Key3.LongKey ==1)&&(Key2.times > Shortnum))||((Key2.LongKey ==1)&&(Key3.times > Shortnum)))&&(Interface == 0))       //zigbee����
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
			shorttime = Timer_Array.min*60 + Timer_Array.sec;
			if((Timer_num > Timer_Min)&&(shorttime != 0))
			{
				Timer_num = Timer_Ico;
				Time_start = 1;
			}
			else if((Timer_num > Timer_Min)&&(shorttime == 0))
			{
				Timer_num = Timer_Ico;
				Time_start = 0;
				Interface = 0;
			}
		}
	}
	if(AlarmEvFlag == 1)																																									//���������󰴼�����
	{
		if(Key1.ShortKey == 1)
		{
			Key1.ShortKey = 0;
			BuzNum = 0;
			BuzNum1 = 0;
			AlarmEvFlag = 0;
			BeepStart = 0;
			PWMEN  = ~(1<<PWM_CH6);		//PWM6����
			AlarmTimes.sleepnum++;
			if(AlarmTimes.sleepnum > 3)
			{
				AlarmTimes.sleepnum = 0;
				RTC_AlarmCompare(AlarmTimes.Alarmnum.Alarm1+AlarmTimes.Alarmnum.Alarm2+AlarmTimes.Alarmnum.Alarm3);
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
			AlarmTimes.sleepnum = 0;
			Key1.LongKey = 0;
			BuzNum = 0;
			BuzNum1 = 0;
			AlarmEvFlag = 0;
			BeepStart = 0;
			PWMEN  = ~(1<<PWM_CH6);		//PWM6����
			RTC_AlarmCompare(AlarmTimes.Alarmnum.Alarm1+AlarmTimes.Alarmnum.Alarm2+AlarmTimes.Alarmnum.Alarm3);
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
��������		Key_timedate
����˵���� 		�����ڽ���2ʱ��������Ӧ���ݵĿ���
��������� 		flag ��0~5��ʾ��ͬ��˸λ�õ����ּӼ�����Χ����
����ֵ��		��
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
/***********************************************************************************
��������		Key_Countdown
����˵���� 		�����Ӽ��Լ�ʱ���ܵĿ���
��������� 		flag��1��ʱ��2����
����ֵ��		��
***********************************************************************************/
void Key_Countdown(uint8_t flag)
{
	if((Key4.LongKey == 1||Key4.ShortKey == 1)&&(Interface == 1))
	{
		Key4.ShortKey = 0;
		Key4.LongKey = 0;
		if(flag == Timer_Hour)
		{
			Timer_Array.min--;
			if(Timer_Array.min == -1)
			{
				Timer_Array.min = 60;
			}		
			
		}
		else if(flag == Timer_Min)
		{
			Timer_Array.sec--;
			if((Timer_Array.sec == -1)&&(Timer_Array.min < 60))
			{
				Timer_Array.sec = 59;
			}
			else if((Timer_Array.sec == -1)&&(Timer_Array.min == 60))
			{
				Timer_Array.sec = 0;
			}
		}
	}
	if((Key2.LongKey == 1||Key2.ShortKey == 1)&&(Interface == 1))
	{
		Key2.ShortKey = 0;
		Key2.LongKey = 0;
		if(flag == Timer_Hour)
		{
			Timer_Array.min++;
			if(Timer_Array.min > 60)
			{
				Timer_Array.min = 0;
			}			
		}
		else if(flag == Timer_Min)
		{
			Timer_Array.sec++;
			if((Timer_Array.min == 60)&&(Timer_Array.sec >= 1))
			{
				Timer_Array.sec = 0;
			}
			else if((Timer_Array.min < 60)&&(Timer_Array.sec >= 60))
			{
				Timer_Array.sec = 0;
			}
		}
	}
}
/***********************************************************************************
��������		Key_Alarm
����˵���� 		�����Ӽ������ӹ��ܵĿ���
��������� 		flag��5�����ӽ���
����ֵ��		��
***********************************************************************************/
void Key_Alarm(uint8_t flag)
{
	if(flag == RTC_Status_Alarm)
	{
		if(Key3.ShortKey == 1)
		{
			Key3.ShortKey = 0;
			AlarmTimes.Alarm_flag++;
			if(AlarmTimes.Alarm_flag > 0)
			{
				if(AlarmTimes.Alarm_num == 1)
					AlarmTimes.Alarmnum.Alarm1 =1;
				else if(AlarmTimes.Alarm_num == 2)
					AlarmTimes.Alarmnum.Alarm2 =1;
				else if(AlarmTimes.Alarm_num == 3)
					AlarmTimes.Alarmnum.Alarm3 =1;
			}
			if(AlarmTimes.Alarm_flag > 2)
			{
				AlarmTimes.Alarm_flag = 0;
			}
		}
		if(Key4.LongKey == 1||Key4.ShortKey == 1)
		{
			Key4.ShortKey = 0;
			Key4.LongKey =0;
			if(AlarmTimes.Alarm_flag == 0)
			{
				AlarmTimes.Alarm_num--;
				if(AlarmTimes.Alarm_num <= 0)
				{
					AlarmTimes.Alarm_num = 1;
				}
			}
			else if(AlarmTimes.Alarm_flag == 1)
			{
				if(AlarmTimes.Alarm_num == 1)
				{
					AlarmTimes.Alarm_Array[0].hour--;
					if(AlarmTimes.Alarm_Array[0].hour <= -1)
					{
						AlarmTimes.Alarm_Array[0].hour = 23;
					}
				}
				else if(AlarmTimes.Alarm_num == 2)
				{
					AlarmTimes.Alarm_Array[2].hour--;
					if(AlarmTimes.Alarm_Array[2].hour <= -1)
					{
						AlarmTimes.Alarm_Array[2].hour = 23;
					}
				}
				else if(AlarmTimes.Alarm_num == 3)
				{
					AlarmTimes.Alarm_Array[2].hour--;
					if(AlarmTimes.Alarm_Array[2].hour <= -1)
					{
						AlarmTimes.Alarm_Array[2].hour = 23;
					}
				}
			}
			else if(AlarmTimes.Alarm_flag == 2)
			{
				if(AlarmTimes.Alarm_num == 1)
				{
					AlarmTimes.Alarm_Array[0].min--;
					if(AlarmTimes.Alarm_Array[0].min <= -1)
					{
						AlarmTimes.Alarm_Array[0].min = 59;
					}
				}
				else if(AlarmTimes.Alarm_num == 2)
				{
					AlarmTimes.Alarm_Array[1].min--;
					if(AlarmTimes.Alarm_Array[1].min <= -1)
					{
						AlarmTimes.Alarm_Array[1].min = 59;
					}
				}
				else if(AlarmTimes.Alarm_num == 3)
				{
					AlarmTimes.Alarm_Array[2].min--;
					if(AlarmTimes.Alarm_Array[2].min <= -1)
					{
						AlarmTimes.Alarm_Array[2].min = 59;
					}
				}
			}
		}
		if(Key2.LongKey == 1||Key2.ShortKey == 1)
		{
			Key2.ShortKey = 0;
			Key2.LongKey = 0;
			if(AlarmTimes.Alarm_flag == 0)
			{
				AlarmTimes.Alarm_num++;
				if(AlarmTimes.Alarm_num >= 4)
				{
					AlarmTimes.Alarm_num = 3;
				}
			}
			else if(AlarmTimes.Alarm_flag == 1)
			{
				if(AlarmTimes.Alarm_num == 1)
				{
					AlarmTimes.Alarm_Array[0].hour++;
					if(AlarmTimes.Alarm_Array[0].hour >= 24)
					{
						AlarmTimes.Alarm_Array[0].hour = 0;
					}
				}
				else if(AlarmTimes.Alarm_num == 2)
				{
					AlarmTimes.Alarm_Array[2].hour++;
					if(AlarmTimes.Alarm_Array[2].hour >= 24)
					{
						AlarmTimes.Alarm_Array[2].hour = 0;
					}
				}
				else if(AlarmTimes.Alarm_num == 3)
				{
					AlarmTimes.Alarm_Array[2].hour++;
					if(AlarmTimes.Alarm_Array[2].hour >= 24)
					{
						AlarmTimes.Alarm_Array[2].hour = 0;
					}
				}
			}
			else if(AlarmTimes.Alarm_flag == 2)
			{
				if(AlarmTimes.Alarm_num == 1)
				{
					AlarmTimes.Alarm_Array[0].min++;
					if(AlarmTimes.Alarm_Array[0].min >= 60)
					{
						AlarmTimes.Alarm_Array[0].min = 0;
					}
				}
				else if(AlarmTimes.Alarm_num == 2)
				{
					AlarmTimes.Alarm_Array[1].min++;
					if(AlarmTimes.Alarm_Array[1].min >= 60)
					{
						AlarmTimes.Alarm_Array[1].min = 0;
					}
				}
				else if(AlarmTimes.Alarm_num == 3)
				{
					AlarmTimes.Alarm_Array[2].min++;
					if(AlarmTimes.Alarm_Array[2].min >= 60)
					{
						AlarmTimes.Alarm_Array[2].min = 0;
					}
				}
			}
		}
		if(DelAlarmFlag == 1)
		{
			if(AlarmTimes.Alarm_num == 1)
			{
				AlarmTimes.Alarmnum.Alarm1 =0;
				AlarmTimes.Alarm_Array[0].hour = 0;
				AlarmTimes.Alarm_Array[0].min = 0;
			}
			else if(AlarmTimes.Alarm_num == 2)
			{
				AlarmTimes.Alarmnum.Alarm2 =0;
				AlarmTimes.Alarm_Array[2].hour = 0;
				AlarmTimes.Alarm_Array[1].min = 0;
			}
			else if(AlarmTimes.Alarm_num == 3)
			{
				AlarmTimes.Alarmnum.Alarm3 =0;
				AlarmTimes.Alarm_Array[2].hour = 0;
				AlarmTimes.Alarm_Array[2].min = 0;
			}
		}
	}
}
/***********************************************************************************
��������		Key_RockonTime
����˵���� 		������ʱ��λ����
��������� 		��
����ֵ��		��
***********************************************************************************/
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
			Timer_num = Timer_Ico;
			Timer_Array.min = 0;
			Timer_Array.sec = 0;
//			for(i = 0;i <= 1;i++)
//			{
//				Timer_Array[i] = 0;
//			}
		}
		else if(Interface == 2)
		{
			Interface = 0;
			if(RTC_num == RTC_Status_Alarm)
			{
				AlarmTimes.Alarmnum.Alarm1 = AlarmTimes.LastAlarmnum.Alarm1;
				AlarmTimes.Alarmnum.Alarm2 = AlarmTimes.LastAlarmnum.Alarm2;
				AlarmTimes.Alarmnum.Alarm3 = AlarmTimes.LastAlarmnum.Alarm3;
				for(i = 0;i <= 2;i++)
				{
					AlarmTimes.Alarm_Array[i] = AlarmTimes.LastAlarm_Array[i];
				}
			}
			RTC_num = RTC_Status_Hour;

		}
	}
}
#endif
#endif
