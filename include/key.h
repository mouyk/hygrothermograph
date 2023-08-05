#ifndef KEY_H
#define KEY_H
#include "stdio.h"
#include "string.h"
#include "include/ca51f_config.h"	

#define Maxnum  100       /*!< 计数上限*/
#define Shortnum  50       /*!< 计数上限*/

//结构体
typedef struct{
    uint8_t times;
    
    uint8_t key_value;
		uint8_t ShortKey;
    uint8_t LongKey;
	
		uint8_t KeyFlag;
   
}Key;

extern bit FahrenFlag;
extern bit HourFlag;
extern uint8_t Interface;
extern uint8_t Hold_down;
extern uint8_t keyclearnum;
extern uint8_t ZigbeeFlag,DelAlarmFlag;
extern Key Key1,Key2,Key3,Key4;

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
void Key_RockonTime(void);

#endif
