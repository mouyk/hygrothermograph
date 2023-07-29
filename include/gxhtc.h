#ifndef _GXHTC_H
#define _GXHTC_H

#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	

//如果移植程序时只要改一下三个地方就行了
/* 定时使用的IO口 */


#define GXHTC3_SCL_H P37=1
#define GXHTC3_SCL_L P37=0

#define GXHTC3_SDA_H P36=1
#define GXHTC3_SDA_L P36=0

/* 声明全局函数 */
void GXHTC3_INIT(void);
void GXHTC3_SDA_OUT(void);
void GXHTC3_SDA_IN(void);
void GXHTC3_StarT(void);
void GXHTC3_StoP(void);
void GXHTC3_Ack(void);
void GXHTC3_NAck(void);
uint8_t   GXHTC3_Wait_Ack(void);
void GXHTC3_Send_Byte(uint8_t txd);
uint8_t   GXHTC3_Read_Byte(uint8_t ack);
void GXHTC3_read_result(uint8_t addr);
void GXHTC3_SDA_in(void);
void al_float_buffer_sort(float *buf, uint8_t length);
void get_gxth30(void);

extern float GXHTC3_temp,GXHTC3_humi,GXHTC3_Temperature,GXHTC3_Humidity;
#endif
