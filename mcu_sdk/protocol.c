/**
* @file  protocol.c
* @brief this file contains protocol analysis and construct response function when received zigbee module send message
* @author qinlang
* @date 2022.05.06
* @par email:qinlang.chen@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/
#include <intrins.h>
#include "include/zigbee.h"
#include "include/uart.h"
#include "include/gxhtc.h"
#include "include/rtc.h"
#include "include/key.h"
#include "include/adc.h"
#include "include/protocol.h"
#include "include/ca51f_config.h"	
#include "include/ca51f2sfr.h"


extern void Uart0_PutChar(unsigned char value);
extern void uart_printf(char *fmt,...);
/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的zigbee_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/
/******************************************************************************
                              第一步:初始化
1:在需要使用到zigbee相关文件的文件中include "zigbee.h"
2:在MCU初始化中调用mcu_api.c文件中的zigbee_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的zigbee_uart_service()函数

6:mcu何时开启zigbee模块配网，当zigbee模块查询到mcu的产品信息之后，或者zigbee模块发送了当前网络状态
给到mcu，查询pid信息会在上电5秒之后发送,对接部分zigbee老版本通用对接固件低功耗版本尤其需要注意不要
在第一次确认波特率的时候提前唤醒或者发送串口给zigbee模组，有概率导致zigbee模组波特率存储失败。

7：上电之后，mcu何时上报数据给网关合适，在收到网络状态为已配网，或者接收到读取dp数据的时候。
8: 在mcu ota过程中发送固件内容请求未收到zigbee模块的应答时，需要mcu端做好超时重新请求的处理，和存储image
处理，校验和可以自行设计或者使用参考。
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/

///> dp data list, this will be generated by cloud platform
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_TEMP_CURRENT, DP_TYPE_VALUE},
  {DPID_HUMIDITY_VALUE, DP_TYPE_VALUE},
  {DPID_BATTERY_STATE, DP_TYPE_ENUM},
  {DPID_BATTERY_PERCENTAGE, DP_TYPE_VALUE},
  {DPID_CHARGE_STATE, DP_TYPE_BOOL},
  {DPID_TEMP_UNIT_CONVERT, DP_TYPE_ENUM},
  {DPID_CLOCK_SET, DP_TYPE_RAW},
  {DPID_TIME_MODE, DP_TYPE_ENUM},
  {DPID_SNOOZE_TIMES, DP_TYPE_VALUE},
  {DPID_SNOOZE, DP_TYPE_BOOL},
  {DPID_SNOOZE_TIME, DP_TYPE_VALUE},
  {DPID_BACKLIGHT_SWITCH, DP_TYPE_BOOL},
  {DPID_MAXTEMP_SET, DP_TYPE_VALUE},
  {DPID_MINITEMP_SET, DP_TYPE_VALUE},
  {DPID_MAXHUM_SET, DP_TYPE_VALUE},
  {DPID_MINIHUM_SET, DP_TYPE_VALUE},
  {DPID_LANGUAGE, DP_TYPE_ENUM},
};


/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

static void report_mcu_ota_result(unsigned char  res);


/**
* @brief encapsulates a generic send function, developer should use their own function to completing this fuction 
* @param[in] {value} send signle data 
* @return  void
*/
void uart_transmit_output(unsigned char value)
{
 // #error "please use your own uart send fuction complete this fuction, exmaple"

//  extern void Uart_PutChar(unsigned char value);
	//Uart0_PutChar(0x32);
   Uart0_PutChar(value);	
}

