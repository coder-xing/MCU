/*�����ܣ���ͨ������ת��ADC12
 * �ص㣺�����ⲿ�ο���ѹ
 * �ӿ�:P6.0������ad��ѹֵ����1602��ʾ��ȡ�ĵ�ѹֵ
 * */
#include <msp430f169.h>
#include <LCD1602.h>
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	ADC12CTL0 =SHT0_2 + ADC12ON;//����ʱ��
	ADC12CTL1 =SHP;             //
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
	}
}
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_deal(void)
{   unsigned  long data;
        data=ADC12MEM0;
        data=data*330;
        data=data/4096;
        LCD_write_num(1,0,data/100);
        LCD_write_single_char(2,0,'.');
        LCD_write_num(3,0,data/10%10);
        LCD_write_num(4,0,data%10);
       // LCD_write_num(5,0,data%10);

}
