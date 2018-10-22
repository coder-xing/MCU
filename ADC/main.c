#include <msp430.h> 
#include "LCD1602.h"
/**
 * main.c
 */
long data;
void main(void)
{  uint i;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Port_init();
	LCD_init();
	LCD_clear();
	P4DIR=0xff;
	P5DIR=0xff;
	P6SEL|=0x01;
	LCD_write_str(2,1,"nihao");
	ADC12CTL0=ADC12ON+REFON+REF2_5V ;
	ADC12CTL1=SHP;
	ADC12MCTL0=SREF_1;
	for(i=0;i<0x3600;i++){}//延时等待参考电压建立
	    ADC12CTL0 |=ENC;
	while(1){
	    ADC12CTL0|=ADC12SC;
	    while((ADC12IFG&BIT0)==0);
	    data=(long)ADC12MEM0*2500/4096;
	  //  data=data*10;
	  //  data=data*10;
	  //  data=data/4096;
	    LCD_write_num(4,0 ,data%10);
	    data=data/10;
	    LCD_write_num(5,0,data%10);
	    data=data/10;
	    LCD_write_num(6,0,data%10);
	    data=data/10;
	    LCD_write_num(7,0,data%10);
	}
}
