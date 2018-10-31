#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
/**
 * main.c
 */
//***********************************************************************
//��ʼ��ʱ��
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                  //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                    //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}

void ADC12_init()
{   uint i;
    ADC12CTL0 = ADC12ON +REFON+REF2_5V; //�趨����ʱ��
    ADC12CTL1 = SHP;            //��ͨ������ת����ʹ���ڲ�������ʱ����ѡ��ͬ������
    ADC12MCTL0=SREF_1;
    for(i=0;i<0x3600;i++){}
    ADC12IE =0x01;              //ʹ���ж�ת��
    ADC12CTL0 |=ENC;            //ʹ��ת����
    P6SEL |=0x01;
    _EINT();
}
void main(void)
{   //uint a,compare;
    //uchar data=0;
	WDTCTL = WDTPW | WDTHOLD;	// �رտ��Ź�
	Clock_Init();
	ADC12_init();
	Port_init();
	LCD_init();
	LCD_clear();
//	P6DIR|=BIT3;
	LCD_write_str(0,0,"value is:");
	while(1){
	    ADC12CTL0 |=ADC12SC;
	/*    while((ADC12IFG&BIT0)==0);
	    compare=ADC12MEM0;
	    for(a=0;a<12;a++){
	        data=compare>>11;
	        if(data&BIT0)
	            LCD_write_single_char(a,1,'1');
	        else
	            LCD_write_single_char(a,1,'0');
	       compare<<=1;
	    }*/
	}
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{   uchar a,data;
    uint compare;
    compare=ADC12MEM0;
    for(a=0;a<12;a++){
          data=compare>>11;
          if(data&BIT0)
               LCD_write_single_char(a,1,'1');
          else
               LCD_write_single_char(a,1,'0');
               compare<<=1;
           }
}

