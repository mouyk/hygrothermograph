#ifndef CA51F_CONFIG_H
#define CA51F_CONFIG_H
/**************************************************************************************************************/
#define IRCH		0
#define IRCL		1
#define PLL			2
#define XOSCL		3

/**************************************************************************************************************/
#define SYSCLK_SRC			IRCH			//芯片系统时钟选择
/**************************************************************************************************************/


/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

/************系统时钟频率定义，主要用于配置UART波特率**********************************************************/
#if (SYSCLK_SRC == IRCH)
	#define FOSC		(3686400)
#elif (SYSCLK_SRC == PLL)
	#define PLL_Multiple			6			//PLL倍频倍数
	#define FOSC		(3686400*PLL_Multiple)
#else
	#define FOSC		(3686400)
#endif
/***************************************************************************************************************/
#define LVD_RST_ENABLE

/*************************UART功能开关宏定义********************************************************************/
#define UART0_EN			//如果使用UART0，打开此宏定义
//#define UART1_EN			//如果使用UART1，打开此宏定义
//#define UART2_EN			//如果使用UART2，打开此宏定义

#define PRINT_EN				//使用uart_printf函数打印使能

#ifdef PRINT_EN
	#define UART0_PRINT		//如果使用UART0打印，打开此宏定义
// 	#define UART1_PRINT		//如果使用UART1打印，打开此宏定义
// 	#define UART2_PRINT		//如果使用UART1打印，打开此宏定义

	#ifdef UART0_PRINT
		#define UART0_EN
	#elif defined  UART1_PRINT
		#define UART1_EN
	#elif defined  UART2_PRINT
		#define UART2_EN
	#endif
#endif
#ifdef UART0_EN
	#define UART0_BAUTRATE		115200
#endif
#ifdef UART1_EN
	#define UART1_BAUTRATE		115200
#endif
#ifdef UART2_EN
	#define UART2_BAUTRATE		115200
#endif
/*********************************************************************************************************************/


#endif										
