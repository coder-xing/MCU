#include <msp430f169.h>
#include "TFT2.8.h"
//******************ȫ�ֱ���***************************

#define White          0xFFFF           //��ʾ��ɫ����
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned int Device_code;               //TFT����IC�ͺţ�2.8��ΪILI9320
extern void delayms(unsigned int count);

void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}

/********************************************************************
            ��ʼ��IO���ӳ���
********************************************************************/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;


  P3SEL=0x00;                    //����IO��Ϊ��ͨI/Oģʽ
  P3DIR = 0xff;                  //����IO�ڷ���Ϊ���
  P3OUT = 0x00;                  //��ʼ����Ϊ00,LED������IO��
}
/********************************************************************
    LED��˸һ�Σ�������Ҫ�鿴����ִ��λ��ʱ������ã����ƶϵ�
********************************************************************/
void LED_Light()
{
    LED8=0x00;                    //����LED
    delay_ms(500);
    LED8=0xff;                    //Ϩ��LED
    delay_ms(500);
}

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	uint i,y;
	Clock_Init();                      //ϵͳʱ������
	Port_Init();                       //ϵͳ��ʼ��������IO������
	LED_Light();                       //LED����һ�Σ���������˵����Ƭ����ʼ���ɹ�
	Device_code=0x9320;                //TFT����IC�ͺ�
	TFT_Initial();                     //��ʼ��LCD
	CLR_Screen(Black );
	while(1)
	{
	    Show_Image(20,80,20,100);
//	    for(i=0;i<320;i++){ Put_pixel(68,i,Blue);}                   //��5��������ʵ�ֻ���
//	    for(i=200;i>50;i--){ Put_pixel(40,i,Red );}
//	    for(y=0;y<200;y++)
//	    for(i=200;i>50;i--){ Put_pixel(i,i+y,Red );}
//	    delay_ms(1000);//��5��������ʵ�ֻ���
//	    CLR_Screen(Black );
	  //      CLR_Screen(Red);
	}



}
