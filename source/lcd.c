
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
#include "include/gxhtc.h"
#include "include/disp.h"
#include "include/key.h"
#include "include/rtc.h"
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
	for(i = 0; i < 34; i++)
	{
		INDEX = i;
		LXDAT = 0;
	}
	
//	P72F = P72_XOSCL_IN_SETTING;			//����P72Ϊ��������
//	P71F = P71_XOSCL_OUT_SETTING;			//����P71Ϊ��������
//	CKCON |= XLCKE;							//ʹ��XSOCL
//	while(!(CKCON & XLSTA));				//�ȴ�XSOCL�ȶ�
	
// 	CKCON |= ILCKE;							//ʹ��IRCL

	LXDIVH = 0;				//����LCDʱ�ӷ�Ƶ��Ŀ��֡Ƶ��Ϊ64HZ
	LXDIVL = 0;	
	LXCAD = 0;
	LXCFG =	 DMOD(DMOD_80ua) | BIAS(BIAS_1_3) | LDRV(LDRV_7);			//����LCD����������ƫѹ(bias)���Զ�
	LXCON =  LEN(LEN_XOSCL) | LMOD(LMOD_lcd);	 						//����LCDʱ��ԴΪXOSCL��ѡ��LCDģʽ
// 	LXCON =  LEN(LEN_IRCL) | LMOD(LMOD_lcd);	 						//����LCDʱ��ԴΪIRCL��ѡ��LCDģʽ
	
	//����LCDȫ��
	for(i = 0; i < 34; i++)
	{
		INDEX = i;
		LXDAT = 0x0F;
	}

//	LCD_Off();
	
	I2CCON = 0;						//�ر�I2C������ϵͳʱ���޷��ر�
	CKCON = XLCKE;					//�رճ�LCDʱ���������ʱ��
// 	CKCON = ILCKE;	
	PWCON &= ~0x08;					//LDO����͹���ģʽ
	
	PCON = (PCON&0x84) | 0x02;      //����STOPģʽ
	_nop_();

}
void Lcd_Humiture(void)
{
	extern uint8_t lcd_ram[34];
	uint16_t lcdtemp=0,lcdhumi=0;
	
	lcdtemp=GXHTC3_temp*10;
	lcdhumi=GXHTC3_humi;
	UpdateNixieTubeRAMB(lcdtemp/100,27);
	UpdateNixieTubeRAMB(lcdtemp%100/10,25);	
	UpdateNixieTubeRAMB(lcdtemp%100%10,23);	
	UpdateNixieTubeRAMA(lcdhumi/10,19);
	UpdateNixieTubeRAMA(lcdhumi%10,21);	
	if(FahrenFlag == 0)
	{
		lcd_ram[30] = C_Tmp;
	}
	else
	{
		lcd_ram[30] =F_Tmp;
	}
	lcd_ram[25] |= 0x01;     //.
	lcd_ram[23] |= 0x01;      //��ʪ�ȱ�
	lcd_ram[22] |= 0x01;     //%
	if((GXHTC3_humi>=40)&&(GXHTC3_humi<=70))    //ʪ��Ц��
		lcd_ram[20] |=smile_Humi;
	if((GXHTC3_temp>=-10)&&(GXHTC3_temp<=50))    //�¶�Ц��
		lcd_ram[29] |=smile_Temp;
	if(GXHTC3_temp<0)
		lcd_ram[29] |=negative;                    //����
}
void Lcd_Colon(uint8_t flag)
{
	static uint8_t a = 0;
	if(flag == 1)
	{
		if(a == 0)
		{
			lcd_ram[4] |= 0x01;
			a = 1;
		}
		else
		{
			a = 0;
		}
	}
	else
	{
		lcd_ram[4] |= 0x01;
	}
}
void Lcd_IconFunction(void)
{
	lcd_ram[32] = SOC3;
}
void Lcd_WeekDisplay(uint8_t num)
{
	if(num == 1)
		lcd_ram[18] = Monday;
	else if(num == 2)
		lcd_ram[18] = Tuesday;
	else if(num == 3)
		lcd_ram[18] = wednesday;
	else if(num == 0)
		lcd_ram[18] = Sunday;
	if(num == 4)
		lcd_ram[17] |= thursday;
	if(num == 5)
		lcd_ram[8] |= friday;
	if(num == 6)
		lcd_ram[27] |= Saturday;
}
void Lcd_DateFunction(uint16_t year, uint8_t month,uint8_t day)
{
	uint8_t week = 0;
	UpdateNixieTubeRAMA((year-2000)/10,9);
	UpdateNixieTubeRAMA((year-2000)%10,11);
	UpdateNixieTubeRAMA(month%10,13);	
	if(month>=10)	
		lcd_ram[14] |= month1;	
	if((day>=10)&&(day<20))
		lcd_ram[15] = 0x0C;
	else if((day>=20)&&(day<30))
		lcd_ram[15] = 0x07;
	else if(day>=30)
		lcd_ram[15] = 0X0E;
	UpdateNixieTubeRAMA(day%10,16);		
	lcd_ram[10] |=0x01;
	week = RTC_Set_Week(year,month,day);
	Lcd_WeekDisplay(week);
}
#endif
