/*项目简介：。*/
#include <msp430f169.h>
#include <stdio.h>
#include <string.h>
//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
unsigned int time1=0,time2=0;
unsigned int time;
unsigned char byte[4];
unsigned char cishu=0;
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
/***********************************
 * 函数功能:实现定时器A的PWM初始化
 * 寄存器选择:TACCTL1
 ***********************************/
void TimeA_init(){
    TACTL |=TASSEL_1+MC_2;        //选择SMCLK作为定时器A的时钟，8分频，连续计数模式;
    TACCTL1=CM_1+CCIS_0+CAP+SCS+CCIE;   //选择上升下降沿都捕获(CM_1:上升沿捕获，CM_2:下降沿捕获)
                                        //捕获源:CCI1A,捕获模式,同步捕获,捕获中断允许
}

void port_init(){
     P1SEL |=BIT2;                      //选择P1.2第二功能脚:定时器A捕获CCI1A输入;
     P3DIR |=BIT1;                      //P1.1脚设置为输出模式
     P6DIR=0xff;
     P6OUT=0xff;
     P3OUT =BIT1;
}
/*
 * main.c
 */
void main(void)
{//   unsigned long speed;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TimeA_init();
    Clock_Init();
    port_init();                        //端口初始化
    _EINT();                            //打开总中断
    while(1)
    {
        P6OUT=~cishu;
    }

}

#pragma vector =TIMERA1_VECTOR
__interrupt void ceju()
{
    switch(TAIV)
    {
    case 0x02:cishu++;break;
    case 0x04:break;
    case 0x10:break;
    }
}



