
#ifndef _MAIN_C_
#define _MAIN_C_
/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/lcd_led.h"
#include "include/delay.h"
#include "include/lcd.h"
#include "include/gpio.h"
#include "include/gxhtc.h"
#include "include/rtc.h"
#include <intrins.h>
#include "include/pwm.h"


/*********************************************************************************************************************			
	本例程实现4com*9seg、1/3bias LCD显示功能，LCD时钟设置为XOSCL或IRCL，实现LCD最小电流模式。
	
	重要提示：
	在关闭LCD功能前，把所设置的COM脚和SEG脚设置为输出模式并输出低电平，可以避免关闭LCD功能时LCD屏出现拖影现象。
*********************************************************************************************************************/			
extern uint8_t key_value;

void main(void)
{
extern float GXHTC3_temp,GXHTC3_humi;
extern bit HalfSecFlag;
extern bit AlarmEvFlag;	
	unsigned char i;
	uint8_t ab = 0;
	uint16_t times = 0;
	uint16_t a,b,c,d,e,f,g=0;
#ifdef LVD_RST_ENABLE
	LVDCON = 0xE1;					//设置LVD复位电压为2V
#endif	
	
#if (SYSCLK_SRC == PLL)
	Sys_Clk_Set_PLL(PLL_Multiple);	//设置系统时钟为PLL，PLL_Multiple为倍频倍数
#endif
	
#ifdef UART0_EN
	Uart0_Initial(UART0_BAUTRATE);	//初始化UART0
#endif
	
#ifdef UART1_EN
	Uart1_Initial(UART1_BAUTRATE);	//初始化UART1
#endif

#ifdef UART2_EN
	Uart2_Initial(UART2_BAUTRATE);	//初始化UART2
#endif

	EA = 1;													//开全局中断

#ifdef PRINT_EN
	uart_printf("LCD Power Saving Mode Demo Code\n");
#endif

	
//	P72F = P72_XOSCL_IN_SETTING;			//设置P72为晶振引脚
//	P71F = P71_XOSCL_OUT_SETTING;			//设置P71为晶振引脚
//	CKCON |= XLCKE;							//使能XSOCL
//	while(!(CKCON & XLSTA));				//等待XSOCL稳定
//	
//// 	CKCON |= ILCKE;							//使能IRCL

//	LXDIVH = 0;				//设置LCD时钟分频，目标帧频率为64HZ
//	LXDIVL = 0;	
//	LXCAD = 0;
//	LXCFG =	 DMOD(DMOD_5ua) | BIAS(BIAS_1_3) | LDRV(LDRV_7);			//设置LCD驱动电流、偏压(bias)、辉度
//	LXCON =  LEN(LEN_XOSCL) | LMOD(LMOD_lcd);	 						//设置LCD时钟源为XOSCL，选择LCD模式
//// 	LXCON =  LEN(LEN_IRCL) | LMOD(LMOD_lcd);	 						//设置LCD时钟源为IRCL，选择LCD模式
//	
//	//设置LCD全显
//	for(i = 0; i < 9; i++)
//	{
//		INDEX = i;
//		LXDAT = 0xFF;
//	}

////	LCD_Off();
//	
//	I2CCON = 0;						//关闭I2C，否则系统时钟无法关闭
//	CKCON = XLCKE;					//关闭除LCD时钟外的其他时钟
//// 	CKCON = ILCKE;	
//	PWCON &= ~0x08;					//LDO进入低功率模式
//	
//	PCON = (PCON&0x84) | 0x02;      //进入STOP模式
//	_nop_();
//	_nop_();
//	_nop_();
GXHTC3_INIT();	
PWM6_init();
GPIO_init();
INT2_Init();
INT3_Init();
INT4_Init();
INT5_Init();
RTC_init();

RTC_Set(2023,6,17,14,37,0);

	while(1)
	{
//			get_gxth30();	
//		PWMEN  = (1<<PWM_CH6);		//PWM6使能		
//		Delay_ms(10);
//		PWMEN  = ~(1<<PWM_CH6);		//PWM6禁用	
//		Delay_ms(1000);		
		if(40==key_value)
		{
			if(P40 == 0)
			{
				times++;
			}
			else
			{
				key_value=0;
				uart_printf("%Key1 times=%d\n",times);	
//			uart_printf("Key1=S4\n");
			}
		}
		if(41==key_value){key_value=0;	uart_printf("Key2=S3\n");};		
		if(42==key_value){key_value=0;	uart_printf("Key3=S2\n");};		
		if(43==key_value){key_value=0;	uart_printf("Key4=S1\n");};	
		if(ab == 0)
			{
				P32F = OUTPUT;					//P32设置为推挽输出模式	
				P32 = 1;
				ab = 1;
			}
			else
			{
				ab = 0;
				P32F = OUTPUT;					//P32设置为推挽输出模式	
				P32 = 0;
			}

		if(HalfSecFlag)	//半秒打印当前时间
		{
			HalfSecFlag = 0;
	#ifdef PRINT_EN
			RTC_Get();
//			uart_printf("%d-%d-%d %d:%d:%d ( %d )\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);	
			a=calendar.w_year;
			b=calendar.w_month;		
			c=calendar.w_date;		
		    d=calendar.week;
			e=calendar.hour;
			f=calendar.min;
			g=calendar.sec;
			uart_printf("%d/%d/%d-[%d]  %d:%d:%d\n",a,b,c,d,e,f,g);	
				
	#endif		
		}
/*		
		if(AlarmEvFlag)	//闹钟中断产生时打印
		{
			AlarmEvFlag = 0;
	#ifdef PRINT_EN
			uart_printf("Alarm event happen!\n");	
	#endif		
		}
*/		
	}
}
#endif
