#ifndef _RTC_C_
#define _RTC_C_
/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/delay.h"
#include "include/rtc.h"
#include <intrins.h>
/*********************************************************************************************************************/


_calendar_obj calendar;

int16_t RTC_Array[6] = {0};
uint8_t RTC_num = 0;														//0��ʱ   1����    2����   3����    4����    5������

/***********************************************************************************
��������		RTC_WriteSecond
����˵���� 		RTCд����ֵ
��������� 		second ��ֵ
����ֵ��		��
***********************************************************************************/	
void RTC_WriteSecond(unsigned char second) //second = 0~59
{
	if(second > 59) return;
	RTCON |= RTCWE(1);
	RTCS = second;
	Delay_50us(1);
	RTCON &= ~RTCWE(1);	 	
}
/***********************************************************************************/

/***********************************************************************************
��������		RTC_WriteMinute
����˵���� 		RTCд���ֵ
��������� 		minute ��ֵ
����ֵ��		��
***********************************************************************************/	
void RTC_WriteMinute(unsigned char minute) //minute = 0~59
{
	if(minute > 59) return;
	RTCON |= RTCWE(1);
	RTCM = minute;
	Delay_50us(1);
	RTCON &= ~RTCWE(1);
}
/***********************************************************************************/

/***********************************************************************************
��������		RTC_WriteHour
����˵���� 		RTCд��Сʱ
��������� 		hour Сʱֵ
����ֵ��		��
***********************************************************************************/
void RTC_WriteHour(unsigned char hour)	//hour = 0~23
{
	if(hour > 23) return;
	RTCON |= RTCWE(1);
	RTCH = (RTCH&0xE0)|hour;
	Delay_50us(1);
	RTCON &= ~RTCWE(1);	 	
}
/***********************************************************************************/
/***********************************************************************************
��������		RTC_WriteDay
����˵���� 		RTCд������
��������� 		day ����
����ֵ��		��
***********************************************************************************/
void RTC_WriteDay(unsigned int day)	//day = 0~65536
{
//	if(day > 0xfffe) return;
	RTCON |= RTCWE(1);
	RTCDL = day&0xFF;
	RTCDH = (day>>8)&0xFF;	
	Delay_50us(1);
	RTCON &= ~RTCWE(1);	 	
}
/***********************************************************************************
��������		RTC_WriteWeek
����˵���� 		RTCд������
��������� 		week ����ֵ
����ֵ��		��
***********************************************************************************/
void RTC_WriteWeek(unsigned char week) //hour = 1~7��������һ~������, ���week = 0�� ��ʾ�ر����ڼ�������
{
	if(week > 7) return;
	RTCON |= RTCWE(1);
	week  =  week<<5;
	RTCH  = (RTCH&0x1F)|week;
	Delay_50us(10);
	RTCON &= ~RTCWE(1);	 	
}
/***********************************************************************************/

