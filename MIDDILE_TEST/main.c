#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
/*矩阵按键*/
#define KEYDIR P2DIR
#define KEYOUT P2OUT
#define KEYIN P2IN
#define KEYIE P2IE
#define KEYIES P2IES
#define KEYIFG P2IFG
uchar keyval;
uchar posi=0;
/*矩阵按键初始化*/
void keyboard_init(){
   KEYDIR =0x0f;//高四位输入，低四位输出
   KEYOUT =0xf0;
   KEYIE=0xf0;//高4位中断允许位
   KEYIES=0xf0;//下降沿触发
   KEYIFG =0x00;//清除中断标志位
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

void UART_Init()
{
  U0CTL|=SWRST;               //复位SWRST
  U0CTL|=CHAR;                //8位数据模式
  U0TCTL|=SSEL1;              //SMCLK为串口时钟
  U0BR1=baud_h;               //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;               //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                //微调寄存器为0，波特率9600bps
  ME1|=UTXE0;                 //UART0发送使能
  ME1|=URXE0;                 //UART0接收使能
  U0CTL&=~SWRST;
  IE1|=URXIE0;                //接收中断使能位
  P3SEL|= BIT4;               //设置IO口为普通I/O模式
  P3DIR|= BIT4;               //设置IO口方向为输出
  P3SEL|= BIT5;
}
//向串口发送数据
void Send_Byte(uchar data)
{
  while((IFG1&UTXIFG0)==0);          //发送寄存器空的时候发送数据
    U0TXBUF=data;
}

void main(void)
{   P6DIR=0xff;
    P6OUT=0xff;
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    Clock_Init();                       //系统时钟设置
    UART_Init();                        //串口设置初始化
    keyboard_init();                    //矩阵按键初始化
    LCD_Port_init();                     //1602端口初始化
    LCD_clear();                        //清屏
    LCD_init();                         //1602初始化
    _EINT();                            //开总中断
    while(1);                         //无限循环
     // P6OUT=~keyval;
//     Send_Byte(keyval);
//      delay_ms(100);

}
//串口接收数据中断
#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  uchar data;
  data=U0RXBUF;
//  delay_us(1);
//  LCD_write_num(posi++,1,data);
  P6OUT=~data;//接收到的数据存起来
  Send_Byte(data);                    //将接收到的数据再发送出去
}

//矩阵键盘获取键值
#pragma vector=PORT2_VECTOR
__interrupt void key_cal(void)
{  // P6OUT=0X0f;
    if((KEYIN & 0xf0)<0xf0)
    {   delay_ms(15);//消抖
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
