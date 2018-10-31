/*项目简介：此项目用于测试超声波模块的使用。*/
#include <msp430f169.h>
#include "LCD1602.h"
//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
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
void TimeA_init(){
    TACTL |=TASSEL_2+ ID_3+MC_2;
    TACCTL1=CM_3+CCIS_0+CAP+SCS+CCIE;
    _EINT();
}
void port_init(){
     P1SEL |=BIT2;
     P1DIR |=BIT1;
     P6DIR=0xff;
     P6OUT=0xff;
     P1OUT =BIT1;
}

uint transform(uint num){
    uchar a,zhi=0;
    for(a=15;a>=0;a--){
        zhi+=(num>>a)*(2^a);
    }
    return zhi;
}
/*
 * main.c
 */
uint time1,time2,time;
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	TimeA_init();
	Clock_Init();
	port_init();
	Port_init();
	LCD_clear();
	LCD_init();
	delay_us(10);
	while(1)
	{
	    P1OUT=0xff;
	    delay_us(20);
	    P1OUT=0x00;
	    delay_ms(10);
	    time=transform(time);
	    time*=
	    LCD_write_single_char(1,1,)
	    delay_ms(10);
	}

}
#pragma vector =TIMERA1_VECTOR
__interrupt void ceju()
{
    switch(TAIV)
    {
    case 0x02:
    if(time1==0)
    {
        time1=CCR1;
    }
    else
    {
        time2=CCR1;
        time=time2-time1;
        time1=time2=0;
    }break;
    case 0x04:break;
    case 0x10:break;
    }
}


