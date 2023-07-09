#ifndef _KEY_C_
#define _KEY_C_
#include "include/ca51f_config.h"		
#include "include/ca51f2sfr.h"
#include "include/ca51f2xsfr.h"
#include "include/ca51f_config.h"		
#include "include/gpiodef_f2.h"
#include "include/system_clock.h"
#include "include/key.h"
#include "include/uart.h"
#include <intrins.h>

uint8_t key_value1,key_value2,key_value3,key_value4=0;
uint8_t FahrenFlag = 0;
//EPCON�Ĵ�������
#define EPPL(N)	(N<<7)
/*****************************************************************************
 ** \brief	 KEY_Init	
			 KEY_Init��ʼ��GPIO����,���簴������
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void KEY_init(void)
{
/*���� ��������	P40	| S4	P41	| S3	P42	| S2	P43	| S1	*/
	
	P40F = INPUT|PU_EN;				//P40����Ϊ����ģʽ��ʹ���������� 
	P40C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P40C &= ~0x20;					//��������ѡ��Ϊ������	
	
	P41F = INPUT|PU_EN;				//P41����Ϊ����ģʽ��ʹ���������� 
	P41C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P41C &= ~0x20;					//��������ѡ��Ϊ������	
	
	P42F = INPUT|PU_EN;				//P42����Ϊ����ģʽ��ʹ���������� 
	P42C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P42C &= ~0x20;					//��������ѡ��Ϊ������		
	
	P43F = INPUT|PU_EN;				//P43����Ϊ����ģʽ��ʹ���������� 
	P43C |= 0x20;					//��������ѡ��Ϊǿ����
// 	P43C &= ~0x20;					//��������ѡ��Ϊ������
}

/*�ⲿ�ж�2��������****************************************************************************************************/
void INT2_Init(void)
{
//	P10F = INPUT;							//P10����Ϊ����ģʽ
	INDEX = 0;								//EPCONΪ�������ļĴ���������INDEX=0 ��ӦINT2
	EPCON = EPPL(1) | 32;					//����P40ΪINT2�ж����ţ��½��ش���
	INT2EN = 1; 							//�ⲿ�ж�2�ж�ʹ��
	EPIE |= 0x01;							//INT2�ж�ʹ��
	key_value4 = 0;		
}
void INT2_ISR (void) interrupt 7
{
	if(EPIF & 0x01)
	{
		EPIF = 0x01;
//		int2_flag = 1;
		key_value4 =40;
	}
}

/*�ⲿ�ж�3��������****************************************************************************************************/
void INT3_Init(void)
{
//	P11F = INPUT;							//P11����Ϊ����ģʽ	
	INDEX = 1;								//INDEXΪ�������ļĴ���������INDEX=1 ��ӦINT3
	EPCON = EPPL(1) | 33;					//����P41ΪINT3�ж����ţ��½��ش���		
	INT3EN = 1; 							//�ⲿ�ж�3�ж�ʹ�� 
	EPIE |= 0x02;							//INT3�ж�ʹ��
	key_value3 = 0;	
}
void INT3_ISR (void) interrupt 8
{
	if(EPIF & 0x02)
	{
		EPIF = 0x02;	
	key_value3 = 41;	
	}
}
/*********************************************************************************************************************/

/*�ⲿ�ж�4��������****************************************************************************************************/
void INT4_Init(void)
{
//	P12F = INPUT;							//P12����Ϊ����ģʽ		
	INDEX = 2;								//INDEXΪ�������ļĴ���������INDEX=2 ��ӦINT4
	EPCON = EPPL(1) | 34;					//����P42ΪINT4�ж����ţ��½��ش���				
	INT4EN = 1;  							//�ⲿ�ж�4�ж�ʹ��
	EPIE |= 0x04;							//INT4�ж�ʹ��
	key_value2 = 0;	
}
void INT4_ISR (void) interrupt 9
{
	if(EPIF & 0x04)
	{
		EPIF = 0x04;
	key_value2 = 42;	
	}
}
/*********************************************************************************************************************/

