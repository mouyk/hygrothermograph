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
uint8_t RTC_num = 0;														//0：时   1：分    2：年   3：月    4：日    5：闹钟

/***********************************************************************************
函数名：		RTC_WriteSecond
功能说明： 		RTC写入秒值
输入参数： 		second 秒值
返回值：		无
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
函数名：		RTC_WriteMinute
功能说明： 		RTC写入分值
输入参数： 		minute 分值
返回值：		无
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
函数名：		RTC_WriteHour
功能说明： 		RTC写入小时
输入参数： 		hour 小时值
返回值：		无
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
函数名：		RTC_WriteDay
功能说明： 		RTC写入天数
输入参数： 		day 天数
返回值：		无
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
函数名：		RTC_WriteWeek
功能说明： 		RTC写入星期
输入参数： 		week 星期值
返回值：		无
***********************************************************************************/
void RTC_WriteWeek(unsigned char week) //hour = 1~7代表星期一~星期天, 如果week = 0， 表示关闭星期计数功能
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
	RTC控制例程
*********************************************************************************************************************/
bit HalfSecFlag;
bit AlarmEvFlag;
bit millisecondFlag;
uint8_t times10 = 0,times10Flag = 0;
void RTC_ISR (void) interrupt 13 	 
{
	if(RTCIF & RTC_MF)			//毫秒中断
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
	if(RTCIF & RTC_HF)			//半秒中断
	{
		RTCIF = RTC_HF;
		HalfSecFlag = 1;
	}	
	if(RTCIF & RTC_AF)			//闹钟中断
	{
		RTCIF = RTC_AF;	
		AlarmEvFlag = 1;
	}
}	
/***********************************************************************************
函数名：		RTC_init
功能说明： 		初始化RTC 时钟，并设置时间
输入参数： 		无
返回值：		无
***********************************************************************************/
void RTC_init(void)
{

	P72F = P72_XOSCL_IN_SETTING;			//设置P32为晶振引脚
	P71F = P71_XOSCL_OUT_SETTING;			//设置P33为晶振引脚
	CKCON |= XLCKE;							//使能XSOCL
	while(!(CKCON & XLSTA));				//等待XSOCL稳定
	RTCON = RTCE(1) | MSE(1) | HSE(1) | SCE(1) | MCE(1) | HCE(1);	//使能RTC功能，设置半秒、毫秒中断开启，闹铃功能开启(时、分、秒匹配使能)
	
	Delay_50us(6);		//RTC使能后必须延时300us再写入时间， 否则写入时间可能无效。
	
	//设置当前时间为12:00:00
	RTC_WriteHour(12);			
	RTC_WriteMinute(0);		
	RTC_WriteSecond(0);			

	//设置闹钟时间为12:01:00
	RTAH	=	0;			
	RTAM	=	0;
	RTAS	=	0;
	
	RTMSS = 0;			//设置毫秒中断时间间隔
	INT8EN = 1;			//RTC中断使能
	
	HalfSecFlag = 0;
	AlarmEvFlag = 0;
	

	if(HalfSecFlag)	//半秒打印当前时间
	{
		HalfSecFlag = 0;
#ifdef PRINT_EN
//		uart_printf("Hour = %d,Minute = %d,Second = %d\n",(unsigned int)(RTCH&0x1F),(unsigned int)RTCM,(unsigned int)RTCS);	
#endif		
	}
	if(AlarmEvFlag)	//闹钟中断产生时打印
	{
		AlarmEvFlag = 0;
#ifdef PRINT_EN
//		uart_printf("Alarm event happen!\n");	
#endif		
	}

}
/***********************************************************************************
函数名：		RTC_Alarm_init
功能说明： 		闹钟开启、关闭及闹钟参数输入
输入参数： 		flag：1：表示开启闹钟，:0：关闭闹钟；hour：闹钟中的时，min：闹钟中的分，sec：闹钟的秒
返回值：		无
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

//判断是否是闰年函数
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
uint8_t IS_Leap_Year( uint16_t year )
{
    //能被4或者400整除，并且不能被100整除
    if( ( ( year % 4 == 0 ) || ( year % 400 == 0 ) ) && ( year % 100 != 0 ) )
        return 1;
    else
        return 0;

}
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; //月修正数据表
//平年的月份日期表
uint8_t const  mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t RTC_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec )
{

    uint16_t t;
    uint16_t sdaycount = 0;
	
   //设置当前星期几	
	RTC_Set_Week(syear,smon,sday);							//根据年月日算出星期几并写入到 星期计数器	

	
    if( syear < 1970 || syear > 2099 )
        return 1;
    for( t = 1970; t < syear; t++ )							//所有年份的秒钟相加
    {
        if( IS_Leap_Year( t ) )
            sdaycount += 366;								//闰年天数
        else
            sdaycount += 365;								//平年天数
    }
    smon -= 1;
    for( t = 0; t < smon; t++ )								//把前面月份的天数相加
    {
        sdaycount += ( uint16_t )mon_table[t];
        if( IS_Leap_Year( syear ) && t == 1 )				//闰年2月份增加一天的秒钟数
        sdaycount += 1;
    }
    sdaycount += ( uint16_t )(sday - 1) * 1;				//把前面天数相加
//    sdaycount += ( uint16_t )(sday) * 1;				//把前面天数相加	
	#ifdef PRINT_EN	
//	uart_printf("all day  %d\n",sdaycount);	
	#endif
	RTC_WriteDay(sdaycount);								//设置当前时间→1970年的天数写到天数寄存器里面
   //设置当前时间 天 时分秒
	RTC_WriteHour(hour);									//写入小时计数器			
	RTC_WriteMinute(min);									//写入分钟计数器	
	RTC_WriteSecond(sec);									//写入秒钟计数器

/* 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
	PWR_BackupAccessCmd( ENABLE );

	RTC_SetCounter( sdaycount );							//设置RTC计数器的值
	RTC_WaitForLastTask();
 */
 
 
 return 0;
}
//初始化闹钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒
//返回值:0,成功;其他:错误代码.
uint8_t RTC_Alarm_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec )
{
/*
    uint16_t t;
    uint16_t sdaycount = 0;
    if( syear < 1970 || syear > 2099 )
        return 1;
    for( t = 1970; t < syear; t++ )						                //所有年份的秒钟相加
    {
        if( IS_Leap_Year( t ) )
            sdaycount += 366;								//闰年天数
        else
            sdaycount += 365;								//平年天数
    }
    smon -= 1;
    for( t = 0; t < smon; t++ )							                //把前面月份的秒钟数相加
    {
        sdaycount += ( uint16_t )mon_table[t] * 1;
        if( IS_Leap_Year( syear ) && t == 1 )				            //闰年2月份增加一天的秒钟数
            sdaycount += 1;
    }
    sdaycount += ( uint16_t )( sday - 1 ) * 1;				        	//把前面天数的秒钟数相加
    sdaycount += ( uint16_t )hour * 3600;						        //加小时秒钟数
    sdaycount += ( uint16_t )min * 60;							        //加分钟秒钟数
    sdaycount += sec;									                //加上最后的秒数

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
    PWR_BackupAccessCmd( ENABLE );

    RTC_SetAlarm( sdaycount );
    RTC_WaitForLastTask();
*/

	//设置闹钟时间为12:01:00
	RTAH	=	hour;			
	RTAM	=	min;
	RTAS	=	min;

    return 0;
}
//得到当前的时间
//返回值:0,成功;其他:错误代码.
uint8_t RTC_Get( void )
{
    static uint16_t daycnt = 0;
    uint16_t timecount = 0;
    uint16_t temp = 0;
    uint16_t temp1 = 0;

    timecount 	= (RTCDH<<8)+RTCDL;							//获取1970到现在总共天数							
    temp 		= timecount / 1;							//得到天数

    if( daycnt != temp )
    {
        daycnt = temp;
        temp1 = 1970;
        while( temp > 365 )								    //超过1年
        {
            if( IS_Leap_Year( temp1 ) )						//闰年
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
            else											//平年
            {
                temp -= 365;
            }
            temp1++;
        }
        calendar.w_year = temp1;						//得到年份
        temp1 = 0;
        while( temp >= 28 )								//超过1月
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
        calendar.w_month = temp1 + 1;					//月
        calendar.w_date = temp + 1;						//日
    }
	
    calendar.hour = RTCH&0x1F;
    calendar.min = 	RTCM;
    calendar.sec = 	RTCS;
    calendar.week = RTC_Set_Week( calendar.w_year, calendar.w_month, calendar.w_date );//RTC_Get_Week( calendar.w_year, calendar.w_month, calendar.w_date );   RTCH>>5
    return 0;
}
//获得现在是星期几
//功能描述:输入公历日期设置星期几(只允许1901-2099年)
//输入参数：公历年月日
//返回值：	星期号
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
函数名：		RTC_Daysmonth
功能说明： 		通过年和月计算当前的最大天数
输入参数： 		year：年份，month：月份
返回值：		days：天数
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