/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/**
* @brief Upload all dp information of the system, and realize the synchronization of APP and muc data
* @param[in] {void}
* @return  void
*/
void all_data_update(void)
{
 // #error "mcu must realize function internal function"
   /*
  * these fucntion will be generated by cloud platform, but when use these function must reference the function in  mcu_api.c
 */
     mcu_dp_value_update(DPID_TEMP_CURRENT,GXHTC3_temp*10); //VALUE型数据上报;
     mcu_dp_value_update(DPID_HUMIDITY_VALUE,GXHTC3_humi); //VALUE型数据上报;
//    mcu_dp_enum_update(DPID_BATTERY_STATE,当前电池电量状态); //枚举型数据上报;
    mcu_dp_value_update(DPID_BATTERY_PERCENTAGE,Socnum); //VALUE型数据上报;
    mcu_dp_bool_update(DPID_CHARGE_STATE,Charge_State); //BOOL型数据上报;
//    mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT,FahrenFlag); //枚举型数据上报;
//    mcu_dp_raw_update(DPID_CLOCK_SET,当前闹钟设置指针,当前闹钟设置数据长度); //RAW型数据上报;
//    mcu_dp_enum_update(DPID_TIME_MODE,HourFlag); //枚举型数据上报;
//    mcu_dp_value_update(DPID_SNOOZE_TIMES,SnoozeNum); //VALUE型数据上报;
//    mcu_dp_bool_update(DPID_SNOOZE,当前贪睡开关); //BOOL型数据上报;
//    mcu_dp_value_update(DPID_SNOOZE_TIME,Snoozetime); //VALUE型数据上报;
//    mcu_dp_bool_update(DPID_BACKLIGHT_SWITCH,P32); //BOOL型数据上报;
//    mcu_dp_value_update(DPID_MAXTEMP_SET,当前温度上限设置); //VALUE型数据上报;
//    mcu_dp_value_update(DPID_MINITEMP_SET,当前温度下限设置); //VALUE型数据上报;
//    mcu_dp_value_update(DPID_MAXHUM_SET,当前湿度上限设置); //VALUE型数据上报;
//    mcu_dp_value_update(DPID_MINIHUM_SET,当前湿度下限设置); //VALUE型数据上报;
//    mcu_dp_enum_update(DPID_LANGUAGE,当前语言); //枚举型数据上报;

}

/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/

///> this will realize by  cloud platform

/*****************************************************************************
函数名称 : dp_download_temp_unit_convert_handle
功能描述 : 针对DPID_TEMP_UNIT_CONVERT的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_temp_unit_convert_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char temp_unit_convert;
    
    temp_unit_convert = mcu_get_dp_download_enum(value,length);
    switch(temp_unit_convert) {
        case 0:
					FahrenFlag = 0;
        break;
        
        case 1:
					FahrenFlag = 1;
        break;
        
        default:
    
        break;
    }
    
    //There should be a report after processing the DP
    ret = mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT, temp_unit_convert);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_clock_set_handle
功能描述 : 针对DPID_CLOCK_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_clock_set_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为RAW
    unsigned char ret;
    /*  */
		Uart0_PutChar(0x21);
    //RAW type data processing
//    switch(length) {
//        case 0x01:
//					AlarmTimes.Alarm_Array[0].hour = 0;
//					AlarmTimes.Alarm_Array[0].min = 0;
//					AlarmTimes.Alarm_Array[1].hour = 0;
//					AlarmTimes.Alarm_Array[1].min = 0;
//					AlarmTimes.Alarm_Array[2].hour = 0;
//					AlarmTimes.Alarm_Array[2].min = 0;
//					AlarmTimes.Alarmnum.Alarm1 = 0;
//					AlarmTimes.Alarmnum.Alarm2 = 0;
//					AlarmTimes.Alarmnum.Alarm3 = 0;
//					RTC_AlarmHandle(0);
//					RTC_AlarmCompare(0);
//        break;
//    
//        case 0x1C:
//					AlarmTimes.Alarm_Array[0].hour = value[3];
//					AlarmTimes.Alarm_Array[0].min = value[4];
//					AlarmTimes.Alarmnum.Alarm1 = 1;
//					RTC_AlarmHandle(AlarmTimes.Alarmnum.Alarm1);
//					RTC_AlarmCompare(AlarmTimes.Alarmnum.Alarm1);
//        break;
//        
//				case 0x37:
//					AlarmTimes.Alarm_Array[0].hour = value[3];
//					AlarmTimes.Alarm_Array[0].min = value[4];
//					AlarmTimes.Alarm_Array[1].hour = value[30];
//					AlarmTimes.Alarm_Array[1].min = value[31];
//					AlarmTimes.Alarmnum.Alarm1 = 1;
//					AlarmTimes.Alarmnum.Alarm2 = 1;
//					RTC_AlarmHandle(AlarmTimes.Alarmnum.Alarm1+AlarmTimes.Alarmnum.Alarm2);
//					RTC_AlarmCompare(AlarmTimes.Alarmnum.Alarm1+AlarmTimes.Alarmnum.Alarm2);
//        break;
//				
//        default:
//    
//        break;
//    }
    /*       */
    
    //There should be a report after processing the DP
    ret = mcu_dp_raw_update(DPID_CLOCK_SET,value,length);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_time_mode_handle
