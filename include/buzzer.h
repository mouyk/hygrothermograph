#ifndef BUZZER_H
#define BUZZER_H

#define Buzzernum1  9       /*!< 计数上限   10:长*/	
#define Buzzernum2  4       /*!< 计数上限   10:长*/		

extern uint8_t BeepStart;
extern uint8_t BuzNum,BuzNum1;
void Buzzer_Control(uint8_t flag);
void Buzzer_Sounds1(void);
void Buzzer_Sounds2(void);
#endif