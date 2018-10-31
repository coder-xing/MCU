#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
/**
 * main.c
 */
//***********************************************************************
//初始化时钟
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                  //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                    //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}

void ADC12_init()
{   uint i;
    ADC12CTL0 = ADC12ON +REFON+REF2_5V; //设定采样时间
    ADC12CTL1 = SHP;            //单通道单次转换，使用内部采样定时器，选择同向输入
    ADC12MCTL0=SREF_1;
    for(i=0;i<0x3600;i++){}
    ADC12IE =0x01;              //使能中断转换
    ADC12CTL0 |=ENC;            //使能转换器
    P6SEL |=0x01;
    _EINT();
}
void main(void)
{   //uint a,compare;
    //uchar data=0;
	WDTCTL = WDTPW | WDTHOLD;	// 关闭看门狗
	Clock_Init();
	ADC12_init();
	Port_init();
	LCD_init();
	LCD_clear();
//	P6DIR|=BIT3;
	LCD_write_str(0,0,"value is:");
	while(1){
	    ADC12CTL0 |=ADC12SC;
	/*    while((ADC12IFG&BIT0)==0);
	    compare=ADC12MEM0;
	    for(a=0;a<12;a++){
	        data=compare>>11;
	        if(data&BIT0)
	            LCD_write_single_char(a,1,'1');
	        else
	            LCD_write_single_char(a,1,'0');
	       compare<<=1;
	    }*/
	}
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{   uchar a,data;
    uint compare;
    compare=ADC12MEM0;
    for(a=0;a<12;a++){
          data=compare>>11;
          if(data&BIT0)
               LCD_write_single_char(a,1,'1');
          else
               LCD_write_single_char(a,1,'0');
               compare<<=1;
           }
}