功能描述 : 针对DPID_TIME_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_time_mode_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char time_mode;
    
    time_mode = mcu_get_dp_download_enum(value,length);
    switch(time_mode) {
        case 0:
					HourFlag = 0;
        break;
        
        case 1:
					HourFlag = 1;
        break;
        
        default:
    
        break;
    }
    
    //There should be a report after processing the DP
    ret = mcu_dp_enum_update(DPID_TIME_MODE, time_mode);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_snooze_times_handle
功能描述 : 针对DPID_SNOOZE_TIMES的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_snooze_times_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long snooze_times;
    
    snooze_times = mcu_get_dp_download_value(value,length);
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_SNOOZE_TIMES,snooze_times);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_snooze_handle
功能描述 : 针对DPID_SNOOZE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_snooze_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为BOOL
    unsigned char ret;
    //0:off/1:on
    unsigned char snooze;
    
    snooze = mcu_get_dp_download_bool(value,length);
    if(snooze == 0) {
        //bool off
    }else {
        //bool on
    }
  
    //There should be a report after processing the DP
    ret = mcu_dp_bool_update(DPID_SNOOZE,snooze);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_snooze_time_handle
功能描述 : 针对DPID_SNOOZE_TIME的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_snooze_time_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long snooze_time;
    
    snooze_time = mcu_get_dp_download_value(value,length);
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_SNOOZE_TIME,snooze_time);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_backlight_switch_handle
功能描述 : 针对DPID_BACKLIGHT_SWITCH的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_backlight_switch_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为BOOL
    unsigned char ret;
    //0:off/1:on
    unsigned char backlight_switch;
    
    backlight_switch = mcu_get_dp_download_bool(value,length);
    if(backlight_switch == 0) {
        //bool off
    }else {
        //bool on
			P32 = 1;
    }
  
    //There should be a report after processing the DP
    ret = mcu_dp_bool_update(DPID_BACKLIGHT_SWITCH,backlight_switch);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_maxtemp_set_handle
功能描述 : 针对DPID_MAXTEMP_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_maxtemp_set_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long maxtemp_set;
    
    maxtemp_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_MAXTEMP_SET,maxtemp_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_minitemp_set_handle
功能描述 : 针对DPID_MINITEMP_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_minitemp_set_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long minitemp_set;
    
    minitemp_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_MINITEMP_SET,minitemp_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_maxhum_set_handle
功能描述 : 针对DPID_MAXHUM_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_maxhum_set_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long maxhum_set;
    
    maxhum_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_MAXHUM_SET,maxhum_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_minihum_set_handle
功能描述 : 针对DPID_MINIHUM_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_minihum_set_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long minihum_set;
    
    minihum_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_MINIHUM_SET,minihum_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_language_handle
功能描述 : 针对DPID_LANGUAGE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_language_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char language;
    
    language = mcu_get_dp_download_enum(value,length);
    switch(language) {
        case 0:
        break;
        
        case 1:
        break;
        
        default:
    
        break;
    }
    
    //There should be a report after processing the DP
    ret = mcu_dp_enum_update(DPID_LANGUAGE, language);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}




#ifdef SUPPORT_MCU_RTC_CHECK
/**
* @brief mcu check local RTC time 
* @param[in] {time} timestamp
* @return  void
*/
void mcu_write_rtctime(unsigned char time[])
{
 // #error "mcu should realize RTC time wtriting fuction, and delete this line"
  /*
  time[0]~time[3]：standard time
  time[4]~time[7]: Local time
 */
	my_memcpy((void *)timestamp,(const char *)time,4);	//get timestamp
	zigbee_timestamp_to_time();	
}
#endif


