#include <msp430f169.h>
#include "LCD1602.h"
/**
 * main.c
 */
unsigned int result[];
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P6SEL |=BIT0;
	TACTL |=TASSEL1+MC0+TACLR;//SMCLK,增计数模式，不分频
	TACCR0 =0x3999;
	TACCR1 =0x1fff;
	ADC12CTL0 =ADC12ON +SHT0_1 +REF2_5V +REFON;
	ADC12CTL1 =SHP +CONSEQ_2 +SHS_1;
	ADC12MCTL0 =INCH_0 +SREF_1;
	ADC12IE |=BIT0;
	ADC12CTL0 |=ENC +ADC12SC;
	_EINT();
	return 0;
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12(void)
{
    static unsigned int n=0;
    result[n++]=ADC12MEM0;
    if(n==512){
        ADC12CTL0 &=~ENC;
        n=0;
    }
}
