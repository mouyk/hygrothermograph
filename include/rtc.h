#ifndef RTC_H
#define RTC_H
//RTCON定义
#define RTCE(N)		(N<<7) 
#define MSE(N)		(N<<6) 
#define HSE(N)		(N<<5)
#define SCE(N)		(N<<4)
#define MCE(N)		(N<<3)
#define HCE(N)		(N<<2)
#define RTCWE(N)	(N<<1)


//RTCIF定义
#define RTC_MF		(1<<2)
#define RTC_HF		(1<<1)
#define RTC_AF		(1<<0)

//结构体
typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    //公历日月年周
    uint16_t w_year;
    uint8_t w_month;
    uint8_t w_date;
    uint8_t week;

} _calendar_obj;

extern 	_calendar_obj calendar;
extern bit times10Flag;
extern bit HalfSecFlag;
extern bit AlarmEvFlag;
extern bit millisecondFlag;
extern int16_t RTC_Array[6];
extern uint8_t RTC_num;
extern uint8_t LastAlarm1,LastAlarm2,LastAlarm3;					//各个闹钟设置成功至1
extern uint8_t Alarm1,Alarm2,Alarm3;					//各个闹钟设置成功至1
extern int8_t Alarm_num;														//0：闹钟1   1：闹钟2    2：闹钟3
extern int8_t Alarm_flag;														//0：闹钟标识 :1：闹钟时  2：闹钟分
extern int8_t LastAlarm_Array[6];
extern int8_t Alarm_Array[6];

void RTC_WriteSecond(unsigned char second);
void RTC_WriteMinute(unsigned char minute);
void RTC_WriteHour(unsigned char hour);
void RTC_WriteWeek(unsigned char week);
void RTC_init(void);
void RTC_Alarm_init(uint8_t flag,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Daysmonth(uint16_t year, uint8_t month);
uint8_t RTC_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec );
//uint8_t RTC_Alarm_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec );
uint8_t RTC_Get(void);
uint8_t RTC_Set_Week( uint16_t year, uint8_t month, uint8_t day );
void RTC_AlarmHandle(uint8_t num);
void RTC_AlarmTimes(uint8_t num);
void RTC_AlarmCompare(uint8_t num);
void sort(uint16_t *a,uint8_t len);
void RTC_BuzzerControl(void);

#endif