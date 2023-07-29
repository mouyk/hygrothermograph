#ifndef _DISP_C_
#define _DISP_C_

#include "stdlib.h"
#include "stdio.h"
//#include "main.h"
#include "string.h"
#include "Disp.h"
#include "include/ca51f_config.h"	

uint8_t lcd_ram[34] = {0};

#define LCD_DRV	1

#ifdef LCD_DRV
/* 
#define seg_a                       0x01
#define seg_b                       0x20
#define seg_c                       0x80
#define seg_d                       0x08
#define seg_e                       0x04
#define seg_f                       0x02
#define seg_g                       0x40

static uint8_t            			lcd_ram[34]               = {0};


static uint8_t            DispLcdTableNum[]        = {
  seg_a+seg_b+seg_c+seg_d+seg_e+seg_f,              //0 
  seg_b+seg_c,                                      //1
  seg_a+seg_b+seg_g+seg_e+seg_d,                    //2
  seg_a+seg_b+seg_c+seg_d+seg_g,                    //3
  seg_f+seg_g+seg_b+seg_c,                          //4
  seg_a+seg_f+seg_g+seg_c+seg_d,                    //5
  seg_a+seg_f+seg_e+seg_d+seg_c+seg_g,              //6
  seg_a+seg_b+seg_c,                                //7
  seg_a+seg_b+seg_c+seg_d+seg_e+seg_f+seg_g,        //8
  seg_a+seg_b+seg_c+seg_d+seg_f+seg_g,              //9
};
 */
 /*万年历湿度数码管*/
const  uint8_t            DispLcdTableNum[10]        = {
  0xaf,		   //0 
  0xa0,        //1
  0x6d,        //2
  0xe9,        //3
  0xe2,        //4
  0xcb,        //5
  0xcf,        //6
  0xa1,        //7
  0xef,        //8
  0xeb,        //9
};

/*
#define seg_a                       0x80
#define seg_b                       0x08
#define seg_c                       0x02
#define seg_d                       0x10
#define seg_e                       0x20
#define seg_f                       0x40
#define seg_g                       0x04

static uint8_t            DispLcdTableNumTemp[]        = {
  seg_a+seg_b+seg_c+seg_d+seg_e+seg_f,              //0 
  seg_b+seg_c,                                      //1
  seg_a+seg_b+seg_g+seg_e+seg_d,                    //2
  seg_a+seg_b+seg_c+seg_d+seg_g,                    //3
  seg_f+seg_g+seg_b+seg_c,                          //4
  seg_a+seg_f+seg_g+seg_c+seg_d,                    //5
  seg_a+seg_f+seg_e+seg_d+seg_c+seg_g,              //6
  seg_a+seg_b+seg_c,                                //7
  seg_a+seg_b+seg_c+seg_d+seg_e+seg_f+seg_g,        //8
  seg_a+seg_b+seg_c+seg_d+seg_f+seg_g,              //9
};
*/


/*温度特殊数码管*/
const uint8_t            DispLcdTableNumTemp[12]        = {
  0xfa,		   //0 
  0x0a,        //1
  0xbc,        //2
  0x9e,        //3
  0x4e,        //4
  0xd6,        //5
  0xf6,        //6
  0x8a,        //7
  0xfe,        //8
  0xde,        //9
	0x70,				 //L
	0x6e,				 //H
};


/*****************************************************************************
 ** \brief	 UpdateNixieTubeRAM1
			 UpdateNixieTubeRAM2
			 将数据转换成数码管缓存数据
			// SEG1 INDEX 1 时 十位		addr=1
			// SEG2 INDEX 2 时 十位
			// SEG3 INDEX 3 时 个位		addr=3
			// SEG3 INDEX 4 时 个位
 ** \param   [in] none       
 ** \return   none
*****************************************************************************/
void UpdateNixieTubeRAMA(char dataRAM,char addr)
{
      lcd_ram[addr]                                 =0x0f&DispLcdTableNum[dataRAM];
      lcd_ram[addr+1]                               =0x0f&DispLcdTableNum[dataRAM]>>4;
}
void UpdateNixieTubeRAMB(char dataRAM,char addr)
{	
      lcd_ram[addr]                                 =0x0f&DispLcdTableNumTemp[dataRAM];
      lcd_ram[addr+1]                               =0x0f&DispLcdTableNumTemp[dataRAM]>>4;
}


#endif

