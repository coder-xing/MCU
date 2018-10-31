#include <msp430.h>

char RXData = 0;
unsigned int i = 0;
char TestData = 0;

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x0A;                            // Select I2C pins
  P1OUT = 0;                                // Clear P1.0
  P1DIR |= 0x01;                            // P1.0 output

  U0CTL |= I2C + SYNC;                      // Recommended init procedure
  U0CTL &= ~I2CEN;                          // Recommended init procedure
  I2CTCTL = I2CSSEL1 + I2CRM;               // SMCLK, repeat Mode
  I2CSA = 0x0048;                           // Slave Address is 048h
  I2CIE = RXRDYIE;                          // Enable RXRDYIFG interrupt
  U0CTL |= I2CEN;                           // Enable I2C

  __enable_interrupt();                     // Enable interrupts

  U0CTL |= MST;                             // Master mode
  I2CTCTL |= I2CSTT;                        // Initiate transfer

  while (1)
  {
    if (i == 6)
    {
     i = 0;
     TestData++;
     I2CTCTL |= I2CSTP;
     while (I2CTCTL & I2CSTP);              // Check Stop Condition
     U0CTL |= MST;
     I2CTCTL |= I2CSTT;

    }
    i++;
    __bis_SR_register(CPUOFF);              // Enter LPM0
    P1OUT |= 0x01;                          // Set P1.0
    if (TestData++ != RXData) break;        // Test received data
    P1OUT &= ~0x01;                         // Clear P1.0
  }

  while (1);                                // Trap CPU

}

// Common ISR for I2C Module
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0TX_VECTOR
__interrupt void I2C_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0TX_VECTOR))) I2C_ISR (void)
#else
#error Compiler not supported!
#endif
{
 switch(I2CIV)
 {
   case  0: break;                          // No interrupt
   case  2: break;                          // Arbitration lost
   case  4: break;                          // No Acknowledge
   case  6: break;                          // Own Address
   case  8: break;                          // Register Access Ready
   case 10:                                 // Receive Ready
     RXData = I2CDRB;                       // RX data
     __bic_SR_register_on_exit(CPUOFF);     // Clear LPM0
     break;
   case 12: break;                          // Transmit Ready
   case 14: break;                          // General Call
   case 16: break;                          // Start Condition
 }
}
