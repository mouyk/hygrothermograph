
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
	unsigned char i;
	uint8_t ab = 0;
	uint16_t times = 0;
	uint16_t a,b,c,d,e,f,g=0;
#ifdef LVD_RST_ENABLE
	LVDCON = 0xE1;					//����LVD��λ��ѹΪ2V
#endif	
	
#if (SYSCLK_SRC == PLL)
	Sys_Clk_Set_PLL(PLL_Multiple);	//����ϵͳʱ��ΪPLL��PLL_MultipleΪ��Ƶ����
#endif
	
#ifdef UART0_EN
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
INT2_Init();
INT3_Init();
INT4_Init();
INT5_Init();
RTC_init();

RTC_Set(2023,6,17,14,37,0);

	while(1)
	{
//			get_gxth30();	
//		PWMEN  = (1<<PWM_CH6);		//PWM6ʹ��		
//		Delay_ms(10);
//		PWMEN  = ~(1<<PWM_CH6);		//PWM6����	
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
				P32F = OUTPUT;					//P32����Ϊ�������ģʽ	
				P32 = 1;
				ab = 1;
			}
			else
			{
				ab = 0;
				P32F = OUTPUT;					//P32����Ϊ�������ģʽ	
				P32 = 0;
			}

		if(HalfSecFlag)	//�����ӡ��ǰʱ��
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
