#ifndef RTC_H
#define RTC_H
//RTCON����
#define RTCE(N)		(N<<7) 
#define MSE(N)		(N<<6) 
#define HSE(N)		(N<<5)
#define SCE(N)		(N<<4)
#define MCE(N)		(N<<3)
#define HCE(N)		(N<<2)
#define RTCWE(N)	(N<<1)


//RTCIF����
#define RTC_MF		(1<<2)
#define RTC_HF		(1<<1)
#define RTC_AF		(1<<0)

//�ṹ��
typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    //������������
    uint16_t w_year;
    uint8_t w_month;
    uint8_t w_date;
    uint8_t week;

} _calendar_obj;

extern 	_calendar_obj calendar;

void RTC_WriteSecond(unsigned char second);
void RTC_WriteMinute(unsigned char minute);
void RTC_WriteHour(unsigned char hour);
void RTC_WriteWeek(unsigned char week);
void RTC_init(void);

uint8_t RTC_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec );
uint8_t RTC_Alarm_Set( uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec );
uint8_t RTC_Get( void );
uint8_t RTC_Set_Week( uint16_t year, uint8_t month, uint8_t day );

#endif