#ifndef _TIME_C_
#define _TIME_C_

#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/ca51f_config.h"		
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"

#include "include/time.h"

uint8_t Timer_Array[2]={0};
uint8_t Timer_num = 0;									//0：计时标识    1：时     2：分
void TIME2_init(void)
{
	P12F = P12_T2CP_SETTING;	 
	T2CON = TR2(0) | T2R(2) | T2IE(1) | UCKS(0) | T2P(0); 			//设置定时器为重载模式0																	                                                     
	T2MOD = CCFG(0) | T2M(0);						//设置为定时计数模式
	T2CH = TH_VAL;									//设置T2重载值
	T2CL = TL_VAL;
	TH2 = TH_VAL;									//设置计数初值
	TL2 = TL_VAL;
	T2CON |= TR2(1);   								//定时器2使能
	ET2 = 1;    									//定时器2中断使能
}
/***********************************************************************************
函数名：		TIMER2_ISR
功能说明： 		time2产生中断，其中times250：250ms，times1000：1S
输入参数： 		无
返回值：		无
***********************************************************************************/
uint8_t times250Flag = 0,times1000Flag = 0;
uint8_t times250 = 0,times1000 = 0;
void TIMER2_ISR (void) interrupt 5 
{
	if(T2MOD & TF2)		  //定时器2溢出中断,当前设置为10ms产生
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
	if(T2MOD & RF2)		  //定时器2重载中断
	{
		T2MOD = (T2MOD&0x1F) | RF2;
		
	}
}


#endif