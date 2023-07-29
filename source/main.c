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

#include "include/key.h"
#include "include/gpio.h"
#include "include/gxhtc.h"
#include "include/rtc.h"
#include "include/time.h"
#include <intrins.h>
#include "include/pwm.h"
#include "include/adc.h"
#include "mcu_sdk/zigbee.h"
#include "include/buzzer.h"
#include "include/disp.h"
/*********************************************************************************************************************			
	本例程实现4com*9seg、1/3bias LCD显示功能，LCD时钟设置为XOSCL或IRCL，实现LCD最小电流模式。
	
	重要提示：
	在关闭LCD功能前，把所设置的COM脚和SEG脚设置为输出模式并输出低电平，可以避免关闭LCD功能时LCD屏出现拖影现象。
*********************************************************************************************************************/			

void main(void)
{
uint8_t i;
extern uint8_t lcd_ram[34];

#ifdef LVD_RST_ENABLE
	LVDCON = 0xE1;					//设置LVD复位电压为2V
#endif	
	
#if (SYSCLK_SRC == PLL)
	Sys_Clk_Set_PLL(PLL_Multiple);	//设置系统时钟为PLL，PLL_Multiple为倍频倍数
#endif
	
#ifdef UART0_EN
	Zigbee_GPIO_init();		
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

GXHTC3_INIT();	
PWM6_init();
GPIO_init();
KEY_init();
INT2_Init();
INT3_Init();
INT4_Init();
INT5_Init();
RTC_init();
ADC_init();
TIME2_init();
RTC_Set(2023,10,11,23,59,41);
RTC_Alarm_init(0,0,0,0);
Lcd_init();

//zigbee_protocol_init();

//mcu_exit_zigbee();
	while(1)
	{
//		zigbee_uart_service();	
//			Uart0_PutChar(0x55);
//			Uart0_PutChar(0xaa);
//			Uart0_PutChar(0x00);
//			Uart0_PutChar(0x04);	
//			Uart0_PutChar(0x00);
//			Uart0_PutChar(0x00);	
//			Uart0_PutChar(0x03);			
//			mcu_network_start();
		if(times10Flag ==1)
		{
			times10Flag = 0;
			Key_Scanf();
			Key_HandleFunction();
			Buzzer_Control(BeepStart);
		}
		if(times100Flag)
		{
			times100Flag = 0; 
			if((Hold_down == 1))
			{
				if(Interface == 1)
					Lcd_IconFunction(Interface,Timer_num,Hold_down);
				else
					Lcd_IconFunction(Interface,RTC_num,Hold_down);
				Hold_down = 0;
				Key4Flag = 0;
				Key2Flag = 0;
			}
			for(i = 0; i < 34; i++)
			{
				INDEX = i;
				LXDAT = lcd_ram[i];
			}
		}
		if(times250Flag == 1)
		{
			times250Flag = 0;
			if((Hold_down == 0))
			{
				if(Interface == 1)
					Lcd_IconFunction(Interface,Timer_num,Hold_down);
				else
					Lcd_IconFunction(Interface,RTC_num,Hold_down);
			}
				
		}
		if(times1000Flag == 1)
		{
			Lcd_Backlight();
			times1000Flag = 0;
			Time_start = Counting_Function(Time_start);
//			uart_printf("C");
		}
			
		if(HalfSecFlag)	//半秒打印当前时间
		{
			HalfSecFlag = 0;
			RTC_Get();	
			get_gxth30();
			Lcd_Humiture();
//			mcu_join_zigbee();
//			Uart0_PutChar(0x31);
//			uart_printf("Current Voltage %f\n",VDD_Voltage);					
//	#endif		

		}	
		if(AlarmEvFlag)	//闹钟中断产生时打印
		{
			AlarmEvFlag = 0;
			RTC_AlarmCompare(Alarm1+Alarm2+Alarm3);
	#ifdef PRINT_EN
			uart_printf("Alarm event happen!\n");	
	#endif		
		}
		
	}
}
#endif
