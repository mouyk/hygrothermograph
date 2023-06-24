#ifndef _Delay_C_
#define _Delay_C_
#include "include/ca51f_config.h"
/*********************************************************************************************************************/
void Delay_50us(unsigned int n)	   
{
	unsigned char i;
#if (SYSCLK_SRC == PLL)
	n *= PLL_Multiple;
#endif 
	
	while(n--)
	{
		for(i=0;i<15;i++);
	}
}
void delay_us(unsigned int n)
{
	unsigned int i;
for(n=n;n>0;n--)
	{
//		_nop_();
//		_nop_();
//		_nop_();
		for(i=5;i>0;i--);
	}
}
void Delay_ms(unsigned int n)
{
	while(n--)
	{
		Delay_50us(20);
	}
}

/*********************************************************************************************************************/
#endif