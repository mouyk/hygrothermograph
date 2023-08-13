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

#define Timer_Ico			0
#define Timer_Hour		1
#define Timer_Min			2

//结构体
typedef struct{
    int8_t min;									//分
    int8_t sec;  								//秒
}timer_struct;

#define INT_TIME			10000			//定时时间，单位为us

#define	TH_VAL				(unsigned char)((0x10000 - (INT_TIME*(FOSC/1000))/1000)>>8)
#define	TL_VAL				(unsigned char)(0x10000 - (INT_TIME*(FOSC/1000))/1000)
/*********************************************************************************************************************/

extern bit times100Flag,times250Flag,times1000Flag;
extern timer_struct Timer_Array;
extern uint8_t Timer_num;
extern bit Time_start;

void TIME2_init(void);
bit Counting_Function(bit flag);
#endif