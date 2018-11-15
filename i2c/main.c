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
void oled_init()
{
    i2c_write(0x00,0x8d);
    i2c_write(0x00,0x14);
    i2c_write(0x00,0xaf);
}
void oled_clear()
{   unsigned char a,b;
    for(a=0;a<8;a++)
    {
    i2c_write(0x00,0xb0+a);
    i2c_write(0x00,0x00);
    i2c_write(0x00,0x10);
    for(b=0;b<128;b++)
    i2c_write(0x40,0x00);
    }
}

void LED_on()
{
    unsigned char i,n;
    for(i=0;i<8;i++)
    {
        i2c_write(0x00,0xb0+i);
        i2c_write(0x00,0x00);
        i2c_write(0x00,0x10);
        for(n=0;n<128;n++)i2c_write(0x40,1);
    }
}
void port_init(){
    P3SEL |= 0x0A;
    U0CTL |=I2C +SYNC; //ѡ��I2cģʽ
    U0CTL &=~I2CEN;//��λ
    I2CTCTL =I2CSSEL1; //SMCLkʱ��
    I2CSA =0x78>>1;        //�ӻ���ַ
    U0CTL |=I2CEN;      //i2cʹ��
 //   I2CIE = RXRDYIE;
    P6DIR =0xff;
    P6OUT =0x00;
    P3DIR &=~0x0a;
}
/*
 * main.c
 */
void main(void)
{ //  uchar xx,yy;
	WDTCTL = WDTPW | WDTHOLD;	  // stop watchdog timer
	port_init();
	Clock_Init();
	oled_init();
	LED_on();
	oled_clear();
	while(1){

	}
}
