#include <msp430f169.h>
#include<string.h>
#include"LCD1602.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位
uchar *Data="data is:";
void UART_init(){
    U0CTL |=SWRST;              //USART控制寄存器复位模式
    U0CTL |=CHAR;               //USART8位数据
    U0TCTL |=SSEL1;             //选择时钟为SMCLK  SSEL1 0x20;
    U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
    U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
    U0MCTL=0x00;                //微调寄存器为0
    ME1 |=UTXE0;                //SFR模块使能寄存器，UART0发送使能
    ME1 |=URXE0;                //UART0接收使能
    U0CTL&=~SWRST;              //0,正常工作模式
    IE1 |=URXIE0;               //IE，SFR中断使能寄存器。
    P3SEL|= BIT4;               //设置IO口为普通I/O模式
    P3DIR|= BIT4;               //设置IO口方向为输出
    P3SEL|= BIT5;
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
void Send_Byte(uchar data)
{
  while((IFG1&UTXIFG0)==0);          //发送寄存器空的时候发送数据 ,0x80;
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
  uchar data=0;              //接收到的数据存起来
  data=U0RXBUF;
  Send_Byte(data);  //将接收到的数据再发送出去
  while(*Data)
      Data++;
  *Data=data;
}
