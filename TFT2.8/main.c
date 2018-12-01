
#include <msp430x16x.h>

#include "TFT2.8.h"                      //TFT头文件
                     //TFT操作初始化及函数

//******************全局变量***************************

#define White          0xFFFF           //显示颜色代码
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned int Device_code;               //TFT控制IC型号，2.8寸为ILI9320

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
/********************************************************************
                      主函数
********************************************************************/
void main()
{   unsigned char i,j;
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    Clock_Init();                      //系统时钟设置
    Port_Init();                       //系统初始化，设置IO口属性
    Device_code=0x9320;                //TFT控制IC型号
    TFT_Initial();                     //初始化LCD
    CLR_Screen(Black);               //用背景色清屏
    while(1)                           //循环执行程序
    {   for(i=0;i<200;i++)
            for(j=0;j<5;j++)
                Put_pixel(20+i,100+j,Red);
        for(i=0;i<100;i++)
            for(j=0;j<5;j++)
                Put_pixel(20+j,100+i,Red);
        LCD_PutString24(60,140,"晶晶我爱你",Yellow, Black); //欢迎界面，24x24大小汉字，字模软件隶书小二号
        //    Put16x16(10,10,"",Blue,Green);
        LCD_PutString(24,16,"123蓝芯",White,Blue);
    }

}
