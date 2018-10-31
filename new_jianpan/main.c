/****************************************************************************
                          keyboard for MSP430
                          Designed by CUGer——Mr.Wei
                程序可用，当系统时钟改变时，注意修改按键消抖延时
****************************************************************************/
#include<msp430x16x.h>

#define KEY_DIR P1DIR
#define KEY_OUT P1OUT
#define KEY_IN  P1IN
#define KEY_IE  P1IE
#define KEY_IES P1IES
#define KEY_IFG P1IFG

/***************全局变量***************/
unsigned char Key_Val;                              //存放键值

void CtrlKey(unsigned char sw);                     //控制键盘开关//sw=0关 sw=1开

/*******************************************
函数名称：Init_Keypad
功    能：初始化扫描键盘的IO端口
参    数：无
返回值  ：无
********************************************/
void Init_Keypad(void)
{
  KEY_DIR = 0x0f;                                     //P1.0~P1.3设置为输出状态,P1.4~P1.7输入 状态(上拉H)
  KEY_OUT  &=0xf0;                                    //P1.0~P1.3输出为低电平（卫编）

  KEY_IES =0xf0;                                      //P1.4~P1.7下降沿触发中断
  KEY_IE  =0xf0;                                      //P1.4~P1.7允许中断
  KEY_IFG= 0;                                         //中断标志清0

  Key_Val = 16;                                        //按键值初始设为非0~15的值均可
}

/*******************************************
函数名称：Check_Key
功    能：扫描键盘的IO端口，获得键值
参    数：无
返回值  ：无
********************************************/
//p14\5\6\7 接上拉电阻
/***************************************
            key_Val 对应键值
        列：[p14]  [p15]  [p16]   [p17]
             ↓     ↓     ↓     ↓
 行：
[p13]→      0     1      2      3
[p12]→      4     5      6      7
[p11]→      8     9      10     11
[p10]→     12     13     14     15
***************************************/
void Check_Key(void)
{
  unsigned char hang ,lie,tmp1,tmP6;
  unsigned char keymap[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//设置键盘逻辑键值与程序计算键值的映射

  tmp1 = 0x08;
  for(hang = 0;hang < 4;hang++)              //行扫描
  {
    KEY_OUT = 0x0f;                       //P1.0~P1.3输出全1
    KEY_OUT -= tmp1;                      //P1.0~p1.3输出四位中有一个为0
    tmp1 >>=1;
    if((KEY_IN & 0xf0)<0xf0)              //是否P1IN的P1.4~P1.7中有一位为0
    {
      tmP6 = 0x10;                        // tmP6用于检测出哪一位为0
      for(lie = 0;lie < 4;lie++)          // 列检测
      {
        if((KEY_IN & tmP6) == 0x00)       // 是否是该列,等于0为是
        {
          Key_Val = keymap[hang*4 + lie];  // 获取键值
          return;                         // 退出循环
        }
        tmP6 <<= 1;                       // tmP6右移1位
      }
    }
  }
}

/*******************************************
函数名称：delay
功    能：延时约15ms，完成消抖功能
参    数：无
返回值  ：t= tmp*5*clk 根据使用时钟调整tmp值
********************************************/
void delay(void)
{
    unsigned int tmp;

    for(tmp = 12000;tmp > 0;tmp--);
}
/*******************************************
函数名称：Key_Event
功    能：检测按键，并获取键值
参    数：无
返回值  ：无
********************************************/
void Key_Event(void)
{
    unsigned char tmp;

    KEY_OUT =0;                                                             // 设置P1OUT全为0，等待按键输入
    tmp = KEY_IN;                                                          // 获取 p1IN
    if((tmp & 0xf0) < 0xf0)                                                //如果有键按下
    {
        delay();                                                            //消除抖动
        Check_Key();                                                        // 调用check_Key(),获取键值
        switch(Key_Val)                                                     //P6.0~P6.3接发光二极管，测试程序用
        {
        case 0:  P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 1:  P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 2:  P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 3:  P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        case 4:  P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 5:  P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 6:  P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 7:  P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        case 8:  P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 9:  P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 10: P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 11: P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        case 12: P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 13: P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 14: P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 15: P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        default :break;
        }
    }
}


/*********************************************************************
控制打开或者关闭键盘中断
SW= 0：关闭；   ELSE：打开
*********************************************************************/
void CtrlKey(unsigned char sw)
{
 if(sw==0)
   KEY_IE =0;     //关闭端口中断
 else
           KEY_IE =0xf0; //打开端口中断
}

/*********************************************************************
                      主函数
*********************************************************************/

void main()
{
  WDTCTL=WDTPW+WDTHOLD;                    //关闭看门狗
  Init_Keypad();                           //键盘初始化
  delay();
  CtrlKey(1);                              //键盘打开
  while(1)
    Key_Event();                           //不断扫描，获取键值

}

/*端口1按键中断*/
#pragma vector=PORT1_VECTOR
__interrupt void Port(void)
{
  if((KEY_IFG&0xf0)!=0)
  {
    Key_Event();
    if(Key_Val!=16)         //键值！=16有键按下
    {
      CtrlKey(0);         //关键盘中断
    }
  }

  KEY_IFG=0;KEY_OUT=0;   //清中断标志
}