/**
* @brief Zigbee functional test feedback
* @param[in] {void} 
* @return  void
*/
void zigbee_test_result(void)
{
	//#error "this test is makesure the rf fuction of zigbee module, if test pass or not should do something, mcu should realize"
	unsigned char rssi = zigbee_uart_rx_buf[DATA_START+1];

	if(zigbee_uart_rx_buf[DATA_START]== 0x01){
		if(rssi > 0x3C)	{
			//test sucess the range of rssi is 0% ~ 100%
		}
		else{
			//test failure
		}
	}
	else{
		//test failure
	}
}

/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/**
* @brief this function will handle uart received frame data  
* @param[in] {dpid}   dp id
* @param[in] {value}  dp data 
* @param[in] {length} lenght of dp data 
* @return  handle result 
*/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /* only list of function, mcu need realize these fuction*/
  unsigned char ret;
//	Uart0_PutChar(length);
  switch(dpid){
          case DPID_TEMP_UNIT_CONVERT:
            //温标切换处理函数
            ret = dp_download_temp_unit_convert_handle(value,length);
        break;
        case DPID_CLOCK_SET:
            //闹钟设置处理函数
//						Uart0_PutChar(length);
//						Uart0_PutChar(0x21);
            ret = dp_download_clock_set_handle(value,length);
        break;
        case DPID_TIME_MODE:
            //时间制处理函数
            ret = dp_download_time_mode_handle(value,length);
        break;
        case DPID_SNOOZE_TIMES:
            //贪睡次数处理函数
            ret = dp_download_snooze_times_handle(value,length);
        break;
        case DPID_SNOOZE:
            //贪睡开关处理函数
            ret = dp_download_snooze_handle(value,length);
        break;
        case DPID_SNOOZE_TIME:
            //贪睡时间处理函数
            ret = dp_download_snooze_time_handle(value,length);
        break;
        case DPID_BACKLIGHT_SWITCH:
            //背光开关处理函数
            ret = dp_download_backlight_switch_handle(value,length);
        break;
        case DPID_MAXTEMP_SET:
            //温度上限设置处理函数
            ret = dp_download_maxtemp_set_handle(value,length);
        break;
        case DPID_MINITEMP_SET:
            //温度下限设置处理函数
            ret = dp_download_minitemp_set_handle(value,length);
        break;
        case DPID_MAXHUM_SET:
            //湿度上限设置处理函数
            ret = dp_download_maxhum_set_handle(value,length);
        break;
        case DPID_MINIHUM_SET:
            //湿度下限设置处理函数
            ret = dp_download_minihum_set_handle(value,length);
        break;
        case DPID_LANGUAGE:
            //语言处理函数
            ret = dp_download_language_handle(value,length);
        break;

  
  default:
    break;
  }
  return ret;
}

/**
* @brief get received cmd total number
* @param[in] {void}   
* @return  received cmd total number
*/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}


/**
* @brief received zigbee net_work state handle 
* @param[in] {zigbee_work_state}  zigbee current network state
* @return  void 
*/
uint8_t abc = 0;
void zigbee_work_state_event(unsigned char zigbee_work_state)
{	
	unsigned short length= 0;
	zigbee_uart_write_frame(ZIGBEE_STATE_CMD, length);	

	switch(zigbee_work_state){
		case ZIGBEE_NOT_JION:	
				ZigbeeState = zigbee_work_state;
			break;
		
		case ZIGBEE_JOIN_GATEWAY:	
//			mcu_get_system_time();
			ZigbeeState = zigbee_work_state;
			break;
		
		case ZIGBEE_JOIN_ERROR:	
			ZigbeeState = zigbee_work_state;
			break;
		
		case ZIGBEE_JOINING:	
			ZigbeeState = zigbee_work_state;
			break;
		
		default:
			break;
	}
}


/**
* @brief received reset zigbee response 
* @param[in] {state} response state 
* @return  void 
*/
void mcu_reset_zigbee_event(unsigned char state)
{	
	switch(state){
		case RESET_ZIGBEE_OK:
		
			break;
		
		case RESET_ZIGBEE_ERROR:
		
			break;
		
		default:
			break;
	}
}


