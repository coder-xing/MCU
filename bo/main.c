#include <msp430x16x.h>
#include "GUI.h"
#include "TFT2.8.h"
#include "Touch.h"
#define uchar unsigned char
#define uint unsigned int
unsigned int LCD_X,LCD_Y;

//**************声明外部函数和变量********************

extern void delayms(unsigned int count);

/********************************************************************
            初始化IO口子程序
********************************************************************/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;
  P3SEL=0x00;                    //设置IO口为普通I/O模式
  P3DIR = 0xff;                  //设置IO口方向为输出
  P3OUT = 0x00;                  //初始设置为00,LED灯所在IO口
  P2SEL = 0x00;
  P2DIR |= BIT3 + BIT4 + BIT5 + BIT6;               //触摸控制线
}

/********************************************************************
    LED闪烁一次，可在需要查看程序执行位置时灵活设置，类似断点
********************************************************************/
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}

void key_show(unsigned char x1,unsigned char y1,unsigned char * str)
{
    GUIfull(x1,y1,x1+60,y1+60,Cyan);
    LCD_PutString(x1+26,y1+22,str,Black,Cyan);  //按钮值显示
}
void touch_screen()
{
    if(Getpix()==1)
    {
        if((LCD_X>=15)&&(LCD_X<15+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                CLR_Screen(White);               //用背景色清屏
                start_show();
            }
        }
        else if((LCD_X>=90)&&(LCD_X<90+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                CLR_Screen(White);               //用背景色清屏
                start_show();
            }

        }
        else if((LCD_X>=165)&&(LCD_X<165+60))
        {   if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                CLR_Screen(White);               //用背景色清屏
                start_show();
            }

        }
    }
}
/********************************************************************
                      主函数
********************************************************************/
void main()
{  // unsigned char i,j;
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    Clock_Init();                      //系统时钟设置
    Port_Init();                       //系统初始化，设置IO口属性
    Device_code=0x9320;                //TFT控制IC型号
    TFT_Initial();                     //初始化LCD
    start_7843();
    CLR_Screen(White);               //用背景色清屏
    start_show();
    delay_ms(2000);
    CLR_Screen(White);               //用背景色清屏
    main_menu();
    key_show(15,200,"1");
    key_show(90,200,"2");
    key_show(165,200,"3");
    while(1)                           //循环执行程序
    {
        touch_screen();
    }
}
