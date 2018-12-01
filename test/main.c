#include <msp430f169.h>
#include "LCD1602.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
/*矩阵按键*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
uchar keyval;               //获取的键盘的值0~15;
uchar func_choose=0;        //用于确定按键13选择的功能
ulong Cal=0,confirm;                //用于获取输入的值
uchar position=0;                   //用于测试数据输入
/*矩阵按键初始化(P1.0~P1.3上拉电阻)
 *       P1.7 P1.6 P1.5 P1.4
 * P1.0   0    1    2    3
 * P1.1   4    5    6    7
 * P1.2   8    9    up   clc
 * P1.3   func *    down conf
 */
void keyboard_init(){
   KEYDIR =0xf0;//高四位输出，低四位输入
   KEYOUT =0x00;
   KEYIE=0x0f;//高4位中断允许位
   KEYIES=0x0f;//下降沿触发
   KEYIFG =0x00;//清除中断标志位
}
//时钟初始化
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


void main(void)
{   P6DIR=0xff;
    P6OUT=0xff;
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    Clock_Init();                       //系统时钟设置
    keyboard_init();                    //矩阵按键初始化
    LCD_Port_init();                     //1602端口初始化
    LCD_clear();                        //清屏
    LCD_init();                         //1602初始化
    _EINT();                            //开总中断
//    LCD_write_str(0,0,"hello");
    while(1);                         //无限循环

}
//串口接收数据中断
//矩阵键盘获取键值
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{  // P6OUT=0xff;
    if((KEYIN & 0x0f)<0x0f)
    {   delay_ms(15);//消抖
        uchar line,row,cmp1,cmp2;
        cmp1=0x70;
        for(line=0;line<4;line++)
        {
            KEYOUT=cmp1;
            cmp1>>=1;
            if((KEYIN & 0xf0)<0xf0)
            {   cmp2=0x01;
                for(row=0;row<4;row++)
            {
               if((KEYIN &cmp2)==0)
               {
                   keyval= line+row*4;
                  P6OUT=~keyval;
                  return;
               }
               cmp2<<=1;
           }
       }
   }
        switch( keyval)
            {
            case 0:Cal=Cal*10+0;LCD_write_single_char(position++,0,0+'0');break;
            case 1:Cal=Cal*10+1;LCD_write_single_char(position++,0,1+'0');break;
            case 2:Cal=Cal*10+2;LCD_write_single_char(position++,0,2+'0');break;
            case 3:Cal=Cal*10+3;LCD_write_single_char(position++,0,3+'0');break;
            case 4:Cal=Cal*10+4;LCD_write_single_char(position++,0,4+'0');break;
            case 5:Cal=Cal*10+5;LCD_write_single_char(position++,0,5+'0');break;
            case 6:Cal=Cal*10+6;LCD_write_single_char(position++,0,6+'0');break;
            case 7:Cal=Cal*10+7;LCD_write_single_char(position++,0,7+'0');break;
            case 8:Cal=Cal*10+8;LCD_write_single_char(position++,0,8+'0');break;
            case 9:Cal=Cal*10+9;LCD_write_single_char(position++,0,9+'0');break;
            case 10:break;
            case 11:Cal=0;LCD_clear(); position=0;break;
            case 12:break;
            case 13:
                func_choose++;
                if(func_choose==1)
                    P6OUT=~0x01;
                if(func_choose==2)
                    P6OUT=~0x02;
                if(func_choose==3)
                    P6OUT=~0x03;
                if(func_choose==4)
                {   P6OUT=~0x04;
                    func_choose=0;
                }
               break;
            case 14:break;
            case 15:confirm=Cal;
                    Cal=0;
                    break;
            }
    }
KEYIFG=0;
}
