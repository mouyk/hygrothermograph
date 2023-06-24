#ifndef KEY_H
#define KEY_H
#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	
//extern uint8_t key_value1,key_value2,key_value3,key_value4;

#define Maxnum  400       /*!< 计数上限*/
#define Shortnum  100       /*!< 计数上限*/

void KEY_init(void);
void INT2_Init(void);
void INT3_Init(void);
void INT4_Init(void);
void INT5_Init(void);
void Key_Scanf(void);
#endif