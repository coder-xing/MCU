/*#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
#define input P5IN
unsigned char value;
uchar keyPressed;                                               //�����Ƿ񱻰���:1--�ǣ�0--��
uchar keyval=0;                                                   //��ż�ֵ
uchar keyFlag;                                                  //�����Ƿ��ѷſ���1--�ǣ�0--��
uchar key_Map[] = {1,2,3,13,4,5,6,14,7,8,9,15,11,0,12,16};

void jianpan(){
    uchar a=1;
    uchar b=0;
    for(a=1;a<0x10;a<<=1){
        P3DIR=0xff;
        P3OUT=0x00;
        P3DIR=0x0f;
        P3OUT=a;
        if(P3IN!=a){
            value=~P3IN;
            b=1;
        }
    }
}
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	 // stop watchdog timer
	clk_init();
	Port_init();
	LCD_init();

	LCD_clear();
	LCD_write_single_char(1,1,'0');
	while(1){
	    LCD_write_str(0,0,"liu");
	    jianpan();
	    LCD_write_single_char(1,1,value);

	}

}
*/

#include <msp430x14x.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
#define input P5IN

void main()
{   Init_Port();
    uint a;
    Init_CLK();
    Port_init();
    LCD_init();
    LCD_clear();
   // LCD_write_single_char(1,1,'0');
    while(1){
        a= KeyProcess();
    LCD_write_num(1,1,a);
    }
}
void Init_Port(void)
{
    //��P1�����еĹܽ��ڳ�ʼ����ʱ������Ϊ���뷽ʽ
    P1DIR = 0;
    //��P1�����еĹܽ�����Ϊһ��I/O��
    P1SEL = 0;
    // ��P1.4 P1.5 P1.6 P1.7����Ϊ�������
    P1DIR |= BIT4;
    P1DIR |= BIT5;
    P1DIR |= BIT6;
    P1DIR |= BIT7;
    //������͵�ƽ
    P1OUT = 0x00;
    // ���жϼĴ�������
    P1IE = 0;
    P1IES = 0;
    P1IFG = 0;
    //�򿪹ܽŵ��жϹ���
    //��Ӧ�Ĺܽ��ɸߵ��͵�ƽ����ʹ��Ӧ�ı�־��λ
    P1IE |= BIT0;
    P1IES |= BIT0;
    P1IE |= BIT1;
    P1IES |= BIT1;
    P1IE |= BIT2;
    P1IES |= BIT2;
    P1IE |= BIT3;
    P1IES |= BIT3;
    _EINT();//���ж�
    return;
}
void Delay(void)
{
    int i;
    for(i = 100;i>0;i--) ;//��ʱһ��ʱ��
}
int KeyProcess(void)
{
    int nP10,nP11,nP12,nP13;
    int nRes = 0;
    //P1.4����͵�ƽ
    P1OUT &= ~(BIT4);
    nP10 = P1IN & BIT0;
    if (nP10 == 0) nRes = 13;
    nP11 = P1IN & BIT1;
    if (nP11 == 0) nRes = 14;
    nP12 = P1IN & BIT2;
    if (nP12 == 0) nRes = 15;
    nP13 = P1IN & BIT3;
    if (nP13 == 0) nRes = 16;
    //P1.5����͵�ƽ
    P1OUT &= ~(BIT4);
    nP10 = P1IN & BIT0;
    if (nP10 == 0) nRes = 9;
    nP11 = P1IN & BIT1;
    if (nP11 == 0) nRes = 10;
    nP12 = P1IN & BIT2;
    if (nP12 == 0) nRes = 11;
    nP13 = P1IN & BIT3;
    if (nP13 == 0) nRes = 12;
    //P1.6����͵�ƽ
    P1OUT &= ~(BIT4);
    nP10 = P1IN & BIT0;
    if (nP10 == 0) nRes = 5;
    nP11 = P1IN & BIT1;
    if (nP11 == 0) nRes = 6;
    nP12 = P1IN & BIT2;
    if (nP12 == 0) nRes = 7;
    nP13 = P1IN & BIT3;
    if (nP13 == 0) nRes = 8;
    //P1.7����͵�ƽ
    P1OUT &= ~(BIT4);
    nP10 = P1IN & BIT0;
    if (nP10 == 0) nRes = 1;
    nP11 = P1IN & BIT1;
    if (nP11 == 0) nRes = 2;
    nP12 = P1IN & BIT2;
    if (nP12 == 0) nRes = 3;
    nP13 = P1IN & BIT3;
    if (nP13 == 0) nRes = 4;
    P1OUT = 0x00;//�ָ���ǰֵ��
    //��ȡ�����ܽŵ�״̬
    nP10 = P1IN & BIT0;
    nP11 = P1IN & BIT1;
    nP12 = P1IN & BIT2;
    nP13 = P1IN & BIT3;
    for(;;)
    {
        if(nP10 == 1 && nP11 == 1 && nP12 == 1 && nP13 == 1)
        {
            //�ȴ��ɿ�����
            break;
        }
    }
    return nRes;
}
// �������Զ˿� 1 ���ж�
#pragma vector=PORT1_VECTOR
__interrupt void PORT_ISR(void)
{
    Delay();
    KeyProcess();
    if(P1IFG & BIT0)
    {
        P1IFG &= ~(BIT0);// ����жϱ�־λ
    }
    if(P1IFG & BIT1)
    {
        P1IFG &= ~(BIT1);// ����жϱ�־λ
    }
    if(P1IFG & BIT2)
    {
        P1IFG &= ~(BIT2);// ����жϱ�־λ
    }
    if(P1IFG & BIT3)
    {
        P1IFG &= ~(BIT3);// ����жϱ�־λ
    }
}
void Init_CLK(void)
{
    unsigned int i;
    BCSCTL1 = 0X00;         //���Ĵ�������������
                    //XT2��������
                    //LFTX1�����ڵ�Ƶģʽ
                    //ACLK�ķ�Ƶ����Ϊ1

    do
    {
    IFG1 &= ~OFIFG;                       // ���OSCFault��־
    for (i = 0x20; i > 0; i--);
    }
    while ((IFG1 & OFIFG) == OFIFG);      // ���OSCFault =1

    BCSCTL2 = 0X00;         //���Ĵ�������������
    BCSCTL2 += SELM1;       //MCLK��ʱ��ԴΪTX2CLK����Ƶ����Ϊ1
    BCSCTL2 += SELS;        //SMCLK��ʱ��ԴΪTX2CLK����Ƶ����Ϊ1
}