/*�ⲿ�ж�5��������****************************************************************************************************/
void INT5_Init(void)
{
//	P13F = INPUT;							//P13����Ϊ����ģʽ		
	INDEX = 3;								//INDEXΪ�������ļĴ���������INDEX=3 ��ӦINT5
	EPCON = EPPL(1) | 35;					//����P43ΪINT5�ж����ţ��½��ش���		
	INT5EN = 1; 							//�ⲿ�ж�5�ж�ʹ�� 
	EPIE |= 0x08;							//INT5�ж�ʹ��
	key_value1 = 0;	
}
void INT5_ISR (void) interrupt 10
{
	if(EPIF & 0x08)
	{
		EPIF = 0x08;	
	key_value1 = 43;	
	}
}

uint8_t ShortKey1 = 0,ShortKey2 = 0,ShortKey3 = 0,ShortKey4 = 0;
uint8_t LongKey1 = 0,LongKey2 = 0,LongKey3 = 0,LongKey4 = 0;
uint8_t Hold_down1 = 0,Hold_down2 = 0;
void Key_Scanf(void)
{
	uint8_t Key1Flag = 0,Key2Flag = 0,Key3Flag = 0,Key4Flag = 0;
	static uint16_t times4 = 0,times3 = 0,times2 = 0,times1 = 0;
	if(40==key_value4)																						//key4   S4
		{
			if(P40 == 0)
			{
				times4++;
				Key4Flag = 0;
				if(times4>=Maxnum)
				{
					times4 =Maxnum;
				}
			}
			else
			{
				key_value4=0;
				if(times4<Maxnum)
				Key4Flag = 1;
				if((times4 <= Shortnum)&&(Key4Flag == 1))
				{
					ShortKey4 = 1;
					times4 = 0;
					Key4Flag = 0;
				}
				else if((times4 >Shortnum)&&(Key4Flag == 1))
				{
					LongKey4 = 1;
					times4 = 0;
					Key4Flag = 0;
				}
				uart_printf("%Key1 times=%d\n",times4);	
			}
		}
		if(41==key_value3)																						//key3   S3
		{
			if(P41 == 0)
			{
				times3++;
				Key3Flag = 0;
				if(times3>=Maxnum)
				{
					times3 =Maxnum;
					Key3Flag = 2;
				}
			}
			else
			{
				key_value3=0;
				if(times3<Maxnum)
				Key3Flag = 1;
				if((times3 <= Shortnum)&&(Key3Flag == 1))
				{
					ShortKey3 = 1;
					times3 = 0;
					Key3Flag = 0;
				}
				else if((times3 >Shortnum)&&(Key3Flag == 1))
				{
					LongKey3 = 1;
					times3 = 0;
					Key3Flag = 0;
				}
				uart_printf("%Key2 times=%d\n",times3);	
			}
		}
		if((times3 == Maxnum)&&(Key3Flag == 2))
		{
			Hold_down1 = 1;
		}
		if(42==key_value2)																						//key2   S2
		{
			if(P42 == 0)
			{
				times2++;
				Key2Flag = 0;
				if(times2>=Maxnum)
				{
					times2 =Maxnum;
					Key2Flag = 2;
				}
			}
			else
			{
				key_value2=0;
				if(times2<Maxnum)
				Key2Flag = 1;
				if((times2 <= Shortnum)&&(Key2Flag == 1))
				{
					ShortKey2 = 1;
					times2 = 0;
					Key2Flag = 0;
				}
				else if((times2 >Shortnum)&&(Key2Flag == 1))
				{
					LongKey2 = 1;
					times2 = 0;
					Key2Flag = 0;
				}
				uart_printf("%Key3 times=%d\n",times2);	
			}
		}
		if((times2 == Maxnum)&&(Key2Flag == 2))
		{
			Hold_down2 = 1;
		}	
		if(43==key_value1)																						//key1   S1
		{
			if(P43 == 0)
			{
				times1++;
				Key1Flag = 0;
				if(times1>=Maxnum)
				{
					times1 =Maxnum;
				}
			}
			else
			{
				key_value1=0;
				if(times1<Maxnum)
				Key1Flag = 1;
				if((times1 <= Shortnum)&&(Key1Flag == 1))
				{
					ShortKey1 = 1;
					times1 = 0;
					Key1Flag = 0;
				}
				else if((times1 >Shortnum)&&(Key1Flag == 1))
				{
					LongKey1 = 1;
					times1 = 0;
					Key1Flag = 0;
				}
				uart_printf("%Key4 times=%d\n",times1);	
			}
		}		
}
#endif