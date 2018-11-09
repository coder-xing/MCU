#include <msp430f169.h>

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	 // stop watchdog timer
//	P1DIR=0x00;
	P3DIR=0x00;
	P6DIR=0xff;
//	_EINT();
	while(1){
	    if(P3IN&BIT2==1)
	    {
	        P6OUT=~BIT2;
	    }
	    else
	        P6OUT=~BIT3;
	}
}


