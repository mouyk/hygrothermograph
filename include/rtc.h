
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

#define RTC_Status_Hour   0
#define RTC_Status_Min    1
#define RTC_Status_Year   2
#define RTC_Status_Mon    3
#define RTC_Status_Data   4
#define RTC_Status_Alarm  5
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

} _rtccalendar_obj;

//结构体
typedef struct{
    int8_t hour;  														//时
    int8_t min;  															//分
}time_struct;

typedef struct{      //各个闹钟设置成功至1
    uint8_t Alarm1;														//闹钟1设置成功标志1
    uint8_t Alarm2;														//闹钟2设置成功标志1
		uint8_t Alarm3;														//闹钟3设置成功标志1
}Alarmnum_struct;

typedef struct{
		int8_t Alarm_num;														//1：闹钟1   2：闹钟2    3：闹钟3
		int8_t Alarm_flag;														//0：闹钟标识 :1：闹钟时  2：闹钟分
		uint8_t sleepnum;                             //睡眠计数
		Alarmnum_struct LastAlarmnum;      //上次各个闹钟设置成功至1
		Alarmnum_struct Alarmnum;      //当下各个闹钟设置成功至1
		uint16_t AlarmTime[3];					//用于闹钟比较大小的数组
		time_struct LastAlarm_Array[3]; 			//上次3个闹钟的数组 
    time_struct Alarm_Array[3]; 					//当下3个闹钟的数组 
}Alarm_struct;

extern 	_rtccalendar_obj calendar;
extern 	_rtccalendar_obj RTC_Array;
extern bit times10Flag;
extern bit HalfSecFlag;
extern bit AlarmEvFlag;
extern bit millisecondFlag;
extern uint8_t RTC_num;
extern Alarm_struct AlarmTimes;

void RTC_WriteSecond(unsigned char second);
void RTC_WriteMinute(unsigned char minute);
void RTC_WriteHour(unsigned char hour);
void RTC_WriteWeek(unsigned char week);
void RTC_init(void);
void RTC_Alarm_init(uint8_t flag,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Daysmonth(uint16_t year, uint8_t month);
uint8_t RTC_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec );
//uint8_t RTC_Alarm_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec );
uint8_t RTC_Get1(void);
uint8_t RTC_Set_Week( uint16_t year, uint8_t month, uint8_t day );
void RTC_AlarmHandle(uint8_t num);
void RTC_AlarmTimes(uint8_t num);
void RTC_AlarmCompare(uint8_t num);
void sort(uint16_t *a,uint8_t len);
void RTC_BuzzerControl(void);

#endif