/**
* @brief check mcu version response
* @param[in] {void}
* @return  void 
*/
void response_mcu_ota_version_event(void)
{
	unsigned short length = 0;
	length = set_zigbee_uart_byte(length,get_current_mcu_fw_ver());	//current fw version
	zigbee_uart_write_frame(MCU_OTA_VERSION_CMD,length);
}

#ifdef SUPPORT_MCU_OTA
/**
* @brief mcu ota update notify response
* @param[in] {offset} data offset 
* @return  void 
*/
void response_mcu_ota_notify_event(unsigned char offset)
{
	unsigned char i = 0;
	unsigned short length = 0;
	
	current_mcu_fw_pid();	//current PID
	
	while(i<8){
		ota_fw_info.mcu_ota_pid[i] = zigbee_uart_rx_buf[offset + DATA_START + i];								//ota fw PID
		i++;
	}
	ota_fw_info.mcu_ota_ver = zigbee_uart_rx_buf[offset + DATA_START + 8];											//ota fw version
	ota_fw_info.mcu_ota_fw_size = zigbee_uart_rx_buf[offset + DATA_START + 9] << 24 | \
																zigbee_uart_rx_buf[offset +DATA_START + 10] << 16 | \
																zigbee_uart_rx_buf[offset + DATA_START + 11] << 8 | \
																zigbee_uart_rx_buf[offset + DATA_START + 12];								//ota fw size
	ota_fw_info.mcu_ota_checksum = zigbee_uart_rx_buf[offset + DATA_START + 13] << 24 | \
																 zigbee_uart_rx_buf[offset + DATA_START + 14] << 16 | \
																 zigbee_uart_rx_buf[offset + DATA_START + 15] << 8 | \
																 zigbee_uart_rx_buf[offset + DATA_START + 16];								//ota fw checksum
	
	if((!strcmp_barry(&ota_fw_info.mcu_ota_pid[0],&current_mcu_pid[0],8)) && \
		 (ota_fw_info.mcu_ota_ver > get_current_mcu_fw_ver() &&\
		  ota_fw_info.mcu_ota_fw_size > 0)	
		){		//check fw pid and fw version and fw size
		length = set_zigbee_uart_byte(length,0x00);	//OK
	}
	else{
		length = set_zigbee_uart_byte(length,0x01);	//error
	}
    ota_fw_info.mcu_current_offset = 0;
	zigbee_uart_write_frame(MCU_OTA_NOTIFY_CMD,length);
}


/**
* @brief received mcu ota data request response
* @param[in] {fw_offset}  offset of file 
* @param[in] {data}  received data  
* @return  void 
*/
void reveived_mcu_ota_data_handle(unsigned int fw_offset,char *data)
{
	#error "received frame data, should save in flash, mcu should realize this fuction, and delete this line "
}

