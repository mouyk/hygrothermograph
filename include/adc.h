#ifndef _ADC_H_
#define _ADC_H_

//ADCON����
#define AST(N)		(N<<7)					//ADC���� �� AST=0����
#define ADIE(N)		(N<<6)					//�ж�ʹ��
#define ADIF		(1<<5)					//�жϱ�־
#define HTME(N) 	(N<<2)					//N=0-7			 //����ʱ������, ʱ��Ϊ2��HTME�η���ʱ������
#define VSEL(N) 	(N)						//N=0-3			 //ѡ��ο���ѹ 0-�ڲ� 1-VDD 2-�ⲿ

//ADCFGL����													   
#define ACKD(N)		(N<<5)  				//N=0-7			 //ADCʱ�ӷ�Ƶ	  ��Ƶ����=��ACKD+1)
#define ADCHS(N)	(N) 					//N=0-15		 //ADCͨ��ѡ�� �� 1-13��Ӧͨ��0-12


//ADCFGH����
#define AOVF		(1<<7)					//ADC���ݳ����趨��Χ�жϱ�־
#define AOVE(N)		(N<<6)					//ADC���ݳ����趨��Χ�ж�ʹ��


#define VTRIM(N)	(N)						//0-63	   У���ڲ��ο���ѹ


enum 
{
	ADC_REF_INNER  	= 0,
	ADC_REF_VDD  	= 1,
	ADC_REF_Outer  	= 2
};

enum 
{
	ADC_CH0  	= 1,
	ADC_CH1  	= 2,
	ADC_CH2  	= 3,
	ADC_CH3  	= 4,
	ADC_CH4  	= 5,
	ADC_CH5  	= 6,
	ADC_CH6  	= 7,
	ADC_CH7  	= 8,
	ADC_VDD  	= 9,
};
#endif