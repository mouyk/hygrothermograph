//PWMCON
#define TIE(N)			(N<<7)
#define ZIE(N)			(N<<6)
#define PIE(N)			(N<<5)
#define NIE(N)			(N<<4)
#define MS(N)				(N<<3)		//中心对齐模式
#define MOD(N)			(N<<0)	    //互补模式

#define CKS_SYS			(0<<0)
#define CKS_IH			(1<<0)
#define CKS_IL			(2<<0)
#define CKS_XH			(3<<0)
#define CKS_XL			(4<<0)
#define CKS_PLL			(5<<0)
#define CKS_TF			(6<<0)


//PWMCFG
#define TOG(N)			(N<<7)


//PWMAIF  PWMBIF	PWMCIF 
#define  TIF1	(1<<7)
#define  ZIF1	(1<<6)
#define  PIF1	(1<<5)
#define  NIF1	(1<<4)
#define  TIF0	(1<<3)
#define  ZIF0	(1<<2)
#define  PIF0	(1<<1)
#define  NIF0	(1<<0)	

#define PWM_CH0		0
#define PWM_CH1		1
#define PWM_CH2		2
#define PWM_CH3		3
#define PWM_CH4		4
#define PWM_CH5		5
#define PWM_CH6		6
#define PWM_CH7		7
