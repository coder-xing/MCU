#include <msp430f169.h>
#include "i2c.h"
/**
 * main.c
 */
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

void main(void)
{   P6DIR=0xff;
    uchar s=0;
    Clock_Init();
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	i2c_write(0xa0,0x05,0x06);
	    delay_ms(1000);
	    s=i2c_read(0xa0,0x05);
	    P6OUT=~s;
//	    delay_ms(1000);
	    while(1)P6OUT=~s;;
}