/**
* @brief mcu send ota data request 
* @param[in] {void}  
* @return  void 
*/
void mcu_ota_fw_request_event(unsigned char offset)
{	
	unsigned int fw_offset = 0;
	char fw_data[FW_SINGLE_PACKET_SIZE] = {-1};	//
	unsigned char i = 0;
	unsigned char last_package_len = 0;
	static unsigned int current_checksum = 0;

	if(zigbee_uart_rx_buf[offset + DATA_START] == 0x01)				//status check
		return;
	while(i < 8){
		if(current_mcu_pid[i] != zigbee_uart_rx_buf[offset + DATA_START + 1 + i])	//pid check
			return;
		i++;
	}
	if(ota_fw_info.mcu_ota_ver != zigbee_uart_rx_buf[offset + DATA_START + 9]) //version check
		return;
	
	i = 0;
	while(i < 4){
		fw_offset |= (zigbee_uart_rx_buf[offset + DATA_START + 10 + i] << (24 - i * 8));		//offset
		i++;
	}
	i = 0;
	if(ota_fw_info.mcu_current_offset ==  fw_offset)
	{
		if((ota_fw_info.mcu_ota_fw_size - fw_offset) / FW_SINGLE_PACKET_SIZE != 0){
			if(ota_fw_info.mcu_current_offset == 0){
				current_checksum = 0;
			}
			while(i < FW_SINGLE_PACKET_SIZE){
				fw_data[i] = zigbee_uart_rx_buf[offset + DATA_START + 14 + i];   //fw data
				current_checksum += fw_data[i];
				i++;
			}
			ota_fw_info.mcu_current_offset += FW_SINGLE_PACKET_SIZE;
			ota_fw_data_handle(fw_offset,&fw_data[0],FW_SINGLE_PACKET_SIZE);	//OTA paket data handle
		}
		else {
			i = 0;
			last_package_len = ota_fw_info.mcu_ota_fw_size - fw_offset;
			while(i < last_package_len){
				fw_data[i] = zigbee_uart_rx_buf[offset + DATA_START + 14 + i];
				current_checksum += fw_data[i];
				i++;
			}
			ota_fw_info.mcu_current_offset += last_package_len;
			if(ota_fw_info.mcu_ota_checksum != current_checksum){
				//ota failure report ota failure and clear ota struct 
				mcu_ota_result_report(1); // failed
			}	
			else{
				//ota sucess 
				mcu_ota_result_report(0); // seccess
			}
			ota_fw_data_handle(fw_offset,&fw_data[0],last_package_len);	//OTA paket data handle		
			current_checksum = 0;															
		}
	}
	else
	{
		// ota request timeout, then restart ota request from  ota_fw_info.mcu_ota_fw_size
	}
}

static void report_mcu_ota_result(unsigned char  res)
{
	unsigned short length;
	if((res==0)||(res == 1))
	{
		length = set_zigbee_uart_byte(length,res);	
		zigbee_uart_write_frame(MCU_OTA_NOTIFY_CMD,length);
	}
}


/**
* @brief mcu ota data result notify
* @param[in] {void} 
* @return  void 
*/
void mcu_ota_result_event(unsigned char offset)
{
	unsigned char status = zigbee_uart_rx_buf[offset + DATA_START];
	
	if(status == 0x00){
	}
	else if(status == 0x01)	{

	}
}

/**
* @brief mcu ota data handle 
* @param[in] {fw_offset} frame offset 
* @param[in] {data} received data
* @return  void 
*/
void ota_fw_data_handle(unsigned int fw_offset,char *data,unsigned char data_len)
{
	//#error "请在该函数处理固件包数据,并删除该行"
}
#endif


#ifdef  BEACON_TEST
/**
* @brief beacon test notify,which used in testing
* @param[in] {void} 
* @return  void 
*/
void mcu_received_beacon_test_handle(void)
{
	unsigned short length = 0;
	length = set_zigbee_uart_byte(length,0x00);	
	zigbee_uart_write_frame(SEND_BEACON_NOTIFY_CMD,length);
	// then start test
}
#endif

#ifdef  CHECK_ZIGBEE_NETWORK
/**
* @brief check zigbee nwkstate,before start join nwk, check nwk if state is not 0x02,can start jion,
* else delay 5s 
*
* @param[in] {void} 
* @return  void 
*/
void mcu_check_zigbee_nwk_state(void)
{
	unsigned short length = 0;
	zigbee_uart_write_frame(CHECK_ZIGBEE_NETWORK_CMD,length);
	// then start test
}
#endif

void zigbee_notify_factory_new_hanlde(void)
{
	unsigned short length = 0;
	length = set_zigbee_uart_byte(length,0x01);	
	zigbee_uart_write_frame(ZIGBEE_FACTORY_NEW_CMD,length);
	// then start test
}


#ifdef READ_DP_DATA_NOTIFY
/**
* @brief when gateway repower or relink clould, or zigbee module join sucess, repower, this commod will notify 
* mcu, to sys dp data, mcu itself decide whether report.
*
* @param[in] {void} 
* @return  void 
*/
void read_dp_data_notify_hanlde(void)
{
	unsigned short length = 0;
	length = set_zigbee_uart_byte(length,0x01);	
	zigbee_uart_write_frame(ZIGBEE_FACTORY_NEW_CMD,length);
	// then start test
}
#endif 