/*********************************************************************************************************************
	RTC��������
*********************************************************************************************************************/
bit HalfSecFlag;
bit AlarmEvFlag;
bit millisecondFlag;
uint8_t times10 = 0,times10Flag = 0;
void RTC_ISR (void) interrupt 13 	 
{
	if(RTCIF & RTC_MF)			//�����ж�
	{
		RTCIF = RTC_MF;	
		millisecondFlag = 1;
		times10++;
		if(times10 >= 10)
		{
			times10 = 0;
			times10Flag = 1;
		}
	}
	if(RTCIF & RTC_HF)			//�����ж�
	{
		RTCIF = RTC_HF;
		HalfSecFlag = 1;
	}	
	if(RTCIF & RTC_AF)			//�����ж�
	{
		RTCIF = RTC_AF;	
		AlarmEvFlag = 1;
	}
}	
/***********************************************************************************
��������		RTC_init
����˵���� 		��ʼ��RTC ʱ�ӣ�������ʱ��
��������� 		��
����ֵ��		��
***********************************************************************************/
void RTC_init(void)
{

	P72F = P72_XOSCL_IN_SETTING;			//����P32Ϊ��������
	P71F = P71_XOSCL_OUT_SETTING;			//����P33Ϊ��������
	CKCON |= XLCKE;							//ʹ��XSOCL
	while(!(CKCON & XLSTA));				//�ȴ�XSOCL�ȶ�
	RTCON = RTCE(1) | MSE(1) | HSE(1) | SCE(1) | MCE(1) | HCE(1);	//ʹ��RTC���ܣ����ð��롢�����жϿ��������幦�ܿ���(ʱ���֡���ƥ��ʹ��)
	
	Delay_50us(6);		//RTCʹ�ܺ������ʱ300us��д��ʱ�䣬 ����д��ʱ�������Ч��
	
	//���õ�ǰʱ��Ϊ12:00:00
	RTC_WriteHour(12);			
	RTC_WriteMinute(0);		
	RTC_WriteSecond(0);			

	//��������ʱ��Ϊ12:01:00
	RTAH	=	0;			
	RTAM	=	0;
	RTAS	=	0;
	
	RTMSS = 0;			//���ú����ж�ʱ����
	INT8EN = 1;			//RTC�ж�ʹ��
	
	HalfSecFlag = 0;
	AlarmEvFlag = 0;
	

	if(HalfSecFlag)	//�����ӡ��ǰʱ��
	{
		HalfSecFlag = 0;
#ifdef PRINT_EN
//		uart_printf("Hour = %d,Minute = %d,Second = %d\n",(unsigned int)(RTCH&0x1F),(unsigned int)RTCM,(unsigned int)RTCS);	
#endif		
	}
	if(AlarmEvFlag)	//�����жϲ���ʱ��ӡ
	{
		AlarmEvFlag = 0;
#ifdef PRINT_EN
//		uart_printf("Alarm event happen!\n");	
#endif		
	}

}
/***********************************************************************************
��������		RTC_Alarm_init
����˵���� 		���ӿ������رռ����Ӳ�������
��������� 		flag��1����ʾ�������ӣ�:0���ر����ӣ�hour�������е�ʱ��min�������еķ֣�sec�����ӵ���
����ֵ��		��
***********************************************************************************/
void RTC_Alarm_init(uint8_t flag,uint8_t hour,uint8_t min,uint8_t sec)
{
	if(flag == 1)
	{
		RTAH	=	hour;			
		RTAM	=	min;
		RTAS	=	sec;
    RTCON = RTCE(1) | MSE(1) | HSE(1) | SCE(1) | MCE(1) | HCE(1);
	}
	else
	{
		RTCON =  RTCE(1) | MSE(1) | HSE(1) | SCE(0) | MCE(0) | HCE(0);
	}
	
}

