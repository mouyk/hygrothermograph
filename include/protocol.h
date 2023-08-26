/**
* @file  protocol.h
* @brief declaration of fuction in  protocol.c
* @author qinlang
* @date 2022.05.06
* @par email:
* @par email:qinlang.chen@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_


#ifdef __cplusplus
extern "C"
{
#endif

///< product INFORMATION

#define PRODUCT_KEY "cztzezpg"    //开发平台创建产品后生成的16位字符产品唯一标识


///< mcu version 
#define MCU_VER "1.0.0"     // MAX 3.3.15   BIT 7~0  XX.XX.XXXX                  
 
#define ZIGBEE_UART_QUEUE_LMT             256             // using to save data received from uart
#define ZIGBEE_UART_RECV_BUF_LMT          128             //
#define ZIGBEE_UART_SEND_BUF_LMT          128             //


typedef enum
{
    MCU_TYPE_DC_POWER = 1,
    MCU_TYPE_LOWER_POWER,
    MCU_TYPE_SCENE
}MCU_TYPE_E;

/**
 * if mcu need to support the time function, this macro definition should be opened
 * and need complete mcu_write_rtctime function 
 * 
 */
 
#define    SUPPORT_MCU_RTC_CHECK             //start time calibration

/**
 * if mcu need to support OTA, this macro definition should be opened
 */
//#define    SUPPORT_MCU_OTA                  //support mcu ota


/**
 * if mcu need to support mcu type checking, this macro definition should be opened
 * 
 */
//#define    CHECK_MCU_TYPE               //support mcu type check 


/**
 * if mcu need to support zigbee network parameter setting, this macro definition should be opened
 * 
 */
//#define  SET_ZIGBEE_NWK_PARAMETER        //support zigbee nwk parameter setting 


/**
 * if mcu need to send a broadcast data, this macro definition should be opened
 * 
 */
//#define  BROADCAST_DATA_SEND           //support broadcast data sending



/**
 * DP data list,this code will be generate by cloud platforms
 */

//当前温度(只上报)
//备注:
#define DPID_TEMP_CURRENT 1
//湿度数值(只上报)
//备注:
#define DPID_HUMIDITY_VALUE 2
//电池电量状态(只上报)
//备注:
#define DPID_BATTERY_STATE 3
//电池电量(只上报)
//备注:
#define DPID_BATTERY_PERCENTAGE 4
//充电状态(只上报)
//备注:
#define DPID_CHARGE_STATE 8
//温标切换(可下发可上报)
//备注:
#define DPID_TEMP_UNIT_CONVERT 9
//闹钟设置(可下发可上报)
//备注:闹钟信息：名称：
//闹钟设置：组+开/关+周一-周天+时+分
#define DPID_CLOCK_SET 14
//时间制(可下发可上报)
//备注:
#define DPID_TIME_MODE 18
//贪睡次数(可下发可上报)
//备注:
#define DPID_SNOOZE_TIMES 23
//贪睡开关(可下发可上报)
//备注:
#define DPID_SNOOZE 24
//贪睡时间(可下发可上报)
//备注:
#define DPID_SNOOZE_TIME 25
//背光开关(可下发可上报)
//备注:
#define DPID_BACKLIGHT_SWITCH 27
//温度上限设置(可下发可上报)
//备注:
#define DPID_MAXTEMP_SET 81
//温度下限设置(可下发可上报)
//备注:
#define DPID_MINITEMP_SET 82
//湿度上限设置(可下发可上报)
//备注:
#define DPID_MAXHUM_SET 83
//湿度下限设置(可下发可上报)
//备注:
#define DPID_MINIHUM_SET 84
//语言(可下发可上报)
//备注:
#define DPID_LANGUAGE 97



/**
* @brief encapsulates a generic send function, developer should use their own function to completing this fuction 
* @param[in] {value} send signle data 
* @return  void
*/
void uart_transmit_output(unsigned char value);

/**
* @brief Upload all dp information of the system, and realize the synchronization of APP and muc data
* @param[in] {void}
* @return  void
*/
void all_data_update(void);

/**
* @brief mcu check local RTC time 
* @param[in] {time} timestamp
* @return  void
*/
void mcu_write_rtctime(unsigned char time[]);

/**
* @brief Zigbee functional test feedback
* @param[in] {void} 
* @return  void
*/
void zigbee_test_result(void);

/**
* @brief this function will handle uart received frame data  
* @param[in] {dpid}   dp id
* @param[in] {value}  dp data 
* @param[in] {length} lenght of dp data 
* @return  handle result 
*/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);

/**
* @brief get received cmd total number
* @param[in] {void}   
* @return  received cmd total number
*/
unsigned char get_download_cmd_total(void);

/**
* @brief received zigbee net_work state handle 
* @param[in] {zigbee_work_state}  zigbee current network state
* @return  void 
*/
void zigbee_work_state_event(unsigned char zigbee_work_state);
/**
* @brief received reset zigbee response 
* @param[in] {state} response state 
* @return  void 
*/
void mcu_reset_zigbee_event(unsigned char state);

/**
* @brief check mcu version response
* @param[in] {void}
* @return  void 
*/
void response_mcu_ota_version_event(void);


#ifdef SUPPORT_MCU_OTA 
/**
* @brief mcu ota update notify response
* @param[in] {offset} offset of file 
* @return  void 
*/
void response_mcu_ota_notify_event(unsigned char offset);
/**
* @brief received mcu ota data request response
* @param[in] {fw_offset}  offset of file 
* @param[in] {data}  received data  
* @return  void 
*/
void reveived_mcu_ota_data_handle(unsigned int fw_offset,char *data);

/**
* @brief mcu send ota data request 
* @param[in] {offset} offset of file 
* @return  void 
*/
void mcu_ota_fw_request_event(unsigned char offset);

/**
* @brief mcu ota data result notify
* @param[in] {offset} offset of file 
* @return  void 
*/
void mcu_ota_result_event(unsigned char offset);


/**
* @brief mcu ota data handle 
* @param[in] {fw_offset} frame offset 
* @param[in] {data} received data
* @return  void 
*/
void ota_fw_data_handle(unsigned int fw_offset,char *data,unsigned char data_len);
#endif


#ifdef  BEACON_TEST
/**
* @brief beacon test notify,which used in testing
* @param[in] {void} 
* @return  void 
*/
void mcu_received_beacon_test_handle(void);
#endif

#ifdef  CHECK_ZIGBEE_NETWORK
/**
* @brief check zigbee nwkstate,before start join nwk, check nwk if state is not 0x02,can start jion,
* else delay 5s 
*
* @param[in] {void} 
* @return  void 
*/
void mcu_check_zigbee_nwk_state(void);
#endif

#ifdef READ_DP_DATA_NOTIFY
/**
* @brief when gateway repower or relink clould, or zigbee module join sucess, repower, this commod will notify 
* mcu, to sys dp data, mcu itself decide whether report.
*
* @param[in] {void} 
* @return  void 
*/
void read_dp_data_notify_hanlde(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
