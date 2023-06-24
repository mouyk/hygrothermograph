#ifndef _GPIO_H
#define _GPIO_H

#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	

#define PWMDIV_V				(3686400/2700)		//当PWM时钟为其他时钟频率时，需相应修改参数
#define PWMDUT_V				(PWMDIV_V/2)		//占空比为50%

void PWM6_init(void);
void GPIO_init(void);
void INT2_Init(void);
void INT3_Init(void);
void INT4_Init(void);
void INT5_Init(void);

#endif
