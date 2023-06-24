#ifndef _GPIO_H
#define _GPIO_H

#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	

#define PWMDIV_V				(3686400/2700)		//��PWMʱ��Ϊ����ʱ��Ƶ��ʱ������Ӧ�޸Ĳ���
#define PWMDUT_V				(PWMDIV_V/2)		//ռ�ձ�Ϊ50%

void PWM6_init(void);
void GPIO_init(void);
void INT2_Init(void);
void INT3_Init(void);
void INT4_Init(void);
void INT5_Init(void);

#endif
