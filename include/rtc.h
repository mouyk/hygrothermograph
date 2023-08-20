
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

#define RTC_Status_Hour   0
#define RTC_Status_Min    1
#define RTC_Status_Year   2
#define RTC_Status_Mon    3
#define RTC_Status_Data   4
#define RTC_Status_Alarm  5
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

} _rtccalendar_obj;

//�ṹ��
typedef struct{
    int8_t hour;  														//ʱ
    int8_t min;  															//��
}time_struct;

typedef struct{      //�����������óɹ���1
    uint8_t Alarm1;														//����1���óɹ���־1
    uint8_t Alarm2;														//����2���óɹ���־1
		uint8_t Alarm3;														//����3���óɹ���־1
}Alarmnum_struct;

typedef struct{
		int8_t Alarm_num;														//1������1   2������2    3������3
		int8_t Alarm_flag;														//0�����ӱ�ʶ :1������ʱ  2�����ӷ�
		uint8_t sleepnum;                             //˯�߼���
		Alarmnum_struct LastAlarmnum;      //�ϴθ����������óɹ���1
		Alarmnum_struct Alarmnum;      //���¸����������óɹ���1
		uint16_t AlarmTime[3];					//�������ӱȽϴ�С������
		time_struct LastAlarm_Array[3]; 			//�ϴ�3�����ӵ����� 
    time_struct Alarm_Array[3]; 					//����3�����ӵ����� 
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