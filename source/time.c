#ifndef _TIME_C_
#define _TIME_C_

#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/ca51f_config.h"		
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/time.h"
#include "include/key.h"

int8_t Timer_Array[2]={0};
uint8_t Timer_num = 0;									//0����ʱ��ʶ    1��ʱ     2����
uint8_t Time_start = 0;
void TIME2_init(void)
{
	P12F = P12_T2CP_SETTING;	 
	T2CON = TR2(0) | T2R(2) | T2IE(1) | UCKS(0) | T2P(0); 			//���ö�ʱ��Ϊ����ģʽ0																	                                                     
	T2MOD = CCFG(0) | T2M(0);						//����Ϊ��ʱ����ģʽ
	T2CH = TH_VAL;									//����T2����ֵ
	T2CL = TL_VAL;
	TH2 = TH_VAL;									//���ü�����ֵ
	TL2 = TL_VAL;
	T2CON |= TR2(1);   								//��ʱ��2ʹ��
	ET2 = 1;    									//��ʱ��2�ж�ʹ��
}
/***********************************************************************************
��������		TIMER2_ISR
����˵���� 		time2�����жϣ�����times250��250ms��times1000��1S
��������� 		��
����ֵ��		��
***********************************************************************************/
uint8_t times250Flag = 0,times1000Flag = 0;
uint8_t times250 = 0,times1000 = 0;
void TIMER2_ISR (void) interrupt 5 
{
	if(T2MOD & TF2)		  //��ʱ��2����ж�,��ǰ����Ϊ10ms����
	{
		T2MOD = (T2MOD&0x1F) | TF2;
		times250++;
		times1000++;
		if(times250>=25)
		{
			times250 = 0;
			times250Flag = 1;
		}
		if(times1000>=100)
		{
			times1000 = 0;
			times1000Flag = 1;
		}
	}
	if(T2MOD & RF2)		  //��ʱ��2�����ж�
	{
		T2MOD = (T2MOD&0x1F) | RF2;
		
	}
}
uint8_t Counting_Function(uint8_t flag)
{
	static time_num = 3;
	if(flag == 1)
	{
		if(time_num != 0)
		{
			time_num--;
			uart_printf("A");
		}
		if(time_num == 0)
		{
			Timer_Array[1]--;
			if((Timer_Array[1] == -1)&&(Timer_Array[0]*60 != 0))
			{
				Timer_Array[1] = 59;
				Timer_Array[0]--;
			}
			if(Timer_Array[0]*60 + Timer_Array[1] == -1)
			{
				flag = 0;
				Interface = 0;
				time_num = 3;
				uart_printf("B");
			}
		}
	}
	return flag;
}

#endif