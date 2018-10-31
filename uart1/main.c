#include  <msp430x16x.h>
typedef unsigned char uchar;
typedef unsigned int  uint;
void Delays(void);
uchar GetChar(void);
void PutChar(uchar c);
void PutString(uchar *ptr);
void InitUART(void);
uchar pstr;
/************** *****************************
函数名称：GetChar
功    能：向开发板发送字符
参    数：无
返回值  ：char
******************************** ************/
uchar GetChar(void)
{
      uchar c;

      while(URXIFG0 == 1);
      c = RXBUF0;

      return c;                       //将收到的字符发送出去
}


/*******************************************
函数名称：PutChar
功    能：向开发板发送字符
参    数：uchar
返回值  ：无
********************************************/
void PutChar(uchar c)
{
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = c;              //将收到的字符发送出去
}


/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：无
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));                // TX缓存空闲？
            TXBUF0 = *ptr++;                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
函数名称：Delays
功    能：延时一会
参    数：无
返回值  ：无
********************************************/
void Delays(void)
{
    uchar i=20;
    uint j;


    while(i--)
    {
            j=2000;
            while(j--);
    }
}
/*******************************************
函数名称：InitUART
功    能：初始化UART端口
参    数：无
返回值  ：无
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    U0CTL |= CHAR;                            // 8-bit character
    U0TCTL |= SSEL0;                          // UCLK = ACLK
    U0BR0 = 0x03;                             // 32k/9600 - 3.41
    U0BR1 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    U0CTL &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;                            // 使能USART0的接收中断
}
void main(void)
{
    uchar *tishi = " MCU sends 0~127 to PC and the\
                    \n screen will display their corresponding\
                    \n ASCII code as follows:";
    uchar value = 0;
    //uchar c;
    int i = 10;
    int j = 100;

    WDTCTL = WDTPW + WDTHOLD;                 // 关狗

    InitUART();
    _EINT();                                  //打开全局中断

    while(i--)
    {
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = value++;
        value &= 0x7f;                        // 保证value的数值小于128
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = '\n';
        Delays();
    }

    PutString(tishi);


    while(j--)
    {
        PutChar(pstr);
        //if(IFG1 & URXIFG0)      //如果收到字符
          //c = RXBUF0;
        //PutChar(c);
        Delays();
    }

    while(1)
    {
        Delays();
    }
}


#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
    uchar *Pstring = " Receive Data :";
    pstr = RXBUF0;
    PutString(Pstring);
    PutChar(pstr);

}