//�ж��Ƿ������꺯��
//����:���
//���:������ǲ�������.1,��.0,����
uint8_t IS_Leap_Year( uint16_t year )
{
    //�ܱ�4����400���������Ҳ��ܱ�100����
    if( ( ( year % 4 == 0 ) || ( year % 400 == 0 ) ) && ( year % 100 != 0 ) )
        return 1;
    else
        return 0;

}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; //���������ݱ�
//ƽ����·����ڱ�
uint8_t const  mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t RTC_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec )
{

    uint16_t t;
    uint16_t sdaycount = 0;
	
   //���õ�ǰ���ڼ�	
	RTC_Set_Week(syear,smon,sday);							//����������������ڼ���д�뵽 ���ڼ�����	

	
    if( syear < 1970 || syear > 2099 )
        return 1;
    for( t = 1970; t < syear; t++ )							//������ݵ��������
    {
        if( IS_Leap_Year( t ) )
            sdaycount += 366;								//��������
        else
            sdaycount += 365;								//ƽ������
    }
    smon -= 1;
    for( t = 0; t < smon; t++ )								//��ǰ���·ݵ��������
    {
        sdaycount += ( uint16_t )mon_table[t];
        if( IS_Leap_Year( syear ) && t == 1 )				//����2�·�����һ���������
        sdaycount += 1;
    }
    sdaycount += ( uint16_t )(sday - 1) * 1;				//��ǰ���������
//    sdaycount += ( uint16_t )(sday) * 1;				//��ǰ���������	
	#ifdef PRINT_EN	
//	uart_printf("all day  %d\n",sdaycount);	
	#endif
	RTC_WriteDay(sdaycount);								//���õ�ǰʱ���1970�������д�������Ĵ�������
   //���õ�ǰʱ�� �� ʱ����
	RTC_WriteHour(hour);									//д��Сʱ������			
	RTC_WriteMinute(min);									//д����Ӽ�����	
	RTC_WriteSecond(sec);									//д�����Ӽ�����

/* 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
	PWR_BackupAccessCmd( ENABLE );

	RTC_SetCounter( sdaycount );							//����RTC��������ֵ
	RTC_WaitForLastTask();
 */
 
 
 return 0;
}
//��ʼ������
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//syear,smon,sday,hour,min,sec�����ӵ�������ʱ����
//����ֵ:0,�ɹ�;����:�������.
uint8_t RTC_Alarm_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec )
{
/*
    uint16_t t;
    uint16_t sdaycount = 0;
    if( syear < 1970 || syear > 2099 )
        return 1;
    for( t = 1970; t < syear; t++ )						                //������ݵ��������
    {
        if( IS_Leap_Year( t ) )
            sdaycount += 366;								//��������
        else
            sdaycount += 365;								//ƽ������
    }
    smon -= 1;
    for( t = 0; t < smon; t++ )							                //��ǰ���·ݵ����������
    {
        sdaycount += ( uint16_t )mon_table[t] * 1;
        if( IS_Leap_Year( syear ) && t == 1 )				            //����2�·�����һ���������
            sdaycount += 1;
    }
    sdaycount += ( uint16_t )( sday - 1 ) * 1;				        	//��ǰ�����������������
    sdaycount += ( uint16_t )hour * 3600;						        //��Сʱ������
    sdaycount += ( uint16_t )min * 60;							        //�ӷ���������
    sdaycount += sec;									                //������������

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
    PWR_BackupAccessCmd( ENABLE );

    RTC_SetAlarm( sdaycount );
    RTC_WaitForLastTask();
*/

	//��������ʱ��Ϊ12:01:00
	RTAH	=	hour;			
	RTAM	=	min;
	RTAS	=	min;

    return 0;
}
//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
uint8_t RTC_Get( void )
{
    static uint16_t daycnt = 0;
    uint16_t timecount = 0;
    uint16_t temp = 0;
    uint16_t temp1 = 0;

    timecount 	= (RTCDH<<8)+RTCDL;							//��ȡ1970�������ܹ�����							
    temp 		= timecount / 1;							//�õ�����

    if( daycnt != temp )
    {
        daycnt = temp;
        temp1 = 1970;
        while( temp > 365 )								    //����1��
        {
            if( IS_Leap_Year( temp1 ) )						//����
            {
                if( temp > 366 )
                {
                    temp -= 366;
                }
                else
                {
                    temp1++;
                    break;
                }
            }
            else											//ƽ��
            {
                temp -= 365;
            }
            temp1++;
        }
        calendar.w_year = temp1;						//�õ����
        temp1 = 0;
        while( temp >= 28 )								//����1��
        {
            if( IS_Leap_Year( calendar.w_year ) && temp1 == 1 )
            {
                if( temp >= 29 )
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if( temp >= mon_table[temp1] )
                    temp -= mon_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1;					//��
        calendar.w_date = temp + 1;						//��
    }
	
    calendar.hour = RTCH&0x1F;
    calendar.min = 	RTCM;
    calendar.sec = 	RTCS;
    calendar.week = RTC_Set_Week( calendar.w_year, calendar.w_month, calendar.w_date );//RTC_Get_Week( calendar.w_year, calendar.w_month, calendar.w_date );   RTCH>>5
    return 0;
}
//������������ڼ�
//��������:���빫�������������ڼ�(ֻ����1901-2099��)
//�������������������
//����ֵ��	���ں�
uint8_t RTC_Set_Week( uint16_t year, uint8_t month, uint8_t day )
{
    uint16_t tem;
    uint8_t yearH, yearL;
    yearH = year / 100;
    yearL = year % 100;
    if( yearH > 19 )
        yearL += 100;
    tem = yearL + yearL / 4;
    tem = tem % 7;
    tem = tem + day + table_week[month - 1];
    if( yearL % 4 == 0 && month < 3 )
        tem--;
	tem = tem % 7;
//	RTC_WriteWeek(tem);
	#ifdef PRINT_EN	
//	uart_printf("week day  %d\n",tem);	
	#endif	
    return tem;
}
/***********************************************************************************
��������		RTC_Daysmonth
����˵���� 		ͨ������¼��㵱ǰ���������
��������� 		year����ݣ�month���·�
����ֵ��		days������
***********************************************************************************/
uint8_t RTC_Daysmonth(uint16_t year, uint8_t month)
{
	uint8_t days =0;
	if((year%4 ==0)&&(year%100 != 0)||year%400 == 0)
	{
		if(month == 2)
			days =29;
	}
	else
	{
		if(month == 2)
			days =28;
	}
	if(month == 1|month == 3|month == 5|month == 7|month == 8|month == 10|month == 12)
		days =31;
	else
	{
		if(month != 2)
			days = 30;
	}
	return days;
}



/*********************************************************************************************************************/
#endif
