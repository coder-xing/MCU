#include <msp430f169.h>
#include "TFT2.8.h"
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
extern void delayms(unsigned int count);

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
void LED_Light()
{
    LED8=0x00;                    //点亮LED
    delay_ms(500);
    LED8=0xff;                    //熄灭LED
    delay_ms(500);
}

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	uint i,y;
	Clock_Init();                      //系统时钟设置
	Port_Init();                       //系统初始化，设置IO口属性
	LED_Light();                       //LED点亮一次，如有现象，说明单片机初始化成功
	Device_code=0x9320;                //TFT控制IC型号
	TFT_Initial();                     //初始化LCD
	CLR_Screen(Black );
	while(1)
	{
	    Show_Image(20,80,20,100);
//	    for(i=0;i<320;i++){ Put_pixel(68,i,Blue);}                   //第5个区域打点实现画线
//	    for(i=200;i>50;i--){ Put_pixel(40,i,Red );}
//	    for(y=0;y<200;y++)
//	    for(i=200;i>50;i--){ Put_pixel(i,i+y,Red );}
//	    delay_ms(1000);//第5个区域打点实现画线
//	    CLR_Screen(Black );
	  //      CLR_Screen(Red);
	}



}
