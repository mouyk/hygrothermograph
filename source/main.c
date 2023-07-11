
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
//#include "include/lcd.h"
#include "include/key.h"
#include "include/gpio.h"
#include "include/gxhtc.h"
#include "include/rtc.h"
#include <intrins.h>
#include "include/pwm.h"
#include "include/adc.h"
#include "mcu_sdk/zigbee.h"
#include "include/disp.h"
/*********************************************************************************************************************			
	������ʵ��4com*9seg��1/3bias LCD��ʾ���ܣ�LCDʱ������ΪXOSCL��IRCL��ʵ��LCD��С����ģʽ��
	
	��Ҫ��ʾ��
	�ڹر�LCD����ǰ���������õ�COM�ź�SEG������Ϊ���ģʽ������͵�ƽ�����Ա���ر�LCD����ʱLCD��������Ӱ����
*********************************************************************************************************************/			
extern uint8_t key_value;

void main(void)
{
extern float GXHTC3_temp,GXHTC3_humi;
extern bit HalfSecFlag;
extern bit AlarmEvFlag;
extern bit millisecondFlag;
extern uint8_t times10Flag;
	unsigned char i;
	uint8_t ab = 0;
	uint16_t a,b,c,d,e,f,g=0;
	extern double VDD_Voltage;
	extern  uint8_t            			lcd_ram[34];		
	
#ifdef LVD_RST_ENABLE
	LVDCON = 0xE1;					//����LVD��λ��ѹΪ2V
#endif	
	
#if (SYSCLK_SRC == PLL)
	Sys_Clk_Set_PLL(PLL_Multiple);	//����ϵͳʱ��ΪPLL��PLL_MultipleΪ��Ƶ����
#endif
	
#ifdef UART0_EN
	Zigbee_GPIO_init();		
	Uart0_Initial(UART0_BAUTRATE);	//��ʼ��UART0
#endif
	
#ifdef UART1_EN
	Uart1_Initial(UART1_BAUTRATE);	//��ʼ��UART1
#endif

#ifdef UART2_EN
	Uart2_Initial(UART2_BAUTRATE);	//��ʼ��UART2
#endif

	EA = 1;													//��ȫ���ж�

#ifdef PRINT_EN
	uart_printf("LCD Power Saving Mode Demo Code\n");
#endif

	
//	P72F = P72_XOSCL_IN_SETTING;			//����P72Ϊ��������
//	P71F = P71_XOSCL_OUT_SETTING;			//����P71Ϊ��������
//	CKCON |= XLCKE;							//ʹ��XSOCL
//	while(!(CKCON & XLSTA));				//�ȴ�XSOCL�ȶ�
//	
//// 	CKCON |= ILCKE;							//ʹ��IRCL

//	LXDIVH = 0;				//����LCDʱ�ӷ�Ƶ��Ŀ��֡Ƶ��Ϊ64HZ
//	LXDIVL = 0;	
//	LXCAD = 0;
//	LXCFG =	 DMOD(DMOD_5ua) | BIAS(BIAS_1_3) | LDRV(LDRV_7);			//����LCD����������ƫѹ(bias)���Զ�
//	LXCON =  LEN(LEN_XOSCL) | LMOD(LMOD_lcd);	 						//����LCDʱ��ԴΪXOSCL��ѡ��LCDģʽ
//// 	LXCON =  LEN(LEN_IRCL) | LMOD(LMOD_lcd);	 						//����LCDʱ��ԴΪIRCL��ѡ��LCDģʽ
//	
//	//����LCDȫ��
//	for(i = 0; i < 9; i++)
//	{
//		INDEX = i;
//		LXDAT = 0xFF;
//	}

////	LCD_Off();
//	
//	I2CCON = 0;						//�ر�I2C������ϵͳʱ���޷��ر�
//	CKCON = XLCKE;					//�رճ�LCDʱ���������ʱ��
//// 	CKCON = ILCKE;	
//	PWCON &= ~0x08;					//LDO����͹���ģʽ
//	
//	PCON = (PCON&0x84) | 0x02;      //����STOPģʽ
//	_nop_();
//	_nop_();
//	_nop_();
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
RTC_Set(2023,7,11,12,04,41);
Lcd_init();

//zigbee_protocol_init();

//mcu_exit_zigbee();
	while(1)
	{
//		zigbee_uart_service();
//		PWMEN  = (1<<PWM_CH6);		//PWM6ʹ��		
//		Delay_ms(10);
//		PWMEN  = ~(1<<PWM_CH6);		//PWM6����	
//		Delay_ms(1000);	
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
		}
//		if(ab == 0)
//			{
//				P32F = OUTPUT;					//P32����Ϊ�������ģʽ	
//				P32 = 1;
//				ab = 1;
//			}
//			else
//			{
//				ab = 0;
//				P32F = OUTPUT;					//P32����Ϊ�������ģʽ	
//				P32 = 0;
//			}
	for(i = 0; i < 34; i++)
	{
		INDEX = i;
		LXDAT = lcd_ram[i];
	}
		if(HalfSecFlag)	//�����ӡ��ǰʱ��
		{
			HalfSecFlag = 0;
	#ifdef PRINT_EN
			RTC_Get();		get_gxth30();
//			uart_printf("%d-%d-%d %d:%d:%d ( %d )\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);	
			a=calendar.w_year;
			b=calendar.w_month;		
			c=calendar.w_date;		
		    d=calendar.week;
			e=calendar.hour;
			f=calendar.min;
			UpdateNixieTubeRAMA(e/10,1);
			UpdateNixieTubeRAMA(e%10,3);
			
			UpdateNixieTubeRAMA(f/10,5);
			UpdateNixieTubeRAMA(f%10,7);

			Lcd_Colon(1);
			Lcd_IconFunction();
			Lcd_Humiture();
			Lcd_DateFunction(calendar.w_year,calendar.w_month,calendar.w_date);
			g=calendar.sec;
			uart_printf("%d/%d/%d-[%d]  %d:%d:%d\n",a,b,c,d,e,f,g);	
//			mcu_join_zigbee();
//			Uart0_PutChar(0x31);
//			uart_printf("Current Voltage %f\n",VDD_Voltage);					
//	#endif		
			
			
		}
/*		
		if(AlarmEvFlag)	//�����жϲ���ʱ��ӡ
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
