#include <msp430f169.h>
//unsigned char dianya[]={0,15,31,47,63,79,95,111,,127,143,159,175,191,207,223,239,255,239,223,207,191,175,159,143,127,111,95,79,63,47,31,15};
unsigned char dianya[]={128,152,176,198,217,233,245,252,
                        255,252,245,233,217,198,176,152,
                        127,103,79,57,38,22,10,3,
                        1,3,10,22,38,57,79,103,
};
void InitDAC12_0(void){
    ADC12CTL0 = REF2_5V + REFON;
    DAC12_0CTL = DAC12IR + DAC12AMP_7 +DAC12ENC+DAC12RES;
    DAC12_0DAT = 0x0000;
}
void Clock_Init()
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
unsigned char a=0;
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	TACTL |= TASSEL_2 + TACLR + MC_1 + TAIE;
	TACCR0=1;
	InitDAC12_0();
	Clock_Init();
	//_EINT();
	while(1)
	    DAC12_0DAT=dianya[a++];
}
/*#pragma vector =TIMERA1_VECTOR
__interrupt void TIme_A(void){

    switch(TAIV)                                  //需要判断中断的类型
    {
    case 2:break;
    case 4:break;
    case 10:DAC12_0DAT=dianya[a];
            a++;
            if(a==32)
                a=0;
            break;                         //设置标志位Flag
    }
}
*/
