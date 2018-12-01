#include <msp430f169.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "UART.h"
#include "normal.h"
#include "FLASH.h"
/*矩阵按键*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
unsigned char biaozhunbo[11];
unsigned char suijibo[128];
unsigned char recover=0;
unsigned char panduan;
unsigned char itest = 0;
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
    P3DIR |=BIT0;//用于产生一个脉冲
}

void pulse()
{
    P3OUT |=BIT0;
    delay_ms(1);
    P3OUT &=~BIT0;
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

{   uchar fre,rec,fasong=2;
P6DIR=0xff;
P6OUT=0xff;
WDTCTL = WDTPW + WDTHOLD;                     //关闭看门狗
Clock_Init();                                 //系统时钟设置
keyboard_init();                              //矩阵按键初始化
UART_Init();                                  //串口初始化
_EINT();                                     //开总中断
while(1){
//    P6OUT=FlashReadChar(InfoA+5);
    if(recover==1)                      //如果开机按了5键 则将上次的数据读取出来并发送
    {   pulse();
        rec=FlashReadChar(InfoA);
        if(rec==0x11)                   //判断是发送9个字节还是32个字节
        {  // P6OUT=0x00;
            for(fre=0;fre<10;fre++)
                biaozhunbo[fre]=FlashReadChar(InfoA+fre);   //读取字节并用数组保存
            for(fre=0;fre<10;fre++)
            {Send_Byte(biaozhunbo[fre]);}       //串口发送数据
        }
        else if(rec==0x12){
            for(fre=0;fre<128;fre++)
                suijibo[fre]=FlashReadChar(InfoB+fre);
            Send_Byte(0x12);
            for(fre=0;fre<128;fre++)
            {Send_Byte(suijibo[fre]);
            }}
        recover++;
    }
    if(keyval==15){             //确认键
        FlashErase(InfoA);      //将数据写进去
        FlashErase(InfoB);
        if(reyi_flag!=1){       //如果是正弦波 方波 三角波的一种，则调用以下算法
            FlashWriteChar(InfoA,0x11); //先将发送标志位存储
            switch(panduan)
            {
            case 1:FlashWriteChar(InfoA+1,0x01);break;  //存储波形类型
            case 2:FlashWriteChar(InfoA+1,0x02);break;
            case 3:FlashWriteChar(InfoA+1,0x03);break;
            }
            for(fre=4;fre>0;fre--){ //将波形数据存储
                FlashWriteChar(InfoA+fasong,Cal>>(fre-1)*8);
                fasong++;}
            memcpy(byte,&dianya,sizeof(float));//浮点型数据转换为4个字节数据
            for(fre=4;fre>0;fre--){            //存储浮点型数据
                FlashWriteChar(InfoA+fasong,byte[fre-1]);
                fasong++;
            }
            fasong=0;
            switch(panduan)
            {
            case 1: Send_Byte(0x11);Send_Byte(0x01);break;  //串口发送波形数据
            case 2: Send_Byte(0x11);Send_Byte(0x02);break;
            case 3: Send_Byte(0x11);Send_Byte(0x03);break;
            }
            for(fre=4;fre>=1;fre--){                        //发送频率数据 4个字节
                Send_Byte(Cal>>(fre-1)*8);}
            //            Send_Byte(0x12);
            memcpy(byte,&dianya,sizeof(float));
            for(fre=4;fre>0;fre--)
                Send_Byte(byte[fre-1]);                     //发送浮点型数据
            flag=0;
            panduan=0;}
        //任意波形
        if(reyi_flag==1)
        { //  Send_Byte(0x12);
        //unsigned char itest = 0;
        //    pulse();
        xiaoshu+=(float)f_num;                             //计算浮点型数据
        memcpy(byte,&xiaoshu,sizeof(float));                //将浮点型数据转换为4个字节数据
        if(cishu<32){
            for(fre=4;fre>0;fre--){
                suijibo[itest]=byte[fre-1];                 //发送浮点型数据
                itest++;}
        }
        //            for(fre=4;fre>0;fre--)
        //                Send_Byte(byte[fre-1]);
        cishu++;
        if(cishu==32)                                       //如果发送超过32次数据，退出发送任意波模式
        {
            //存储数据
            FlashWriteChar(InfoA,0x12);
            for(itest=0;itest<128;itest++)
                FlashWriteChar(InfoB+itest,suijibo[itest]);
            Send_Byte(0x12);            //标志位
            for(itest=0;itest<128;itest++)
                Send_Byte(suijibo[itest]);
            flag=0;reyi_flag=0;cishu=0;itest=0;
            //一次性发送
        }
        }
        f_num=0;
        Cal=0;              //用于计算键值
        keyval=0;           //键值清零
        xiaoshu=0;          //浮点数
        n=1;                //小数位数
        fudian_flag=0;      //浮点数判断标志位*/
    }
}
}
#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  uchar data=0;
  data=U0RXBUF;
  P6OUT=~data;//接收到的数据存起来
