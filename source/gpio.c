
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
#include "include/lcd.h"
#include "include/gpio.h"
#include <intrins.h>
/*********************************************************************************************************************			

	P00F = OUTPUT;					//P00设置为推挽输出模式
	P00C |= 0x80;					//P00使能高灌电流模式
	
	P00F = INPUT|PU_EN;				//P00设置为输入模式并使能上拉电阻
	P00C |= 0x20;					//上拉电阻选择为强上拉
// 	P00C &= ~0x20;					//上拉电阻选择为弱上拉

	P00F = INPUT|PD_EN;				//P00设置为输入模式并使能下拉电阻
	P00C |= 0x10;					//下拉电阻选择为强下拉
// 	P00C &= ~0x10;					//下拉电阻选择为弱下拉
	
	P00F = P00_COM0_SETTING;		//P00设置为COM引脚功能

*********************************************************************************************************************/	
uint8_t key_value=0;
//EPCON寄存器定义
#define EPPL(N)	(N<<7)
/*****************************************************************************
 ** \brief	 GPIO_Init	
			 GPIO_Init初始化GPIO函数,比如按键开光输入输出
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void GPIO_init(void)
{
/*按键 上拉输入	P40	| S4	P41	| S3	P42	| S2	P43	| S1	*/
	
	P40F = INPUT|PU_EN;				//P40设置为输入模式并使能上拉电阻 
	P40C |= 0x20;					//上拉电阻选择为强上拉
// 	P40C &= ~0x20;					//上拉电阻选择为弱上拉	
	
	P41F = INPUT|PU_EN;				//P41设置为输入模式并使能上拉电阻 
	P41C |= 0x20;					//上拉电阻选择为强上拉
// 	P41C &= ~0x20;					//上拉电阻选择为弱上拉	
	
	P42F = INPUT|PU_EN;				//P42设置为输入模式并使能上拉电阻 
	P42C |= 0x20;					//上拉电阻选择为强上拉
// 	P42C &= ~0x20;					//上拉电阻选择为弱上拉		
	
	P43F = INPUT|PU_EN;				//P43设置为输入模式并使能上拉电阻 
	P43C |= 0x20;					//上拉电阻选择为强上拉
// 	P43C &= ~0x20;					//上拉电阻选择为弱上拉		
	
/*DCDC 开漏输出	P45	*/
	P45F = OUTPUT|OP_EN;			//P45设置为开漏输出 
//	P45C |= 0x20;					//上拉电阻选择为强上拉
// 	P45C &= ~0x20;					//上拉电阻选择为弱上拉	
	P45 = 0;						//P45输出置零 开启模组电源	
	
/*充电检测。P46 充电插入检测，P47 充电状态检测*/
	P46F = INPUT|PU_EN;				//P46设置为输入模式并使能上拉电阻 
	P46C |= 0x20;					//上拉电阻选择为强上拉
// 	P46C &= ~0x20;					//上拉电阻选择为弱上拉	
	P47F = INPUT|PU_EN;				//P47设置为输入模式并使能上拉电阻 
//	P47C |= 0x20;					//上拉电阻选择为强上拉
 	P47C &= ~0x20;					//上拉电阻选择为弱上拉	
	
/* 背光输出控制 P32*/	
	P32F = OUTPUT;					//P32设置为推挽输出模式	
	P32	 = 1;						//默认背光开启
/* 蜂鸣器输出控制 P33 PWM*/	
	P33F = P33_PWM6_SETTING;		//P33设置为PWM 输出模式
	P33	 = 0;						//默认蜂鸣器关闭
	
	
	
	
}

/*****************************************************************************
 ** \brief	 I2C_init	
			 I2C_init 温湿度接口初始化
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void I2C_init(void)
{
/**********选择I2C端口************************************************************/
	I2CIOS = 0;
	P36F = P36_I2C_SDA_SETTING | PU_EN;
	P37F = P37_I2C_SCL_SETTING | PU_EN;		
}

/*****************************************************************************
 ** \brief	 Zigbee_GPIO_init	
			 Zigbee_GPIO_init与模组管脚通讯pin脚
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void Zigbee_GPIO_init(void)
{
/**********选择UART0端口************************************************************/
	#ifdef UART0_EN
	P31F = P31_UART0_RX_SETTING;
	P30F = P30_UART0_TX_SETTING;	
	#endif
/*	P04 模组唤醒MCU P05  MCU唤醒模组	*/
	P04F = INPUT;				//P04设置为输入模式
	P05F = OUTPUT|OP_EN;		//P05设置为开漏输出 
	P05  = 0;					//P05输出置零
}


/*外部中断2控制例程****************************************************************************************************/
void INT2_Init(void)
{
//	P10F = INPUT;							//P10设置为输入模式
	INDEX = 0;								//EPCON为带索引的寄存器，设置INDEX=0 对应INT2
	EPCON = EPPL(1) | 32;					//设置P40为INT2中断引脚，下降沿触发
	INT2EN = 1; 							//外部中断2中断使能
	EPIE |= 0x01;							//INT2中断使能
	key_value = 0;		
}
void INT2_ISR (void) interrupt 7
{
	if(EPIF & 0x01)
	{
		EPIF = 0x01;
//		int2_flag = 1;
		key_value =40;
	}
}

/*外部中断3控制例程****************************************************************************************************/
void INT3_Init(void)
{
//	P11F = INPUT;							//P11设置为输入模式	
	INDEX = 1;								//INDEX为带索引的寄存器，设置INDEX=1 对应INT3
	EPCON = EPPL(1) | 33;					//设置P41为INT3中断引脚，下降沿触发		
	INT3EN = 1; 							//外部中断3中断使能 
	EPIE |= 0x02;							//INT3中断使能
	key_value = 0;	
}
void INT3_ISR (void) interrupt 8
{
	if(EPIF & 0x02)
	{
		EPIF = 0x02;	
	key_value = 41;	
	}
}
/*********************************************************************************************************************/

/*外部中断4控制例程****************************************************************************************************/
void INT4_Init(void)
{
//	P12F = INPUT;							//P12设置为输入模式		
	INDEX = 2;								//INDEX为带索引的寄存器，设置INDEX=2 对应INT4
	EPCON = EPPL(1) | 34;					//设置P42为INT4中断引脚，下降沿触发				
	INT4EN = 1;  							//外部中断4中断使能
	EPIE |= 0x04;							//INT4中断使能
	key_value = 0;	
}
void INT4_ISR (void) interrupt 9
{
	if(EPIF & 0x04)
	{
		EPIF = 0x04;
	key_value = 42;	
	}
}
/*********************************************************************************************************************/

/*外部中断5控制例程****************************************************************************************************/
void INT5_Init(void)
{
//	P13F = INPUT;							//P13设置为输入模式		
	INDEX = 3;								//INDEX为带索引的寄存器，设置INDEX=3 对应INT5
	EPCON = EPPL(1) | 35;					//设置P43为INT5中断引脚，下降沿触发		
	INT5EN = 1; 							//外部中断5中断使能 
	EPIE |= 0x08;							//INT5中断使能
	key_value = 0;	
}
void INT5_ISR (void) interrupt 10
{
	if(EPIF & 0x08)
	{
		EPIF = 0x08;	
	key_value = 43;	
	}
}

#endif
