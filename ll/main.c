#include "msp430x16x.h"
typedef unsigned char u8;
typedef unsigned int u16;
void GPIO_init(){
  P1IES &=~BIT2;
  P1IE |=BIT2;
  P1DIR &=~BIT2;
  P1IFG &=~BIT2;
  _EINT();
}
void delay(u16 i){
  while(i--);
}

void P6PORT_init(){
  P6DIR |= BIT0;
  P6OUT &=~BIT0;
}
void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  GPIO_init();
  P6PORT_init();
  while(1);
}
#pragma vector =PORT1_VECTOR
__interrupt void p6_led(void)
{
  if(P1IFG & BIT2){
    P6OUT ^=BIT0;
    P1IFG &=~(BIT2);
  }
}
