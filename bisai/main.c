#include <msp430f169.h>
#include "L12864.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>
/*矩阵按键*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
uchar keyval;                       //获取的键盘的值0~15;
uchar func=0;                //用于确定按键13选择的功能
ulong Cal=0,confirm=0;                //用于获取输入的值
uchar position=0;                   //用于测试数据输入
uchar addr=0x80;                    //12864显示地址
uchar Lcd_H=0;
uchar flag=0;                       //主菜单标志判断
ulong confirm;
uchar f_num=0;
uchar fudian=0;
uchar n;//浮点数标志判断
ulong fudianshu=0;
float f_cal=0;         //获取浮点数小数部分
unsigned char byte[4];//将浮点数据存入数组
/*矩阵按键初始化(P1.0~P1.3上拉电阻)
 *       P1.7 P1.6 P1.5 P1.4
 * P1.0   0    1    2    3
 * P1.1   4    5    6    7
 * P1.2   8    9    up   clc
 * P1.3   func .    down ok
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
{
    P6DIR=0xff;
    P6OUT=0xff;
    WDTCTL = WDTPW + WDTHOLD;                     //关闭看门狗
    Clock_Init();                                 //系统时钟设置
    keyboard_init();                              //矩阵按键初始化
    UART_Init();                                  //串口初始化
    Port_Init();                                  //系统初始化，设置IO口属性
    LCD_init();                                   //液晶参数初始化设置
    LCD_clear();                                  //清屏
    _EINT();                            //开总中断
//    LCD_write_str(0,0,"hello");
    while(1){}
}
//串口接收数据中断
//矩阵键盘获取键值
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{   uchar a;
    unsigned char fre=0;//用于数据输出计数

 //   unsigned char fudu=0;  输出电压幅度
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
   }    if(flag==0)                               //界面选择;
        {
       switch(keyval)
       {
       case 0:break;
       case 1:Send_Byte(0x01); flag=1;break;
       case 2:Send_Byte(0x02); flag=1;break;
       case 3:Send_Byte(0x03); flag=1;break;
       default:break;
       }
        }
       else
        switch(keyval)
            {
            case 0:
                if(fudian==0)
                    Cal=Cal*10+0;
                else
                {f_cal+=((float)keyval)/(10^n) ;
                    n++;}
                    break;
            case 1:
                if(fudian==0)
                 Cal=Cal*10+1;
                 else
                 {f_cal+=((float)keyval)/(10^n);
                     n++;}
                  break;
            case 2:Cal=Cal*10+2;break;
            case 3:Cal=Cal*10+3;break;
            case 4:Cal=Cal*10+4;break;
            case 5:if(fudian==0)
                Cal=Cal*10+5;
            else
            {f_cal+=(float)keyval/(10^n) ;
                n++;}
            break;
            case 6:Cal=Cal*10+6;break;
            case 7:Cal=Cal*10+7;break;
            case 8:Cal=Cal*10+8;break;
            case 9:Cal=Cal*10+9;break;
            case 10:break;
            case 11:Cal=0;fudian=0;break;
            case 12:
                flag=0;
                Cal=0;
                fudian=0;
                break;
            case 13:
               fudian=1;
//               P6OUT=0x00;
               fudianshu=Cal;
               Cal=0;
               break;
            case 14:break;
            case 15://清除之前所输入的值
                   // Cal=0;     //清除所计算的值
                    /*    for(fre=4;fre>=1;fre--){
                        Send_Byte(Cal>>(fre-1)*8);}
                        Send_Byte(fudu);
                    if(func==0x04);*/
                    if(fudian==1)
                    {
                        confirm=confirm+f_cal;
                        memcpy( byte,&confirm,sizeof(float));
                        for(a=4;a>0;a--)
                        Send_Byte(byte[a-1]);
                    }
                    for(fre=4;fre>=1;fre--){
                        Send_Byte(fudianshu>>(fre-1)*8);}
                    Cal=0;
                    keyval=0;
                    fudian=0;
                    break;
            }
    }
    KEYIFG=0;
}

