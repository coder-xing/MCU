#include <msp430f169.h>
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
#define uchar unsigned char

uchar keyval;
void keyboard_init(){
    KEYDIR =0xf0;//高四位输出，低四位输入
    KEYOUT =0x00;
    KEYIE=0x0f;//高4位中断允许位
    KEYIES=0x0f;//下降沿触发
    KEYIFG =0x00;//清除中断标志位
    P3DIR |=BIT0;//用于产生一个脉冲
}
void Clock_Init()
{
    uchar i;
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
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	keyboard_init();
	Clock_Init();
	while(1);
}
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{ //  uchar fre;
    if((KEYIN & 0x0f)<0x0f)
    {   delay_ms(15);//消抖
    uchar line,row,cmp1,cmp2;
    cmp1=0x70;
    for(line=0;line<4;line++)
    {
        KEYOUT=cmp1;
        cmp1>>=1;
        if((KEYIN & 0xf0)<0xf0)
        {   cmp2=0x01;
        for(row=0;row<4;row++)
        {
            if((KEYIN &cmp2)==0)
            {
                keyval= line+row*4;
            //    P6OUT=~keyval;
                return;
            }
            cmp2<<=1;
        }
        }
    }
}
}
