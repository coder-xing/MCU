#include <msp430f169.h>
#include<string.h>
#include"LCD1602.h"
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ
uchar *Data="data is:";
void UART_init(){
    U0CTL |=SWRST;              //USART���ƼĴ�����λģʽ
    U0CTL |=CHAR;               //USART8λ����
    U0TCTL |=SSEL1;             //ѡ��ʱ��ΪSMCLK  SSEL1 0x20;
    U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
    U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
    U0MCTL=0x00;                //΢���Ĵ���Ϊ0
    ME1 |=UTXE0;                //SFRģ��ʹ�ܼĴ�����UART0����ʹ��
    ME1 |=URXE0;                //UART0����ʹ��
    U0CTL&=~SWRST;              //0,��������ģʽ
    IE1 |=URXIE0;               //IE��SFR�ж�ʹ�ܼĴ�����
    P3SEL|= BIT4;               //����IO��Ϊ��ͨI/Oģʽ
    P3DIR|= BIT4;               //����IO�ڷ���Ϊ���
    P3SEL|= BIT5;
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
void Send_Byte(uchar data)
{
  while((IFG1&UTXIFG0)==0);          //���ͼĴ����յ�ʱ�������� ,0x80;
    U0TXBUF=data;
}
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	 Clock_Init();
	 UART_init();
	 Port_init();
	 LCD_init();
	 LCD_clear();
	 _EINT();
	 while(1){
	     LCD_write_str(0,0,Data);
	 }
}
#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  uchar data=0;              //���յ������ݴ�����
  data=U0RXBUF;
  Send_Byte(data);  //�����յ��������ٷ��ͳ�ȥ
  while(*Data)
      Data++;
  *Data=data;
}
