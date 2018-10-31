#include <msp430f169.h>
#include "i2c.h"
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
void port_init(){
    P3SEL |= 0x0A;
    U0CTL |=I2C +SYNC; //ѡ��I2cģʽ
    U0CTL &=~I2CEN;//��λ
    I2CTCTL =I2CSSEL1; //SMCLkʱ��
    I2CSA =0xd0;       //�ӻ���ַ
    U0CTL |=I2CEN;      //i2cʹ��
    I2CIE = RXRDYIE;
    P6DIR =0xff;
    P6OUT =0x00;
    P3DIR |=BIT1+BIT3;
}
/*
 * main.c
 */
void main(void)

{   uchar xx,yy;
	WDTCTL = WDTPW | WDTHOLD;	  // stop watchdog timer
	port_init();
	Clock_Init();
	while(1){
	yy=hex_bcd(10);
	i2c_write(0x02,0x06);
	xx=i2c_read(0x03);
	P6OUT=i2c_read(0x03);
	uchar light=bcd_hex(xx);
	//P6OUT=light;
	delay_ms(8000);
	}
}
