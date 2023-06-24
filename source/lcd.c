
#ifndef _LCD_C_
#define _LCD_C_
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
#include <intrins.h>
/*********************************************************************************************************************			
	������ʵ��4com*9seg��1/3bias LCD��ʾ���ܣ�LCDʱ������ΪXOSCL��IRCL��ʵ��LCD��С����ģʽ��
	
	��Ҫ��ʾ��
	�ڹر�LCD����ǰ���������õ�COM�ź�SEG������Ϊ���ģʽ������͵�ƽ�����Ա���ر�LCD����ʱLCD��������Ӱ����
*********************************************************************************************************************/			
void LCD_Off(void)
{
	//�ڹر�LCD����ǰ���������õ�COM�ź�SEG������Ϊ���ģʽ������͵�ƽ�����Ա���ر�LCD����ʱLCD��������Ӱ����
	P00 = 0;
	P01 = 0;
	P02 = 0;
	P03 = 0;	 
       
	P57 = 0;
	P34 = 0; 
	P35 = 0;  
	P56 = 0; 
	P50 = 0;
	P51 = 0;
	P52 = 0; 
	P53 = 0;
	P54 = 0;
	
	P00F = OUTPUT;
	P01F = OUTPUT;
	P02F = OUTPUT;
	P03F = OUTPUT;	 
       
	P57F = OUTPUT;
	P34F = OUTPUT; 
	P35F = OUTPUT;  
	P56F = OUTPUT; 
	P50F = OUTPUT;
	P51F = OUTPUT;
	P52F = OUTPUT; 
	P53F = OUTPUT;
	P54F = OUTPUT;
	
	LXCON = 0;						//�ر�LCD����
}

void Lcd_init(void)
{
	unsigned char i;
	EA = 1;													//��ȫ���ж�

#ifdef PRINT_EN
	uart_printf("LCD Power Saving Mode Demo Code\n");
#endif

		//��ʼ��LCD��������
	P00F = P00_COM0_SETTING;
	P01F = P01_COM1_SETTING;
	P02F = P02_COM2_SETTING;
	P03F = P03_COM3_SETTING;	 
       
	P57F = P57_SEG0_SETTING;
	P34F = P34_SEG1_SETTING; 
	P35F = P35_SEG2_SETTING;  
	P56F = P56_SEG3_SETTING; 
	P50F = P50_SEG4_SETTING;
	P51F = P51_SEG5_SETTING;
	P52F = P52_SEG6_SETTING; 
	P53F = P53_SEG7_SETTING;
	
	P54F = P54_SEG8_SETTING;
	P55F = P55_SEG9_SETTING;
	P60F = P60_SEG10_SETTING;	
	P61F = P61_SEG11_SETTING;
	P62F = P62_SEG12_SETTING;		
	P63F = P63_SEG13_SETTING;	
	P64F = P64_SEG14_SETTING;	
	P65F = P65_SEG15_SETTING;	

	P10F = P10_SEG16_SETTING; 
	P11F = P11_SEG17_SETTING; 
	P12F = P12_SEG18_SETTING; 
	P13F = P13_SEG19_SETTING; 
	P14F = P14_SEG20_SETTING; 
	P15F = P15_SEG21_SETTING; 
	P16F = P16_SEG22_SETTING; 
	P17F = P17_SEG23_SETTING; 
	
	P27F = P27_SEG24_SETTING; 
	P26F = P26_SEG25_SETTING; 
	P25F = P25_SEG26_SETTING; 
	P24F = P24_SEG27_SETTING; 
	P23F = P23_SEG28_SETTING; 
	P22F = P22_SEG29_SETTING; 
	P21F = P21_SEG30_SETTING; 
	P20F = P20_SEG31_SETTING; 
	
	P07F = P07_LCD_SEG32_SETTING; 
	P06F = P06_LCD_SEG33_SETTING;


	//LCD RAM����
	for(i = 0; i < 9; i++)
	{
		INDEX = i;
		LXDAT = 0;
	}
	
	P72F = P72_XOSCL_IN_SETTING;			//����P72Ϊ��������
	P71F = P71_XOSCL_OUT_SETTING;			//����P71Ϊ��������
	CKCON |= XLCKE;							//ʹ��XSOCL
	while(!(CKCON & XLSTA));				//�ȴ�XSOCL�ȶ�
	
// 	CKCON |= ILCKE;							//ʹ��IRCL

	LXDIVH = 0;				//����LCDʱ�ӷ�Ƶ��Ŀ��֡Ƶ��Ϊ64HZ
	LXDIVL = 0;	
	LXCAD = 0;
	LXCFG =	 DMOD(DMOD_5ua) | BIAS(BIAS_1_3) | LDRV(LDRV_7);			//����LCD����������ƫѹ(bias)���Զ�
	LXCON =  LEN(LEN_XOSCL) | LMOD(LMOD_lcd);	 						//����LCDʱ��ԴΪXOSCL��ѡ��LCDģʽ
// 	LXCON =  LEN(LEN_IRCL) | LMOD(LMOD_lcd);	 						//����LCDʱ��ԴΪIRCL��ѡ��LCDģʽ
	
	//����LCDȫ��
	for(i = 0; i < 34; i++)
	{
		INDEX = i;
		LXDAT = 0xFF;
	}

//	LCD_Off();
	
	I2CCON = 0;						//�ر�I2C������ϵͳʱ���޷��ر�
	CKCON = XLCKE;					//�رճ�LCDʱ���������ʱ��
// 	CKCON = ILCKE;	
	PWCON &= ~0x08;					//LDO����͹���ģʽ
	
	PCON = (PCON&0x84) | 0x02;      //����STOPģʽ
	_nop_();

}
#endif
