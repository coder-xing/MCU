/************************************
 * ������:ADC12��ͨ������ת��
 * �ص㣺�����ⲿ�ο���ѹ
 * �ӿ�:P6.0������ad��ѹֵ����1602��ʾ��ȡ�ĵ�ѹֵ
 *************************************/
#include <msp430f169.h>
#include <LCD1602.h>
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
unsigned  long data;
void main(void)
{   unsigned int a,b;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	ADC12CTL0 =SHT0_2 + ADC12ON;//����ʱ��
	ADC12CTL1 =SHP+ADC12SSEL_3;             //
	ADC12IE =0x01;
	ADC12MCTL0=SREF_1;
	ADC12CTL0 |=ENC;
	LCD_Port_init();
	LCD_clear();
	LCD_init();
	P6SEL |=0x01;
	P3DIR |=0x01;
	_EINT();
	while(1){
	    ADC12CTL0 |=ADC12SC;
	    for(a=0;a<12;a++)
	        {
	            b=data>>(11-a);
	            if((b &BIT0)==1)
	                LCD_write_single_char(a,0,1+'0');
	            else
	                LCD_write_single_char(a,0,0+'0');

	        }
	/*    LCD_write_num(1,0,data/100);
	    LCD_write_single_char(2,0,'.');
	    LCD_write_num(3,0,data/10%10);
	    LCD_write_num(4,0,data%10);*/
	}
}
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_deal(void)
{
        data=ADC12MEM0;
 //       data=data*330;
 //       data=data/4096;

       // LCD_write_num(5,0,data%10);

}
