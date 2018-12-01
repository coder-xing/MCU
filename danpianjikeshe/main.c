#include <msp430f169.h>
#include "OLED.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define OE_H P1OUT |=BIT0
#define OE_L P1OUT &=~BIT0
#define CE_H P1OUT |=BIT1
#define CE_L P1OUT &=~BIT1
#define WE_H P1OUT |=BIT2
#define WE_L P1OUT &=~BIT2
#define UB_H P1OUT |=BIT3
#define UB_L P1OUT &=~BIT3
#define LB_H P1OUT |=BIT4
#define LB_L P1OUT &=~BIT4
#define ADDSET_H P1OUT |=BIT5
#define ADDSET_L P1OUT &=~BIT5
#define ADDRESS P2IN
#define OUTPUT_L P3OUT
#define OUTPUT_H P6OUT
#define uchar unsigned char
#define uint unsigned int
void Clock_Init()
{
  unsigned char i;
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
void Data_output(unsigned int dat)
{   unsigned char cmp;
    WE_L;
    delay_us(1);
    CE_L;
    delay_us(1);
    LB_L;
    UB_H;
    OUTPUT_L=(dat&0xff);//发送低八位
    delay_us(1);
    LB_H;
    UB_L;
    OUTPUT_L=((dat>>8)&0xff);//发送高八位
    CE_H;
    WE_H;
}

void RAM_port_init()
{
    P5SEL|=BIT5;
    P3DIR=0xff;//低8位数据输出
    P6DIR=0xff;//高八位数据输出
    P1DIR |=BIT0+BIT2+BIT3+BIT4;//控制线
    P2DIR =0x00;//五根地址线
}

void Write_ram(unsigned int *data)
{   ADDSET_H;
    unsigned char addr=0,i;
    for(i=0;i<32;i++)
    {   ADDRESS=addr;
        Data_output(*data);
        addr++;
        data++;
    }
}

void Read_ram()
{   ADDSET_H;
    CE_L;
    delay_us(1);
    OE_L;
    LB_L;
    UB_L;
}

void Stop_read_ram()
{
    OE_H;
    CE_H;
    LB_H;
    UB_H;
}
void PWM_init()
{
    TACTL=TASSEL_2+MC_1;//SMCLK时钟源  MC_1
    TACCTL1=OUTMOD_7;
}
void Period_control(unsigned int time)//time单位us;
{  // unsigned int mo;
   // mo=time/32
   TACCR0=time;
   TACCR1=time/2;
}
/**
 * main.c
 */

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	RAM_port_init();
	P1SEL |=BIT6;
	P1DIR |=BIT6;
	PWM_init();
	Period_control(2);
	while(1)
	{
	   //  P6OUT ^=BIT0;
	     //delay_ms(1000);
	}
}


/*  I2C_init(0x78);
    oled_init();
    OLED_Clear();
    for(a=0;a<4;a++)
    fonts_display(a,0,9+a);
    OLED_ShowChar(64,0,':');
    for(a=0;a<3;a++)
    fonts_display(5+a,0,13+a);
    for(a=0;a<2;a++)
    fonts_display(0+a,2,24+a);
    OLED_ShowChar(32,2,':');
    for(a=0;a<2;a++)
    fonts_display(0+a,4,26+a);
    OLED_ShowChar(32,4,':');*/
