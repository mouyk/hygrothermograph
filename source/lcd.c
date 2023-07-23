
#ifndef _LCD_C_
#define _LCD_C_
/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/lcd_led.h"
#include "include/delay.h"
#include "include/gxhtc.h"
#include "include/disp.h"
#include "include/key.h"
#include "include/time.h"
#include "include/rtc.h"
#include <intrins.h>
/*********************************************************************************************************************			
	本例程实现4com*9seg、1/3bias LCD显示功能，LCD时钟设置为XOSCL或IRCL，实现LCD最小电流模式。
	
	重要提示：
	在关闭LCD功能前，把所设置的COM脚和SEG脚设置为输出模式并输出低电平，可以避免关闭LCD功能时LCD屏出现拖影现象。
*********************************************************************************************************************/			
void LCD_Off(void)
{
	//在关闭LCD功能前，把所设置的COM脚和SEG脚设置为输出模式并输出低电平，可以避免关闭LCD功能时LCD屏出现拖影现象。
	P00 = 0;
	P01 = 0;
	P02 = 0;
	P03 = 0;	 
       
	P57 = 0;
	P34 = 0; 
	P35 = 0;  
	P56 = 0; 
	P50 = 0;
	P51 = 0;
	P52 = 0; 
	P53 = 0;
	P54 = 0;
	
	P00F = OUTPUT;
	P01F = OUTPUT;
	P02F = OUTPUT;
	P03F = OUTPUT;	 
       
	P57F = OUTPUT;
	P34F = OUTPUT; 
	P35F = OUTPUT;  
	P56F = OUTPUT; 
	P50F = OUTPUT;
	P51F = OUTPUT;
	P52F = OUTPUT; 
	P53F = OUTPUT;
	P54F = OUTPUT;
	
	LXCON = 0;						//关闭LCD功能
}

