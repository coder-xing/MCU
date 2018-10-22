#include <msp430f169.h>
#define CPU_F ((double)80000000)
#define delayus(x)  __delay_cycles((long)CPU_F*(double)x/1000000.0)
#define delayms(x)  __delay_cycles((long)CPU_F*(double)x/1000.0)
typedef unsigned char uchar;
typedef unsigned int uint;
 void pwm_init(){
     TACTL |=TASSEL_2+ID_3+MC_1;
     TACCTL1 |= OUTMOD_7;
     P1SEL |=BIT6;
     P1DIR |=BIT6;
     P6DIR |=0xff;
     P6OUT =0x00;
 }
 void input_percent(uint period,uint percent){
     TACCR0 =period;
     TACCR1= period *percent/100;
 }
 void Clk_Init()
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
/**
 * main.c
 */
void main(void)
{   uint i=0;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clk_Init();
	pwm_init();
	while(1){
	i=0;
	for(i=0;i<80;i+=10){
	input_percent(600,i);
	delay   ms(100);}
	}

}
