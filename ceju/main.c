/*��Ŀ��飺����Ŀ���ڲ��Գ�����ģ���ʹ�á�*/
#include <msp430f169.h>
#include "LCD1602.h"
//#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
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
/***********************************
 * ��������:ʵ�ֶ�ʱ��A��PWM��ʼ��
 * �Ĵ���ѡ��:TACCTL1
 ***********************************/
void TimeA_init(){
    TACTL |=TASSEL_2+ ID_3+MC_2;        //ѡ��SMCLK��Ϊ��ʱ��A��ʱ�ӣ�8��Ƶ����������ģʽ;
    TACCTL1=CM_3+CCIS_0+CAP+SCS+CCIE;   //ѡ�������½��ض�����(CM_1:�����ز���CM_2:�½��ز���)
                                        //����Դ:CCI1A,����ģʽ,ͬ������,�����ж�����
    _EINT();                            //�����ж�
}
void port_init(){
     P1SEL |=BIT2;                      //ѡ��P1.2�ڶ����ܽ�:��ʱ��A����CCI1A����;
     P3DIR |=BIT1;                      //P1.1������Ϊ���ģʽ
     P6DIR=0xff;
     P6OUT=0xff;
     P3OUT =BIT1;
}

uint transform(uint num){
    uchar a,zhi=0;
    for(a=15;a>=0;a--){
        zhi+=(num>>a)*(2^a);
    }
    return zhi;
}
/*
 * main.c
 */
uint time1=0;
uint time;
void main(void)
{   uchar distance;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	TimeA_init();
	Clock_Init();
	port_init();
	Port_init();
	LCD_clear();
	LCD_init();
	//delay_us(10);

	while(1)
	{
	    P3OUT|=BIT1;
	    delay_us(15);
	    P3OUT=0x00;
	    delay_ms(10);
	    distance=(time *17)/100;
	    P6OUT=~distance;
	    LCD_write_single_char(1,1,distance/1000+'0');
	    LCD_write_single_char(2,1,distance%1000/100+'0');
	    LCD_write_single_char(3,1,distance%1000%100/10+'0');
	    LCD_write_single_char(4,1,'.');
	    delay_ms(20);

	}
}

#pragma vector =TIMERA1_VECTOR
__interrupt void ceju()
{
    switch(TAIV)
    {
    case 0x02:
     if(time1==0)
     {
         TAR=0;
         TACCR1=0;
         time1=1;
     }
     else
     {
         time=TACCR1;
         time1=0;
     }
    break;
    case 0x04:break;
    case 0x10:break;
    }
}


