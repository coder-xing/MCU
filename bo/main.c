#include <msp430x16x.h>
#include "GUI.h"
#include "TFT2.8.h"
#include "Touch.h"
#define uchar unsigned char
#define uint unsigned int
unsigned int LCD_X,LCD_Y;

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
  P2SEL = 0x00;
  P2DIR |= BIT3 + BIT4 + BIT5 + BIT6;               //����������
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

void key_show(unsigned char x1,unsigned char y1,unsigned char * str)
{
    GUIfull(x1,y1,x1+60,y1+60,Cyan);
    LCD_PutString(x1+26,y1+22,str,Black,Cyan);  //��ťֵ��ʾ
}
void touch_screen()
{
    if(Getpix()==1)
    {
        if((LCD_X>=15)&&(LCD_X<15+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                CLR_Screen(White);               //�ñ���ɫ����
                start_show();
            }
        }
        else if((LCD_X>=90)&&(LCD_X<90+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                CLR_Screen(White);               //�ñ���ɫ����
                start_show();
            }

        }
        else if((LCD_X>=165)&&(LCD_X<165+60))
        {   if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                CLR_Screen(White);               //�ñ���ɫ����
                start_show();
            }

        }
    }
}
/********************************************************************
                      ������
********************************************************************/
void main()
{  // unsigned char i,j;
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    Clock_Init();                      //ϵͳʱ������
    Port_Init();                       //ϵͳ��ʼ��������IO������
    Device_code=0x9320;                //TFT����IC�ͺ�
    TFT_Initial();                     //��ʼ��LCD
    start_7843();
    CLR_Screen(White);               //�ñ���ɫ����
    start_show();
    delay_ms(2000);
    CLR_Screen(White);               //�ñ���ɫ����
    main_menu();
    key_show(15,200,"1");
    key_show(90,200,"2");
    key_show(165,200,"3");
    while(1)                           //ѭ��ִ�г���
    {
        touch_screen();
    }
}
