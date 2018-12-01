#include <msp430f169.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <TBPWM.h>
#include "UART.h"

/*矩阵按键*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
uchar keyval;                       //获取的键盘的值0~15;
ulong Cal=0,confirm;                //用于获取输入的值
uchar position=0;                   //用于测试数据输入
uchar flag=0;                       //主菜单标志判断
ulong f_num=0;                      //浮点数整数部分
uchar fudian_flag=0;                //浮点数标志判断
uchar n=1;                            //浮点数位数
float xiaoshu=0;
unsigned char byte[4];                //浮点数数组
uchar reyi_flag=0;
uchar cishu=0;                       //任意波发送次数

typedef struct _data{
    unsigned char wavetype;
    unsigned long freq;
    float dianya;
}DATA;

DATA data;
float input_array[32]={0};

void data_init(DATA data);


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
//    TBPWM_init(2, 8, 1, 2);
//    TBPWM_set_period_duty(2000,PWM_ctl);//通道1(P4.2)设置为2000us周期，50%占空比
    _EINT();                                     //开总中断

    data_init(data);   //初始化数据包
    while(1){}
}
//串口接收数据中断
//矩阵键盘获取键值
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{   unsigned char fre=0;
    if((KEYIN & 0x0f)<0x0f)
    {
        delay_ms(15);//消抖
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
        if(flag==0)                               //界面选择;
        {
            switch(keyval)
            {
                case 0:break;
                case 1:Send_Byte(0x01); data.wavetype=1; flag=1;break;
                case 2:Send_Byte(0x02); data.wavetype=2; flag=1;break;
                case 3:Send_Byte(0x03); data.wavetype=3; flag=1;break;
                case 4:Send_Byte(0x04); data.wavetype=4; flag=1;reyi_flag=1;break;//输出任意波
                default:break;
            }
        }
        else
            switch(keyval)
            {
                case 0:
                    if(fudian_flag==0)
                        Cal=Cal*10+0;
                    else
                    {
                        xiaoshu+=(0.0)/pow(10,n);
                        n++;
                    }break;
                case 1:
                    if(fudian_flag==0)
                        Cal=Cal*10+1;
                    else
                    {
                        xiaoshu+=(1.0)/pow(10,n);
                        n++;
                    }break;
                case 2:
                    if(fudian_flag==0)
                        Cal=Cal*10+2;
                    else
                    {
                        xiaoshu+=(2.0)/pow(10,n);
                        n++;
                    }break;
                case 3:
                    if(fudian_flag==0)
                        Cal=Cal*10+3;
                    else
                    {
                        xiaoshu+=(3.0)/pow(10,n);
                        n++;
                    }break;
                case 4:
                    if(fudian_flag==0)
                        Cal=Cal*10+4;
                    else
                    {
                        xiaoshu+=(4.0)/pow(10,n);
                        n++;
                    }break;
                case 5:
                    if(fudian_flag==0)
                        Cal=Cal*10+5;
                    else
                    {
                        xiaoshu+=(5.0)/pow(10,n);
                        n++;
                    }break;
                case 6:
                    if(fudian_flag==0)
                        Cal=Cal*10+6;
                    else
                    {
                        xiaoshu+=(6.0)/pow(10,n);
                        n++;
                    }break;
                case 7:
                    if(fudian_flag==0)
                        Cal=Cal*10+7;
                    else
                    {
                        xiaoshu+=(7.0)/pow(10,n);
                        n++;
                    }break;
                case 8:
                    if(fudian_flag==0)
                        Cal=Cal*10+8;
                    else
                    {
                        xiaoshu+=(8.0)/pow(10,n);
                        n++;
                    }break;
                case 9:
                    if(fudian_flag==0)
                        Cal=Cal*10+9;
                        else
                        {
                            xiaoshu+=(9.0)/pow(10,n);
                            n++;
                        }break;
                case 10:
                    data.dianya+=0.1;
                    if(data.dianya > 4.9)
                        data.dianya = 5.0;
                    break;
                case 11:Cal=0;break;
                case 12:
                    flag=0;
                    Cal=0;
                    break;
                case 13:
                    f_num=Cal;
                    Cal=0;
                    fudian_flag=1;

                    break;
                case 14:
                    data.dianya-=0.1;
                    if(data.dianya < 0.2)
                        data.dianya = 0.2;
                    break;
                case 15:
                    //整形数据运算
                    if(reyi_flag!=1)
                    {
                        for(fre=4;fre>=1;fre--)
                        {
                            Send_Byte(Cal>>(fre-1)*8);
                        }
                        memcpy(byte,&data.dianya,sizeof(float));
                        for(fre=4;fre>0;fre--)
                            Send_Byte(byte[fre-1]);
                        flag=0;
                    }
                    //任意波形
                    if(reyi_flag==1)
                    {
                        xiaoshu+=(float)f_num;
                        memcpy(byte,&xiaoshu,sizeof(float));
                        for(fre=4;fre>0;fre--)
                            Send_Byte(byte[fre-1]);
                        cishu++;
                        if(cishu==31)
                        {
                            flag=0;reyi_flag=0;cishu=0;
                        }
                    }
                    //浮点数运算
                    /*if( fudian_flag==1){
                          Send_Byte(Cal>>(fre-1)*8);
                          xiaoshu+=(float)f_num;
                          memcpy( byte,&xiaoshu,sizeof(float));
                          for(fre=4;fre>0;fre--)
                              Send_Byte(byte[fre-1]);
                     }*/
                    f_num=0;
                    Cal=0;              //用于计算键值
                    keyval=0;           //键值清零
                    xiaoshu=0;          //浮点数
                    n=1;                //小数位数
                    fudian_flag=0;      //浮点数判断标志位
                break;
            }
    }
    KEYIFG=0;
}

void data_init(DATA data)
{
    data.wavetype = 0;
    data.freq = 1000;
    data.dianya = 2;
}
