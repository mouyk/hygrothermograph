#ifndef _ADC_H_
#define _ADC_H_

//ADCON定义
#define AST(N)		(N<<7)					//ADC启动 ， AST=0结束
#define ADIE(N)		(N<<6)					//中断使能
#define ADIF		(1<<5)					//中断标志
#define HTME(N) 	(N<<2)					//N=0-7			 //采样时间设置, 时间为2的HTME次方的时钟周期
#define VSEL(N) 	(N)						//N=0-3			 //选择参考电压 0-内部 1-VDD 2-外部

//ADCFGL定义													   
#define ACKD(N)		(N<<5)  				//N=0-7			 //ADC时钟分频	  分频倍数=（ACKD+1)
#define ADCHS(N)	(N) 					//N=0-15		 //ADC通道选择 ， 1-13对应通道0-12


//ADCFGH定义
#define AOVF		(1<<7)					//ADC数据超过设定范围中断标志
#define AOVE(N)		(N<<6)					//ADC数据超过设定范围中断使能


#define VTRIM(N)	(N)						//0-63	   校正内部参考电压


enum 
{
	ADC_REF_INNER  	= 0,
	ADC_REF_VDD  	= 1,
	ADC_REF_Outer  	= 2
};

enum 
{
	ADC_CH0  	= 1,
	ADC_CH1  	= 2,
	ADC_CH2  	= 3,
	ADC_CH3  	= 4,
	ADC_CH4  	= 5,
	ADC_CH5  	= 6,
	ADC_CH6  	= 7,
	ADC_CH7  	= 8,
	ADC_VDD  	= 9,
};
#endif