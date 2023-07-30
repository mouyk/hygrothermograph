
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
			 ��ȡ VDD��ѹ
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void ADC_init(void)
{
	unsigned int AD_Value;	
/**********��ʼ��ADC�Ĵ���������ADCʱ�ӡ�ѡ��ADC�ο���ѹ������ADC�˷š������ڲ�1.5VУ׼ֵ��***********/
 	ADCON = AST(0) | ADIE(0) | HTME(7) | VSEL(ADC_REF_INNER);		//����ADC�ο���ѹΪ�ڲ�1.5V
	ADCFGH = AOVE(0) | VTRIM(35);						
/**************************************************************************************************/

/**********����ADCͨ����ʱ�ӷ�Ƶ*********************************************************************/
	ADCFGL = ACKD(7) | ADCHS(ADC_VDD);	//ѡ��ADCͨ��Ϊ1/4VDD
/**************************************************************************************************/

	ADCON |= AST(1);								//����ADCת��
	while(!(ADCON & ADIF));							//�ȴ�ADCת�����
	ADCON |= ADIF;									//���ADC�жϱ�־
	AD_Value = ADCDH*256 + ADCDL;					//��ȡADֵ
	AD_Value >>= 4;		
	VDD_Voltage = ((double)AD_Value*1.5*4)/4095;	//����ADCֵ����Ϊ��ѹֵ��ע�⣺����ADC������1/4VDD�����Ի���ΪVDD��ѹʱADCֵҪ����4
}


#endif
#endif