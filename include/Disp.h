
#ifndef __DISP_H__
#define __DISP_H__

/*******************************************************************************
* 文 件 名         : DISP
* 功    能		   : LCD SEG COM配置表	   

*******************************************************************************/
// SEG0 INDEX 0
#define Data_AM                     0x04            //上 午
#define Data_PM                     0x08            //下 午
// SEG1 INDEX 1 时 十位
// SEG2 INDEX 2 时 十位
// SEG3 INDEX 3 时 个位
// SEG3 INDEX 4 时 个位
#define LoGo_Zigbee                 0x01            //zigbee logo  INDEX 2
#define colon                       0x01            //冒号标识     INDEX 4
// SEG5 INDEX 5 分 十位
// SEG6 INDEX 6 分 十位
// SEG7 INDEX 7 分 个位
// SEG8 INDEX 8 分 个位
#define LoGo_Bluetooth              0x01            //zigbee logo  INDEX 6
#define friday              		0x01            //星期5		   INDEX 8
// SEG9  INDEX9  年 十位
// SEG10 INDEX10 年 十位
// SEG11 INDEX11 年 个位
// SEG12 INDEX12 年 个位
#define year20              		0x01            //20年		   INDEX 10
#define month0              		0x01            //月十位0	   INDEX 12
// SEG13 INDEX13 月 个位
// SEG14 INDEX14 月 个位
#define month1              		0x01            //月十位1	   INDEX 14
// SEG15 INDEX15 日 十位  3:0X0E  2:0x07  1:0xC0 0:0x00
// SEG16 INDEX16 日 个位
// SEG17 INDEX17 日 个位
#define thursday              		0x01            //星期4		   INDEX 17
// SEG18 INDEX18 星期3217
#define wednesday              		0x01            //星期3		   INDEX 18
#define Tuesday              		0x02            //星期2		   INDEX 18
#define Monday              		0x04            //星期1		   INDEX 18
#define Sunday              		0x08            //星期7		   INDEX 18
// SEG19 INDEX19 湿度 十位
// SEG20 INDEX20 湿度 十位
// SEG21 INDEX21 湿度 个位
// SEG22 INDEX22 湿度 个位			//0x01  百分号常亮
#define smile_Humi              	0x01            //笑脸湿度	   INDEX 20

// SEG23 INDEX23 温度 小数位		//0x01  温湿度标志常亮
// SEG24 INDEX24 温度 小数位
// SEG25 INDEX25 温度 个位			//0x01	点常亮  
// SEG26 INDEX26 温度 个位
// SEG27 INDEX27 温度 十位			 
// SEG28 INDEX28 温度 十位
#define Saturday              		0x01            //星期6		   INDEX 27

// SEG29 INDEX29 			
#define LoGo_WiFi              		0x01            //WiFi LoGo	   INDEX 29
#define smile_Temp              	0x02            //笑脸温度	   INDEX 29
#define negative              		0x04            //负号-		   INDEX 29
#define temp100              		0x08            //温度100	   INDEX 29

// SEG30 INDEX30 		
#define LoGo_timer              	0x01            //计时 LoGo	   INDEX 30
#define C_Tmp              			0x0C            //摄氏度C	   INDEX 30
#define F_Tmp              			0x06            //华氏度F      INDEX 30

// SEG31 INDEX31 闹铃无 123
#define alarm_timer1              	0x0C            //闹铃1	   	   INDEX 31
#define alarm_timer2              	0x0B            //闹铃2	   	   INDEX 31
#define alarm_timer3              	0x0E            //闹铃3	   	   INDEX 31

// SEG32 INDEX32 电量0123
#define SOC0              			0x01            //电量0格	   INDEX 32
#define SOC1              			0x09            //电量1格	   INDEX 32
#define SOC2              			0x0D            //电量2格	   INDEX 32
#define SOC3              			0x0F            //电量3格	   INDEX 32

// SEG33 INDEX33 闹铃logo
#define alarm_timer              	0x01            //闹铃 LoGo	   INDEX 33

void UpdateNixieTubeRAMA(char dataRAM,char addr);

void UpdateNixieTubeRAMB(char dataRAM,char addr);





#endif



