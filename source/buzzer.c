
#ifndef _GPIO_C_
#define _GPIO_C_
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
#include <intrins.h>
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


#endif
