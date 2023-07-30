
#ifndef _BUZZER_C_
#define _BUZZER_C_
/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/lcd_led.h"
#include "include/delay.h"
#include "include/gpio.h"
#include "include/pwm.h"
#include "include/buzzer.h"
#include <intrins.h>


uint8_t BeepStart = 0;
/*********************************************************************************************************************			



*********************************************************************************************************************/	
//#define PWMDIV_V				(3686400/2700)		//��PWMʱ��Ϊ����ʱ��Ƶ��ʱ������Ӧ�޸Ĳ���
//#define PWMDUT_V				(PWMDIV_V/2)		//ռ�ձ�Ϊ50%
/*****************************************************************************
 ** \brief	 PWM6_init	
			 ������������� P33 PWM6  2.7K ռ�ձ�50%
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void PWM6_init(void)
{
/* ������������� P33 PWM*/	
	P33F = P33_PWM6_SETTING;		//P33����ΪPWM ���ģʽ
	P33	 = 0;						//Ĭ�Ϸ������ر�
	
	INDEX = PWM_CH6;
	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | MOD(0);		//����PWMʱ��ԴΪIRCH  
	PWMCFG = TOG(0) | 0;
	//����PWMDIV��PWMDUT
	PWMDIVH	= (unsigned char)(PWMDIV_V>>8);			
	PWMDIVL	= (unsigned char)(PWMDIV_V);			
	PWMDUTH	= (unsigned char)(PWMDUT_V>>8);		
	PWMDUTL	= (unsigned char)(PWMDUT_V);	
//---------------------------------------------------------------------------------------------	

 	PWMUPD = (1<<PWM_CH6);		//PWMDIV��PWMDUT����ʹ��
	while(PWMUPD);				//�ȴ��������
 	PWMEN  = ~(1<<PWM_CH6);		//PWM6ʹ��	
	
}

void Buzzer_Control(uint8_t flag)
{
	if(flag == 1)
	{
		Buzzer_Sounds1();
	}
	else if(flag == 2)
	{
		Buzzer_Sounds2();
	}
}

void Buzzer_Sounds1(void)
{
	static i = 0;
	i++;
	if(i == 1)
	{
		PWMEN  = (1<<PWM_CH6);		//PWM6ʹ��
	}
	else if(i >= Buzzernum1)
	{
		PWMEN  = ~(1<<PWM_CH6);		//PWM6����	
		BeepStart = 0;
		 i = 0;
	}
}
uint8_t BuzNum = 0,BuzNum1 = 0;
void Buzzer_Sounds2(void)
{
	if(BuzNum1 < 3)
	{
		BuzNum++;
		if((BuzNum <= Buzzernum2))
		{
			PWMEN  = (1<<PWM_CH6);		//PWM6ʹ��
		}
		else if(BuzNum >= Buzzernum2*2)
		{
			PWMEN  = ~(1<<PWM_CH6);		//PWM6����	
			BuzNum = 0;
			BuzNum1++;
		}
	}
	else
	{
		BuzNum = 0;
		BuzNum1 = 0;
		BeepStart = 0;
	}
}
#endif
#endif
