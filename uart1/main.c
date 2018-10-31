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
�������ƣ�GetChar
��    �ܣ��򿪷��巢���ַ�
��    ������
����ֵ  ��char
******************************** ************/
uchar GetChar(void)
{
      uchar c;

      while(URXIFG0 == 1);
      c = RXBUF0;

      return c;                       //���յ����ַ����ͳ�ȥ
}


/*******************************************
�������ƣ�PutChar
��    �ܣ��򿪷��巢���ַ�
��    ����uchar
����ֵ  ����
********************************************/
void PutChar(uchar c)
{
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = c;              //���յ����ַ����ͳ�ȥ
}


/*******************************************
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ������
����ֵ  ����
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));                // TX������У�
            TXBUF0 = *ptr++;                       // ��������
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
�������ƣ�Delays
��    �ܣ���ʱһ��
��    ������
����ֵ  ����
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
�������ƣ�InitUART
��    �ܣ���ʼ��UART�˿�
��    ������
����ֵ  ����
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
    IE1 |= URXIE0;                            // ʹ��USART0�Ľ����ж�
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

    WDTCTL = WDTPW + WDTHOLD;                 // �ع�

    InitUART();
    _EINT();                                  //��ȫ���ж�

    while(i--)
    {
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = value++;
        value &= 0x7f;                        // ��֤value����ֵС��128
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = '\n';
        Delays();
    }

    PutString(tishi);


    while(j--)
    {
        PutChar(pstr);
        //if(IFG1 & URXIFG0)      //����յ��ַ�
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
