#ifndef _GXHTC3_C_
#define _GXHTC3_C_
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
#include "include/gxhtc.h"
#include <intrins.h>
#include "stdio.h"

#define write 0
#define read  1
float GXHTC3_temp,GXHTC3_humi,GXHTC3_Temperature,GXHTC3_Humidity;

/****************************************************************************
* Function Name  : GXHTC3_INIT
* Description    : 初始化GPIO.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
void GXHTC3_INIT()
{	
	P36F = OUTPUT;//|OP_EN;
	P37F = OUTPUT;//|OP_EN;		

	GXHTC3_SCL_H;
	GXHTC3_SDA_H;
}

/*******************************************************************************
* 函 数 名         : GXHTC3_SDA_OUT
* 函数功能		   : SDA输出配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void GXHTC3_SDA_OUT()
{
	P36F = OUTPUT|OP_EN;
	//P37F = OUTPUT|OP_EN;		
}

/*******************************************************************************
* 函 数 名         : GXHTC3_SDA_IN
* 函数功能		   : SDA输入配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void GXHTC3_SDA_IN(void)
{
	P36F = INPUT;	
}

//void GXHTC3_SDA_in(void)
//{
//	P36F = INPUT;		
//}
//产生起始信号
void GXHTC3_StarT(void)
{
  GXHTC3_SDA_OUT();
	
	GXHTC3_SDA_H;
	GXHTC3_SCL_H;
	delay_us(5);
	GXHTC3_SDA_L;
	delay_us(6);
	GXHTC3_SCL_L;
}

//产生停止信号
void GXHTC3_StoP(void)
{
   GXHTC3_SDA_OUT();

   GXHTC3_SCL_L;
   GXHTC3_SDA_L;
   GXHTC3_SCL_H;
   delay_us(6);
   GXHTC3_SDA_H;
   delay_us(6);
}

//主机产生应答信号ACK
void GXHTC3_Ack(void)
{
   GXHTC3_SCL_L;
   GXHTC3_SDA_OUT();
   GXHTC3_SDA_L;
   delay_us(2);
   GXHTC3_SCL_H;
   delay_us(5);
   GXHTC3_SCL_L;
}

//主机不产生应答信号NACK
void GXHTC3_NAck(void)
{
   GXHTC3_SCL_L;
   GXHTC3_SDA_OUT();
   GXHTC3_SDA_H;
   delay_us(2);
   GXHTC3_SCL_H;
   delay_us(5);
   GXHTC3_SCL_L;
}
//等待从机应答信号
//返回值：1 接收应答失败
//		  	0 接收应答成功
uint8_t GXHTC3_Wait_Ack(void)
{
	uint8_t tempTime=0;
	GXHTC3_SDA_IN();
	GXHTC3_SDA_H;
	delay_us(1);
	GXHTC3_SCL_H;
	delay_us(1);

	while(P36)
	{
		tempTime++;
		delay_us(1);
		if(tempTime>250)
		{
			GXHTC3_StoP();
			return 1;
		}	 
	}
	GXHTC3_SCL_L;
	delay_us(1);
	return 0;
}
//GXHTC3 发送一个字节
void GXHTC3_Send_Byte(uint8_t txd)
{
	uint8_t i=0;
	GXHTC3_SDA_OUT();
	GXHTC3_SCL_L;//拉低时钟开始数据传输

	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) //0x80  1000 0000
			GXHTC3_SDA_H;
		else
			GXHTC3_SDA_L;

		txd<<=1;
		delay_us(1);
		GXHTC3_SCL_H;
		delay_us(2); //发送数据
		GXHTC3_SCL_L;
		delay_us(2);
	}
}

//GXHTC3 读取一个字节

uint8_t GXHTC3_Read_Byte(uint8_t ack)
{
   uint8_t i=0,receive=0;

   GXHTC3_SDA_IN();
   for(i=0;i<8;i++)
   {
   		GXHTC3_SCL_L;
		delay_us(2);
		GXHTC3_SCL_H;
		while(!P37);
		receive<<=1;
		if(P36)
		   receive++;
		delay_us(1);	
   }

   	if(ack==0)
	   	GXHTC3_NAck();
	else
		GXHTC3_Ack();

	return receive;
}


void GXHTC3_read_result(uint8_t addr)
{
	uint16_t tem,hum;
	uint16_t buff[6];

	float Temperature=0;
	float Humidity=0;
		
	GXHTC3_StarT();
	GXHTC3_Send_Byte(addr<<1 | write);//写7位GXHTC3设备地址加0作为写取位,1为读取位
	GXHTC3_Wait_Ack();
	GXHTC3_Send_Byte(0x7C);
	GXHTC3_Wait_Ack();
	GXHTC3_Send_Byte(0xA2);
	GXHTC3_Wait_Ack();
	GXHTC3_StoP();
	
	Delay_ms(15);
	
	GXHTC3_StarT();
	GXHTC3_Send_Byte(addr<<1 | read);//写7位GXHTC3设备地址加0作为写取位,1为读取位
	
	if(GXHTC3_Wait_Ack()==0)
	{
		GXHTC3_SDA_IN();
			
		buff[0]=GXHTC3_Read_Byte(1);
		buff[1]=GXHTC3_Read_Byte(1);
		buff[2]=GXHTC3_Read_Byte(1);
		buff[3]=GXHTC3_Read_Byte(1);
		buff[4]=GXHTC3_Read_Byte(1);
		buff[5]=GXHTC3_Read_Byte(0);
		GXHTC3_StoP();
		
		tem = ((buff[0]<<8) | buff[1]);//温度拼接
		hum = ((buff[3]<<8) | buff[4]);//湿度拼接
		/*转换实际温度*/
		Temperature= (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
		Humidity= (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
	}
	
	if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤错误数据
	{
		GXHTC3_temp = Temperature;
		GXHTC3_humi = Humidity;
	}

	tem = 0;
	hum = 0;
}

void al_float_buffer_sort(float *buf, uint8_t length)
{
	uint8_t i, j;
	float tmp;
	for (i = 0; i < length; i++)
	{
		for (j = i + 1; j < length; j++)
		{
			if (buf[j] < buf[i])
			{
				tmp = buf[j];
				buf[j] = buf[i];
				buf[i] = tmp;
			}
		}
	}
}

void get_gxth30(void)
{
/* 加滤波
	float buff_temp[5]={0,0,0,0,0},buff_humi[5]={0,0,0,0,0};
	uint8_t i=0;
	for(i = 0;i < 5;i++)
	{	
		GXHTC3_read_result(0x70);
		buff_temp[i] = GXHTC3_temp;
		buff_humi[i] = GXHTC3_humi;
	}
	
	al_float_buffer_sort(buff_temp,5);
	al_float_buffer_sort(buff_humi,5);
	
	GXHTC3_Temperature = (buff_temp[2] + buff_temp[4]) / 2;
	GXHTC3_Humidity = (buff_humi[2] + buff_humi[4]) / 2;
	
	uart_printf("Temp=%f ||| Humi=%f /n",GXHTC3_temp,GXHTC3_humi);
*/
	
///* 不加滤波

	float temp=0,humi=0;
	uint8_t i=0;
	for(i = 0;i < 5;i++)
	{	
		GXHTC3_read_result(0x70);
		temp+=GXHTC3_temp;
		humi+=GXHTC3_humi;
	}
	
	GXHTC3_temp = temp / 5;
	GXHTC3_humi = humi / 5;
	
	uart_printf("Temp=%0.2f ||| Humi=%0.2f \n",GXHTC3_temp,GXHTC3_humi);
//*/	
	
}


#endif