void Lcd_init(void)
{
	unsigned char i;
	EA = 1;													//开全局中断

#ifdef PRINT_EN
	uart_printf("LCD Power Saving Mode Demo Code\n");
#endif

		//初始化LCD驱动引脚
	P00F = P00_COM0_SETTING;
	P01F = P01_COM1_SETTING;
	P02F = P02_COM2_SETTING;
	P03F = P03_COM3_SETTING;	 
       
	P57F = P57_SEG0_SETTING;
	P34F = P34_SEG1_SETTING; 
	P35F = P35_SEG2_SETTING;  
	P56F = P56_SEG3_SETTING; 
	P50F = P50_SEG4_SETTING;
	P51F = P51_SEG5_SETTING;
	P52F = P52_SEG6_SETTING; 
	P53F = P53_SEG7_SETTING;
	
	P54F = P54_SEG8_SETTING;
	P55F = P55_SEG9_SETTING;
	P60F = P60_SEG10_SETTING;	
	P61F = P61_SEG11_SETTING;
	P62F = P62_SEG12_SETTING;		
	P63F = P63_SEG13_SETTING;	
	P64F = P64_SEG14_SETTING;	
	P65F = P65_SEG15_SETTING;	

	P10F = P10_SEG16_SETTING; 
	P11F = P11_SEG17_SETTING; 
	P12F = P12_SEG18_SETTING; 
	P13F = P13_SEG19_SETTING; 
	P14F = P14_SEG20_SETTING; 
	P15F = P15_SEG21_SETTING; 
	P16F = P16_SEG22_SETTING; 
	P17F = P17_SEG23_SETTING; 
	
	P27F = P27_SEG24_SETTING; 
	P26F = P26_SEG25_SETTING; 
	P25F = P25_SEG26_SETTING; 
	P24F = P24_SEG27_SETTING; 
	P23F = P23_SEG28_SETTING; 
	P22F = P22_SEG29_SETTING; 
	P21F = P21_SEG30_SETTING; 
	P20F = P20_SEG31_SETTING; 
	
	P07F = P07_LCD_SEG32_SETTING; 
	P06F = P06_LCD_SEG33_SETTING;


	//LCD RAM清零
	for(i = 0; i < 34; i++)
	{
		INDEX = i;
		LXDAT = 0;
	}
	
//	P72F = P72_XOSCL_IN_SETTING;			//设置P72为晶振引脚
//	P71F = P71_XOSCL_OUT_SETTING;			//设置P71为晶振引脚
//	CKCON |= XLCKE;							//使能XSOCL
//	while(!(CKCON & XLSTA));				//等待XSOCL稳定
	
// 	CKCON |= ILCKE;							//使能IRCL

	LXDIVH = 0;				//设置LCD时钟分频，目标帧频率为64HZ
	LXDIVL = 0;	
	LXCAD = 0;
	LXCFG =	 DMOD(DMOD_80ua) | BIAS(BIAS_1_3) | LDRV(LDRV_7);			//设置LCD驱动电流、偏压(bias)、辉度
	LXCON =  LEN(LEN_XOSCL) | LMOD(LMOD_lcd);	 						//设置LCD时钟源为XOSCL，选择LCD模式
// 	LXCON =  LEN(LEN_IRCL) | LMOD(LMOD_lcd);	 						//设置LCD时钟源为IRCL，选择LCD模式
	
	//设置LCD全显
	for(i = 0; i < 34; i++)
	{
		INDEX = i;
		LXDAT = 0x0F;
	}

//	LCD_Off();
	
	I2CCON = 0;						//关闭I2C，否则系统时钟无法关闭
	CKCON = XLCKE;					//关闭除LCD时钟外的其他时钟
// 	CKCON = ILCKE;	
	PWCON &= ~0x08;					//LDO进入低功率模式
	
	PCON = (PCON&0x84) | 0x02;      //进入STOP模式
	_nop_();

}
/***********************************************************************************
函数名：		Key_timedate
功能说明： 		温湿度数据LCD相应位置显示
输入参数： 		无
返回值：		无
***********************************************************************************/
void Lcd_Humiture(void)
{
	extern uint8_t lcd_ram[34];
	uint16_t lcdtemp=0,lcdhumi=0;
	if((GXHTC3_temp>=-10)&&(GXHTC3_temp<=50))    //温度笑脸
		lcd_ram[29] |=smile_Temp;
	if((GXHTC3_temp>=-10)&&(GXHTC3_temp<=50))
	{
		if(FahrenFlag == 0)
		{
			GXHTC3_temp = GXHTC3_temp;
		}
		else
		{
			GXHTC3_temp = (9.0/5.0)*(GXHTC3_temp)+32;
		}
		lcdtemp=GXHTC3_temp*10;
		lcdhumi=GXHTC3_humi;
		UpdateNixieTubeRAMB(lcdtemp/100,27);
		UpdateNixieTubeRAMB(lcdtemp%100/10,25);	
		UpdateNixieTubeRAMB(lcdtemp%100%10,23);	
		if(GXHTC3_temp<0)
			lcd_ram[29] |=negative;	                    //负号
	}
	else if(GXHTC3_temp > 50)
	{
		UpdateNixieTubeRAMB(0xB,27);
		UpdateNixieTubeRAMB(0xB,25);
		UpdateNixieTubeRAMB(0xB,23);
	}
	else if(GXHTC3_temp < -10)
	{
		UpdateNixieTubeRAMB(0xA,27);
		UpdateNixieTubeRAMB(0xA,25);
		UpdateNixieTubeRAMB(0xA,23);
	}
	UpdateNixieTubeRAMA(lcdhumi/10,19);
	UpdateNixieTubeRAMA(lcdhumi%10,21);	
	lcd_ram[25] |= 0x01;     //.
	lcd_ram[23] |= 0x01;      //温湿度标
	lcd_ram[22] |= 0x01;     //%
	if((GXHTC3_humi>=40)&&(GXHTC3_humi<=70))    //湿度笑脸
		lcd_ram[20] |=smile_Humi;
	
}
/***********************************************************************************
函数名：		Lcd_Colon
功能说明： 		时间中间的冒号控制
输入参数： 		flag：0：表示常亮，1：表示半秒闪烁
返回值：		无
***********************************************************************************/
void Lcd_Colon(uint8_t flag)
{
	static uint8_t ColNum = 0;
	if(flag == 1)
	{
		ColNum++;
		if((ColNum <= 2))
		{
			lcd_ram[4] |= 0x01;
		}
		else if(ColNum >= 4)
		{
			ColNum = 0;
		}

	}
	else
	{
		lcd_ram[4] |= 0x01;
	}
}
/***********************************************************************************
函数名：		Lcd_IconFunction
功能说明： 		LCD图标及时间等的闪烁
输入参数： 		menu：0：表示时间界面，1：表示计时界面，2：时钟调整界面；flag ：0~5表示不同闪烁位置的数字加减及范围控制
返回值：		无
***********************************************************************************/
void Lcd_IconFunction(uint8_t menu,uint8_t flag)
{
	static first = 0;
	lcd_ram[32] = SOC3;
	if(FahrenFlag == 0)
	{
		lcd_ram[30] = C_Tmp;
	}
	else
	{
		lcd_ram[30] =F_Tmp;
	}
	if(menu == 1)
	{
		Lcd_Countdown(menu,flag,Timer_Array[0],Timer_Array[1]);
		Lcd_Colon(0);
	}
	else if(menu == 2)
	{
		if(first == 0)
		{
			first =1;
			RTC_Array[0] = calendar.w_year;
			RTC_Array[1] = calendar.w_month;
			RTC_Array[2] = calendar.w_date;
			RTC_Array[3] = calendar.hour;
			RTC_Array[4] = calendar.min;
			RTC_Array[5] = calendar.sec;
		}
		Lcd_TimeHanlde(flag,RTC_Array[3],RTC_Array[4]);
		Lcd_DateFunction(flag,RTC_Array[0],RTC_Array[1],RTC_Array[2]);
		if(Alarm_num == 0)
		{
			Lcd_AlarmHanlde(flag,Alarm_num,Alarm_flag,Alarm_Array[0],Alarm_Array[1]);
		}
		else if(Alarm_num == 1)
		{
			Lcd_AlarmHanlde(flag,Alarm_num,Alarm_flag,Alarm_Array[2],Alarm_Array[3]);
		}
		else
		{
			Lcd_AlarmHanlde(flag,Alarm_num,Alarm_flag,Alarm_Array[4],Alarm_Array[5]);
		}
		Lcd_Colon(0);
		Lcd_ZigbeeIcon(ZigbeeFlag);
	}
	else
	{
		first = 0;
		Lcd_TimeHanlde(2,calendar.hour,calendar.min);
		Lcd_DateFunction(flag,calendar.w_year,calendar.w_month,calendar.w_date);
		Lcd_Colon(1);
		Lcd_ZigbeeIcon(ZigbeeFlag);
	}
}
/***********************************************************************************
函数名：		Lcd_WeekDisplay
功能说明： 		LCD星期位置显示处理
输入参数： 		num：0~6：表示星期一~星期日
返回值：		无
***********************************************************************************/
void Lcd_WeekDisplay(uint8_t num)
{
	if(num == 1)
		lcd_ram[18] = Monday;													//星期一
	else if(num == 2)
		lcd_ram[18] = Tuesday;													//星期二
	else if(num == 3)
		lcd_ram[18] = wednesday;													//星期三
	else if(num == 0)
		lcd_ram[18] = Sunday;													//星期天
	else
		lcd_ram[18] = 0;
	if(num == 4)
		lcd_ram[17] |= thursday;													//星期四
	else
	{
		lcd_ram[17] = lcd_ram[17]>>1;
		lcd_ram[17] = lcd_ram[17]<<1;
	}
	if(num == 5)
		lcd_ram[8] |= friday;													//星期五
	else
	{
		lcd_ram[8] = lcd_ram[8]>>1;
		lcd_ram[8] = lcd_ram[8]<<1;
	}
	if(num == 6)
		lcd_ram[27] |= Saturday;													//星期六
	else
	{
		lcd_ram[27] = lcd_ram[27]>>1;
		lcd_ram[27] = lcd_ram[27]<<1;
	}
}
/***********************************************************************************
函数名：		Lcd_DateFunction
功能说明： 		LCD中日期相应位置闪烁控制
输入参数： 		flag：2~4：表示年月日闪烁位置，year：年份，month：月份，day：日
返回值：		无
***********************************************************************************/
void Lcd_DateFunction(uint8_t flag, uint16_t year, uint8_t month,uint8_t day)
{
	uint8_t week = 0;
	static date = 0;
	if(flag == 2)
	{
		if(date == 0)
		{
			date = 1;
			UpdateNixieTubeRAMA((year-2000)/10,9);
			UpdateNixieTubeRAMA((year-2000)%10,11);
		}
		else
		{
			date = 0;
			lcd_ram[9] = 0;
			lcd_ram[10] = 0;
			lcd_ram[11] = 0;
			lcd_ram[12] = 0;
		}
		week = RTC_Set_Week(year,month,day);
		Lcd_WeekDisplay(week);
	}
	else
	{
		UpdateNixieTubeRAMA((year-2000)/10,9);
		UpdateNixieTubeRAMA((year-2000)%10,11);
		week = RTC_Set_Week(year,month,day);
		Lcd_WeekDisplay(week);
	}
	if(flag == 3)
	{
		if(date == 0)
		{
			date = 1;
			UpdateNixieTubeRAMA(month%10,13);	
			if(month>=10)	
				lcd_ram[14] |= month1;
		}
		else
		{
			date = 0;
			lcd_ram[13] = 0;
			lcd_ram[14] = 0;
		}
		week = RTC_Set_Week(year,month,day);
		Lcd_WeekDisplay(week);
	}
	else
	{
		UpdateNixieTubeRAMA(month%10,13);	
			if(month>=10)	
				lcd_ram[14] |= month1;
		week = RTC_Set_Week(year,month,day);
		Lcd_WeekDisplay(week);
	}
	if(flag == 4)
	{
		if(date == 0)
		{
			date = 1;
			if((day>=10)&&(day<20))
				lcd_ram[15] = 0x0C;
			else if((day>=20)&&(day<30))
				lcd_ram[15] = 0x07;
			else if(day>=30)
				lcd_ram[15] = 0X0E;
			UpdateNixieTubeRAMA(day%10,16);
		}
		else
		{
			date = 0;
			lcd_ram[15] = 0;
			lcd_ram[16] = 0;
			lcd_ram[17] = 0;
		}
		week = RTC_Set_Week(year,month,day);
		Lcd_WeekDisplay(week);
	}
	else
	{
		if((day>=10)&&(day<20))
			lcd_ram[15] = 0x0C;
		else if((day>=20)&&(day<30))
			lcd_ram[15] = 0x07;
		else if(day>=30)
			lcd_ram[15] = 0X0E;
		UpdateNixieTubeRAMA(day%10,16);
		week = RTC_Set_Week(year,month,day);
		Lcd_WeekDisplay(week);
	}		
	lcd_ram[10] |=0x01;                              //显示20xx年
	
}
/***********************************************************************************
函数名：		Lcd_HourTurn
功能说明： 		处理上午、下午显示控制
输入参数： 		hour：表示小时
返回值：		无
***********************************************************************************/
void Lcd_HourTurn(uint8_t hour)
{
	if(HourFlag == 0)
	{
		lcd_ram[0] = 0;
	}
	else
	{
		if((hour>=0)&&(hour<12))
		{
			lcd_ram[0] = 0x04;
		}
		else
		{
			lcd_ram[0] = 0x08;
		}
	}
}
/***********************************************************************************
函数名：		Lcd_TimeHanlde
功能说明： 	LCD中时间相应位置闪烁控制
输入参数： 	flag：0~1表示时分闪烁位置	hour：表示小时，min：表示分
返回值：		无
***********************************************************************************/
void Lcd_TimeHanlde(uint8_t flag, uint8_t hour, uint8_t min)
{
	static uint8_t TimeNum = 0;
	uint16_t hour1 = 0;
	if(HourFlag != 0)
	{
		if(hour>=12)
		{
			hour1 = hour -12;
			if(flag == 0)
			{
				if(TimeNum == 0)
				{
					TimeNum = 1;
					Lcd_HourHanlde(hour1,1);
				}
				else
				{
					TimeNum = 0;
					Lcd_HourHanlde(hour1,0);
				}
			}
			else
			{
				Lcd_HourHanlde(hour1,1);
			}
			if(flag == 1)
			{
				if(TimeNum == 0)
				{
					TimeNum = 1;
					Lcd_MinHanlde(min,1);
				}
				else
				{
					TimeNum = 0;
					Lcd_MinHanlde(min,0);
				}
			}
			else
			{
				Lcd_MinHanlde(min,1);
			}
		}
		else
		{
			if(flag == 0)
			{
				if(TimeNum == 0)
				{
					TimeNum = 1;
					Lcd_HourHanlde(hour,1);
				}
				else
				{
					TimeNum = 0;
					Lcd_HourHanlde(hour,0);
				}
			}
			else
			{
				Lcd_HourHanlde(hour,1);
			}
			if(flag == 1)
			{
				if(TimeNum == 0)
				{
					TimeNum = 1;
					Lcd_MinHanlde(min,1);
				}
				else
				{
					TimeNum = 0;
					Lcd_MinHanlde(min,0);
				}
			}
			else
			{
				Lcd_MinHanlde(min,1);
			}
		}
	}
	else
	{
		if(flag == 0)
		{
			if(TimeNum == 0)
			{
				TimeNum = 1;
				Lcd_HourHanlde(hour,1);
			}
			else
			{
				TimeNum = 0;
				Lcd_HourHanlde(hour,0);
			}
		}
		else
		{
			Lcd_HourHanlde(hour,1);
		}
		if(flag == 1)
		{
			if(TimeNum == 0)
			{
				TimeNum = 1;
				Lcd_MinHanlde(min,1);
			}
			else
			{
				TimeNum = 0;
				Lcd_MinHanlde(min,0);
			}
		}
		else
		{
			Lcd_MinHanlde(min,1);
		}
	}
	Lcd_HourTurn(hour);													//上下午表示处理
}
/***********************************************************************************
函数名：		Lcd_AlarmHanlde
功能说明： 	LCD中时间相应位置闪烁控制
输入参数： 	flag：0~1表示时分闪烁位置	hour：表示小时，min：表示分
返回值：		无
***********************************************************************************/
void Lcd_AlarmHanlde(uint8_t flag, uint8_t num, uint8_t flag1, uint8_t hour, uint8_t min)
{
	static uint8_t AlarmNum = 0;
	uint16_t hour1 = 0;
	if(flag == 5)
	{
		if(flag1 == 0)
		{
			if(AlarmNum == 0)
			{
				AlarmNum = 1;
				if(num == 0)
					lcd_ram[31] = alarm_timer1;
				else if(num == 1)
					lcd_ram[31] = alarm_timer2;
				else
					
					lcd_ram[31] = alarm_timer3;
				lcd_ram[33] = alarm_timer;
			}
			else
			{
				AlarmNum = 0;
				lcd_ram[31] = 0;
				lcd_ram[33] = 0;
			}
		}
		else
		{
			if(num == 0)
				lcd_ram[31] = alarm_timer1;
			else if(num == 1)
				lcd_ram[31] = alarm_timer2;
			else
				lcd_ram[31] = alarm_timer2;
			lcd_ram[33] = alarm_timer;
		}
		if(flag1 == 1)
		{
			if(HourFlag != 0)
			{
				if(hour>=12)
				{
					hour1 = hour -12;
					if(AlarmNum == 0)
					{
						AlarmNum = 1;
						Lcd_HourHanlde(hour1,1);
					}
					else
					{
						AlarmNum = 0;
						Lcd_HourHanlde(hour1,0);
					}
				}
				else
				{
					if(AlarmNum == 0)
					{
						AlarmNum = 1;
						Lcd_HourHanlde(hour,1);
					}
					else
					{
						AlarmNum = 0;
						Lcd_HourHanlde(hour,0);
					}
				}
			}
			else
			{
				if(AlarmNum == 0)
				{
					AlarmNum = 1;
					Lcd_HourHanlde(hour,1);
				}
				else
				{
					AlarmNum = 0;
					Lcd_HourHanlde(hour,0);
				}
			}
		}
		else
		{
			if(HourFlag != 0)
			{
				if(hour>=12)
				{
					hour1 = hour -12;
					Lcd_HourHanlde(hour1,1);
				}
				else
				{
					Lcd_HourHanlde(hour,1);
				}
			}
			else
			{
					Lcd_HourHanlde(hour,1);
			}
		}
		if(flag1 == 2)
		{
			if(AlarmNum == 0)
			{
				AlarmNum = 1;
				Lcd_MinHanlde(min,1);
			}
			else
			{
				AlarmNum = 0;
				Lcd_MinHanlde(min,0);
			}
		}
		else
		{
			Lcd_MinHanlde(min,1);
		}
		Lcd_HourTurn(hour);															//上下午表示处理
	}
}
void Lcd_HourHanlde(uint8_t hour, uint8_t flag)
{
	if(flag == 1)
	{
		UpdateNixieTubeRAMA(hour/10,1);
		UpdateNixieTubeRAMA(hour%10,3);
	}
	else
	{
		lcd_ram[1] = 0;
		lcd_ram[2] = 0;
		lcd_ram[3] = 0;
		lcd_ram[4] = 0;
	}
}
void Lcd_MinHanlde(uint8_t min, uint8_t flag)
{
	if(flag == 1)
	{
		UpdateNixieTubeRAMA(min/10,5);
		UpdateNixieTubeRAMA(min%10,7);
	}
	else
	{
		lcd_ram[5] = 0;
		lcd_ram[6] = 0;
		lcd_ram[7] = 0;
		lcd_ram[8] = 0;
	}
}
void Lcd_ZigbeeIcon(uint8_t zigbee)
{
	static uint8_t a = 0,b = 0;

	if(zigbee == 1)
	{
		b++;
		if(a == 0)
		{
			a = 1;
			lcd_ram[2] |= LoGo_Zigbee;												//Zigbee显示
		}
		else
		{
			a = 0;
		}
	}
	if(b >= 8)
	{
		b = 0;
		ZigbeeFlag = 0;
	}
}
void Lcd_Countdown(uint8_t menu, uint8_t flag, uint8_t hour, uint8_t min)
{
	static CouNum = 0;
	if(menu == 1)
	{
		if((flag == 0)&&(Time_start == 0))
		{
			if(CouNum == 0)
			{
				CouNum = 1;
				lcd_ram[30] |= LoGo_timer;
			}
			else
			{
				CouNum = 0;
			}
		}
		else
		{
			lcd_ram[30] |= LoGo_timer;
		}
		if(flag == 1)
		{
			if(CouNum == 0)
			{
				CouNum = 1;
				Lcd_HourHanlde(hour,1);
			}
			else
			{
				CouNum = 0;
				Lcd_HourHanlde(hour,0);
			}
		}
		else
		{
			Lcd_HourHanlde(hour,1);
		}
		if(flag == 2)
		{
			if(CouNum == 0)
			{
				CouNum = 1;
				Lcd_MinHanlde(min,1);
			}
			else
			{
				CouNum = 0;
				Lcd_MinHanlde(min,0);
			}
		}
		else
		{
			Lcd_MinHanlde(min,1);
		}
	}
	lcd_ram[0] = 0;															//强制将小时制显示清除
}
void Lcd_Backlight(void)
{
	static time_num =0;
	if(P32 == 1)
	{
		time_num++;
		if(time_num > 8)
		{
			P32 = 0;
			time_num = 0;
		}
	}
}
#endif
