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
    KEYDIR =0xf0;//����λ���������λ����
    KEYOUT =0x00;
    KEYIE=0x0f;//��4λ�ж�����λ
    KEYIES=0x0f;//�½��ش���
    KEYIFG =0x00;//����жϱ�־λ
    P3DIR |=BIT0;//���ڲ���һ������
}
void Clock_Init()
{
    uchar i;
    BCSCTL1&=~XT2OFF;                 //��XT2����
    BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
    do{
        IFG1&=~OFIFG;                   //������������־
        for(i=0;i<100;i++)
            _NOP();
    }
    while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
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
    {   delay_ms(15);//����
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
