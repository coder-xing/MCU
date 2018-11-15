/*#include <msp430f169.h>
#include "LCD1602.h"
 unsigned int results[];
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	 // stop watchdog timer
	P6SEL |=BIT1+BIT2;
	ADC12CTL0 =ADC12ON +SHT0_8+MSC;
	ADC12CTL1 =SHP+CONSEQ_3;
	ADC12MCTL1|=INCH_1;
	ADC12MCTL2|=INCH_2+EOS;
	ADC12CTL0|=ENC;
	LCD_Port_init();
	LCD_clear();
	LCD_init();
//	ADC12IE =0x04;
//	_EINT();
	while(1)
	{
	    ADC12CTL0 |=ADC12SC;
	    while((ADC12IFG&BIT1)==0);
	    results[0]=(long)ADC12MEM1 *330;
	    results[0]=results[0]/4096;
	    LCD_write_num(0,0,results[0]/100);
	    LCD_write_single_char(1,0,'.');
	    LCD_write_num(2,0,results[0]/10%10);
	    LCD_write_num(3,0,results[0]%10);
	    LCD_write_single_char(4,0,'V');
	    while((ADC12IFG&BIT2)==0);
	    results[1]=(long)ADC12MEM2 *330;
	    results[1]=results[1]/4096;
	    LCD_write_num(0,1,results[1]/100);
	    LCD_write_single_char(1,1,'.');
	    LCD_write_num(2,1,results[1]/10%10);
	    LCD_write_num(3,1,results[1]%10);
	    LCD_write_single_char(4,1,'V');
	}
}
/*#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    }
*/


/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, results, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP-FET430P140 Demo - ADC12, Sequence of Conversions (non-repeated)
//
//  Description: This example shows how to perform A/D conversions on a sequence
//  of channels. A single sequence of conversions is performed - one conversion
//  each on channels A0, A1, A2, and A3. Each conversion uses AVcc and AVss for
//  the references. The conversion resultss are stored in ADC12MEM0, ADC12MEM1,
//  ADC12MEM2, and ADC12MEM3 respectively and are moved to 'resultss[]' upon
//  completion of the sequence. Test by applying voltages to pins A0, A1, A2,
//  and A3, then setting and running to a break point at the "_BIC..."
//  instruction in the ISR. To view the conversion resultss, open a watch window
//  in debugger and view 'resultss' or view ADC12MEM0, ADC12MEM1, ADC12MEM2, and
//  ADC12MEM3 in an ADC12 SFR window.
//  This can run even in LPM4 mode as ADC has its own clock
//  Note that a sequence has no restrictions on which channels are converted.
//  For example, a valid sequence could be A0, A3, A2, A4, A2, A1, A0, and A7.
//  See the MSP430x1xx User's Guide for instructions on using the ADC12.
//
//
//                MSP430F149
//            -----------------
//           |                 |
//   Vin0 -->|P6.0/A0          |
//   Vin1 -->|P6.1/A1          |
//   Vin2 -->|P6.2/A2          |
//   Vin3 -->|P6.3/A3          |
//           |                 |
//
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
static unsigned int results[4];             // Needs to be global in this example
                                            // Otherwise, the compiler removes it
                                            // because it is not used for anything.
int main(void)
{

    WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
    P6SEL |= 0x06;                             // Enable A/D channel inputs
    ADC12CTL0 = ADC12ON+SHT0_2+MSC;           // Turn on ADC12, set sampling time
    ADC12CTL1 = SHP+CONSEQ_3;                 // Use sampling timer, single sequence
 // ADC12MCTL0 = INCH_0;                      // ref+=AVcc, channel = A0
    ADC12MCTL1 = INCH_1;                      // ref+=AVcc, channel = A1
    ADC12MCTL2 = INCH_2+EOS;                  // ref+=AVcc, channel = A2
 // ADC12MCTL3 = INCH_3;                      // ref+=AVcc, channel = A3, end seq.
 // ADC12IE = 0x08;                           // Enable ADC12IFG.3
    delay_us(5);
    ADC12CTL0 |=ENC;                          // Enable conversions
    LCD_Port_init();
    LCD_clear();
    LCD_init();
    _EINT();
    while(1)
    {
        ADC12CTL0 |= ADC12SC;
        delay_us(4);
        while((ADC12IFG&BIT1)==0);
        results[0] = (long)ADC12MEM1*330;  // Start conversion
        results[0]=results[0]/4096;
        LCD_write_num(0,0,results[0]/100);
        LCD_write_single_char(1,0,'.');
        LCD_write_num(2,0,results[0]/10%10);
        LCD_write_num(3,0,results[0]%10);
        LCD_write_single_char(4,0,'V');
        while((ADC12IFG&BIT2)==0);
        results[1] =(long)ADC12MEM2*330;  // Start conversion
        results[1]=results[1]/4096;
        LCD_write_num(0,1,results[1]/100);
        LCD_write_single_char(1,1,'.');
        LCD_write_num(2,1,results[1]/10%10);
        LCD_write_num(3,1,results[1]%10);
        LCD_write_single_char(4,0,'V');
  }
}
/*#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  results[0] = (long)ADC12MEM0*330;                   // Move resultss, IFG is cleared
  results[1] = (long)ADC12MEM1*330;                  // Move resultss, IFG is cleared
  results[2] = (long)ADC12MEM2*330;                  // Move resultss, IFG is cleared
  results[3] =(long)ADC12MEM3*330;            // Move resultss, IFG is cleared
}*/
