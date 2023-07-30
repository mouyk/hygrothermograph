
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
#include "include/adc.h"
#include <intrins.h>
/*********************************************************************************************************************			



*********************************************************************************************************************/	
double VDD_Voltage;
/*****************************************************************************
 ** \brief	 ADC_init	
			 获取 VDD电压
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void ADC_init(void)
{
	unsigned int AD_Value;	
/**********初始化ADC寄存器（设置ADC时钟、选择ADC参考电压、设置ADC运放、加载内部1.5V校准值）***********/
 	ADCON = AST(0) | ADIE(0) | HTME(7) | VSEL(ADC_REF_INNER);		//设置ADC参考电压为内部1.5V
	ADCFGH = AOVE(0) | VTRIM(35);						
/**************************************************************************************************/

/**********设置ADC通道和时钟分频*********************************************************************/
	ADCFGL = ACKD(7) | ADCHS(ADC_VDD);	//选择ADC通道为1/4VDD
/**************************************************************************************************/

	ADCON |= AST(1);								//启动ADC转换
	while(!(ADCON & ADIF));							//等待ADC转换完成
	ADCON |= ADIF;									//清除ADC中断标志
	AD_Value = ADCDH*256 + ADCDL;					//读取AD值
	AD_Value >>= 4;		
	VDD_Voltage = ((double)AD_Value*1.5*4)/4095;	//根据ADC值换算为电压值，注意：由于ADC检测的是1/4VDD，所以换算为VDD电压时ADC值要乘以4
}


#endif
#endif