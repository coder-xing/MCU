
#include <msp430x16x.h>

#include "TFT2.8.h"                      //TFTͷ�ļ�
                     //TFT������ʼ��������

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

//**************�����ⲿ�����ͱ���********************

extern void delayms(unsigned int count);


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
                      ������
********************************************************************/
void main()
{   unsigned char i,j;
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    Clock_Init();                      //ϵͳʱ������
    Port_Init();                       //ϵͳ��ʼ��������IO������
    Device_code=0x9320;                //TFT����IC�ͺ�
    TFT_Initial();                     //��ʼ��LCD
    CLR_Screen(Black);               //�ñ���ɫ����
    while(1)                           //ѭ��ִ�г���
    {   for(i=0;i<200;i++)
            for(j=0;j<5;j++)
                Put_pixel(20+i,100+j,Red);
        for(i=0;i<100;i++)
            for(j=0;j<5;j++)
                Put_pixel(20+j,100+i,Red);
        LCD_PutString24(60,140,"�����Ұ���",Yellow, Black); //��ӭ���棬24x24��С���֣���ģ�������С����
        //    Put16x16(10,10,"",Blue,Green);
        LCD_PutString(24,16,"123��о",White,Blue);
    }

}
