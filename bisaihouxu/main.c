#include <msp430f169.h>
void RAM_port_init()
{
    P3DIR|=BIT0+BIT1+BIT2+BIT3+BIT4;//µÿ÷∑—°‘Òπ‹Ω≈

}
void SRAM_write(unsigned char address,unsigned char )
{

}
/**
 * main.c
 */

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	RAM_port_init();
	while(1);
}
