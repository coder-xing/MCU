#include <msp430f169.h>
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
void port_init(){
    P1SEL=0x00;
    P1DIR|=BIT0+BIT1+BIT2+BIT3;
}
/**
 * main.c
 */
unsigned char step[]={0x03,0x06,0x0c,0x09};
void main(void)
{   unsigned char a,b;
    a=0;b=0x30;
    WDTCTL = WDTPW | WDTHOLD;	 // stop watchdog timer
	port_init();
	while(1){
	    b=0x03;
	    for(a=0;a<4;a++){
	        P1OUT=step[a];
	        delay_us(500);
	    }

	    }
}
