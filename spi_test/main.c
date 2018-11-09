#include <msp430f169.h>

#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define KEYDIR P2DIR
#define KEYOUT P2OUT
#define KEYIN P2IN
#define KEYIE P2IE
#define KEYIES P2IES
#define KEYIFG P2IFG
uchar keyval;

void keyboard_init(){
   KEYDIR =0x0f;//高四位输入，低四位输出
   KEYOUT =0xf0;
   KEYIE=0xf0;//高4位中断允许位
   KEYIES=0xf0;//下降沿触发
   KEYIFG =0x00;//清除中断标志位
}
/**
 * main.c
 */

void SPI_init(){
    U0CTL |= SWRST;                     //SPI软件复位使能
    U0CTL |= CHAR+SYNC+MM;              //8位数据 SPI模式 主机
    U0TCTL |= SSEL1+SSEL0+STC+CKPH;     //时钟选择SMCLK 3线SPI模式
    U0BR0=0x02;                         //波特率
    U0BR1=0x00;
    ME1 |= USPIE0;
    U0CTL&=~SWRST;
    IE1&=~UTXIE0;
    IE1&=~URXIE0;
    P3SEL|=0x0e;
    P3DIR|=0x01;
}

void SPI_sendByte(unsigned char dat){
    U0TXBUF=dat;
    while((IFG1&UTXIFG0)==0);
    //IFG1&=~UTXIFG0;
}

void Clock_Init(){
    unsigned char i;
    BCSCTL1&=~XT2OFF;
    BCSCTL2|=(SELM1+SELS);
    do{
        IFG1&=~OFIFG;
        for(i=0;i<100;i++){
            _NOP();
        }
    }
    while((IFG1&OFIFG)!=0);
    IFG1&=~OFIFG;
}

void main(){
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    Clock_Init();
    SPI_init();
    keyboard_init();
 //   P6DIR=0xff;
    while(1){
        delay_us(2);
        SPI_sendByte(0x05);
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void key_cal(void)
{  // P6OUT=0X0f;
    if(KEYIFG !=0x00)
    {
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
                   SPI_sendByte(keyval);
                   P6OUT=~keyval;
                   return;
               }
               cmp2<<=1;
           }
       }
   }
   }
   KEYIFG=0;
}
