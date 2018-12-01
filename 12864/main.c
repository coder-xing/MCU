#include <msp430f169.h>
#include "L12864.h"

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

void draw_picture()
{
    unsigned char i,j;
    LCD_write_com(0x34);
    for(i=0;i<8;i++)
    {

        LCD_write_com(0x80+i);
        LCD_write_com(0x80);
        for(j=0;j<16;j++)
        {
            LCD_write_data(0x0f);
        }
    }
    LCD_write_com(0x36);
    LCD_write_com(0x30);
}
//***********************************************************************
//      主程序
//***********************************************************************
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  Port_Init();                                  //系统初始化，设置IO口属性
  delay_ms(100);                                //延时100ms
  LCD_init();                                   //液晶参数初始化设置
  LCD_clear();                                  //清屏
  delay_ms(5);
  draw_picture();
  Write_num(0x88,5);
  while(1)
  {
 //     Write_num(0x88,5);
 //     Write_num(0x81,6);
  }
}
