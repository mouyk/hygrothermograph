#ifndef TIME_H
#define TIME_H
#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	

/*********************************************************************************************************************/
//寄存器T2CON定义
#define TR2(N)   (N<<6)
#define T2R(N)   (N<<4)   	//N=0~3
#define T2IE(N)  (N<<3)
#define UCKS(N)  (N<<2)
#define T2P(N)   (N)	    //N=0~3

//寄存器T2MOD定义
#define TF2		 (1<<7)
#define CF2		 (1<<6)
#define RF2		 (1<<5)
#define CCFG(N)	 (N<<3)	  	//N=0~3
#define T2M(N)	 (N)		//N=0~3


#define INT_TIME			10000			//定时时间，单位为us

#define	TH_VAL				(unsigned char)((0x10000 - (INT_TIME*(FOSC/1000))/1000)>>8)
#define	TL_VAL				(unsigned char)(0x10000 - (INT_TIME*(FOSC/1000))/1000)
/*********************************************************************************************************************/

extern uint8_t times250Flag,times1000Flag;
extern int8_t Timer_Array[2];
extern uint8_t Timer_num;
extern uint8_t Time_start;

void TIME2_init(void);
uint8_t Counting_Function(uint8_t flag);
void sort(uint16_t *a,uint8_t len);
#endif