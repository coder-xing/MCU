#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
/*���󰴼�*/
#define KEYDIR P2DIR
#define KEYOUT P2OUT
#define KEYIN P2IN
#define KEYIE P2IE
#define KEYIES P2IES
#define KEYIFG P2IFG
uchar keyval;
uchar posi=0;
/*���󰴼���ʼ��*/
void keyboard_init(){
   KEYDIR =0x0f;//����λ���룬����λ���
   KEYOUT =0xf0;
   KEYIE=0xf0;//��4λ�ж�����λ
   KEYIES=0xf0;//�½��ش���
   KEYIFG =0x00;//����жϱ�־λ
}

void get_key(){
    uchar line,row,cmp1,cmp2;
            cmp1=0x07;
            for(line=0;line<4;line++)
            {
      //     KEYOUT =0x0f;
                KEYOUT=cmp1;
                cmp1>>=1;
                if((KEYIN & 0xf0)<0xf0)
                {   cmp2=0x10;
                    for(row=0;row<4;row++)
                {
                   if((KEYIN &cmp2)==0)
                   {
                       keyval= line*4+row;
                      return;
                   }
                   cmp2<<=1;
               }
           }
       }
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

void UART_Init()
{
  U0CTL|=SWRST;               //��λSWRST
  U0CTL|=CHAR;                //8λ����ģʽ
  U0TCTL|=SSEL1;              //SMCLKΪ����ʱ��
  U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                //΢���Ĵ���Ϊ0��������9600bps
  ME1|=UTXE0;                 //UART0����ʹ��
  ME1|=URXE0;                 //UART0����ʹ��
  U0CTL&=~SWRST;
  IE1|=URXIE0;                //�����ж�ʹ��λ
  P3SEL|= BIT4;               //����IO��Ϊ��ͨI/Oģʽ
  P3DIR|= BIT4;               //����IO�ڷ���Ϊ���
  P3SEL|= BIT5;
}
//�򴮿ڷ�������
void Send_Byte(uchar data)
{
  while((IFG1&UTXIFG0)==0);          //���ͼĴ����յ�ʱ��������
    U0TXBUF=data;
}

void main(void)
{   P6DIR=0xff;
    P6OUT=0xff;
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    Clock_Init();                       //ϵͳʱ������
    UART_Init();                        //�������ó�ʼ��
    keyboard_init();                    //���󰴼���ʼ��
    LCD_Port_init();                     //1602�˿ڳ�ʼ��
    LCD_clear();                        //����
    LCD_init();                         //1602��ʼ��
    _EINT();                            //�����ж�
    while(1);                         //����ѭ��
     // P6OUT=~keyval;
//     Send_Byte(keyval);
//      delay_ms(100);

}
//���ڽ��������ж�
#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  uchar data;
  data=U0RXBUF;
//  delay_us(1);
//  LCD_write_num(posi++,1,data);
  P6OUT=~data;//���յ������ݴ�����
  Send_Byte(data);                    //�����յ��������ٷ��ͳ�ȥ
}

//������̻�ȡ��ֵ
#pragma vector=PORT2_VECTOR
__interrupt void key_cal(void)
{  // P6OUT=0X0f;
    if((KEYIN & 0xf0)<0xf0)
    {   delay_ms(15);//����
        uchar line,row,cmp1,cmp2;
        cmp1=0x07;
        for(line=0;line<4;line++)
        {
  //     KEYOUT =0x0f;
            KEYOUT=cmp1;
            cmp1>>=1;
            if((KEYIN & 0xf0)<0xf0)
            {   cmp2=0x10;
                for(row=0;row<4;row++)
            {
               if((KEYIN &cmp2)==0)
               {
                   keyval= line*4+row;
                   P6OUT=~keyval;
                  return;
               }
               cmp2<<=1;
           }
       }
   }
        Send_Byte(keyval);
   }

   KEYIFG=0;
}