//  Send_Byte(data);                    //将接收到的数据再发送出去
}

//矩阵键盘获取键值
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{ //  uchar fre;
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
            //    P6OUT=~keyval;
                return;
            }
            cmp2<<=1;
        }
        }
    }
    if(flag==0)                               //界面选择;
    {
        switch(keyval)
        {
        case 0:break;
        case 1:panduan=1;flag=1;break;          //设置判断为，判断选择输入的是什么波形
        case 2:panduan=2;flag=1;break;
        case 3:panduan=3;flag=1;break;
        case 4:panduan=4;flag=1;reyi_flag=1;break;
        /*  case 1:Send_Byte(0x11);Send_Byte(0x01); flag=1;break;
       case 2:Send_Byte(0x11);Send_Byte(0x02); flag=1;break;
       case 3:Send_Byte(0x11);Send_Byte(0x03); flag=1;break;
       case 4:Send_Byte(0x11);Send_Byte(0x04);flag=1;reyi_flag=1;break;//输出任意波*/
        case 5:recover++;break;
        default:break;
        }
    }
    else
        switch( keyval)             //根据键值进行操作
        {
        case 0:
            if(fudian_flag==0)
                Cal=Cal*10+0;       //计算整形部分
            else
            {
                xiaoshu+=(0.0)/pow(10,n);//计算浮点部分
                n++;
            }break;
        case 1:if(fudian_flag==0)
            Cal=Cal*10+1;
        else
        {
            xiaoshu+=(1.0)/pow(10,n);
            n++;
        }break;
        case 2:if(fudian_flag==0)
            Cal=Cal*10+2;
        else
        {
            xiaoshu+=(2.0)/pow(10,n);
            n++;
        }break;
        case 3:if(fudian_flag==0)
            Cal=Cal*10+3;
        else
        {
            xiaoshu+=(3.0)/pow(10,n);
            n++;
        }break;
        case 4:if(fudian_flag==0)
            Cal=Cal*10+4;
        else
        {
            xiaoshu+=(4.0)/pow(10,n);
            n++;
        }break;
        case 5:if(fudian_flag==0)
            Cal=Cal*10+5;
        else
        {
            xiaoshu+=(5.0)/pow(10,n);
            n++;
        }break;
        case 6:if(fudian_flag==0)
            Cal=Cal*10+6;
        else
        {
            xiaoshu+=(6.0)/pow(10,n);
            n++;
        }break;
        case 7:if(fudian_flag==0)
            Cal=Cal*10+7;
        else
        {
            xiaoshu+=(7.0)/pow(10,n);
            n++;
        }break;
        case 8:if(fudian_flag==0)
            Cal=Cal*10+8;
        else
        {
            xiaoshu+=(8.0)/pow(10,n);
            n++;
        }break;
        case 9:if(fudian_flag==0)
            Cal=Cal*10+9;
        else
        {
            xiaoshu+=(9.0)/pow(10,n);
            n++;
        }break;
        case 10:dianya+=0.1;                //电压幅值增加
        break;
        case 11:Cal=0;break;                //清除
        case 12:                            //复位
            flag=0;
            Cal=0;
            break;
        case 13:                            //小数点，浮点数判断位
            f_num=Cal;
            Cal=0;
            fudian_flag=1;

            break;
        case 14:dianya-=0.1;               //电压幅值按键减小
        break;
        case 15:                    //整形数据运算
            break;
        }
    }
    KEYIFG=0;                               //中断清零
}

