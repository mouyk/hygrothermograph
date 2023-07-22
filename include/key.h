#ifndef KEY_H
#define KEY_H
#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	
//extern uint8_t key_value1,key_value2,key_value3,key_value4;

#define Maxnum  100       /*!< 计数上限*/
#define Shortnum  50       /*!< 计数上限*/

void KEY_init(void);
void INT2_Init(void);
void INT3_Init(void);
void INT4_Init(void);
void INT5_Init(void);
void Key_Scanf(void);
void Key_HandleFunction(void);
void Key_timedate(uint8_t flag);
void Key_Countdown(uint8_t flag);
void Key_Alarm(uint8_t flag);
extern uint8_t FahrenFlag;
extern uint8_t HourFlag;
extern uint8_t Interface;
extern uint8_t ZigbeeFlag,DelAlarmFlag;
#endif