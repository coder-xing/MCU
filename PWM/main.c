/*程序功能
 * 实现TA一路PWM输出
 * 输出口为P1.2口
 *
 *
 *
 * */
#include <msp430f169.h>
#define  uchar unsigned char
#define uint unsigned int
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
/**
 * main.c
 */
void pwm(uint period,uint percent){
    CCR0=period;
    TACCR1=(percent/100.0)*period;
}
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                  //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}
void port1_init(){

    P1IE|=(BIT0+BIT1+BIT2+BIT3);
    P1IES|=(BIT0+BIT1+BIT2+BIT3);
    P1DIR&=~(BIT0+BIT1+BIT2+BIT3);
    P1IFG =0;
    P6DIR=0xff;
    P6OUT=0x23;

}
uchar a=3.0;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	port1_init();
//	P1DIR |=BIT6+BIT2;
	P1SEL|=BIT6+BIT2;
	P1DIR |=BIT6+BIT2;
	 _EINT();
	TACTL=TASSEL_2+MC_1+ID_3;//SMCLK时钟源  MC_1增计数模式 8分频
	TACCTL1=OUTMOD_7;
	while(1) pwm(20000,a);;
}

#pragma vector= PORT1_VECTOR
__interrupt void keys(void)
{
    if((P1IN&BIT0)==0)
    {
        a+=1;
    }
    if((P1IN&BIT1)==0)
    {
        a-=1;
    }
    if((P1IN&BIT2)==0)
    {
        P6OUT=0x02;
    }
    if((P1IN&BIT3)==0)
    {
        P6OUT=0xff;
    }

P1IFG=0;
}
