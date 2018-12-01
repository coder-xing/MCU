/*
 * L12864.c
 *
 *  Created on: 2018年11月16日
 *      Author: liu
 */
#include "L12864.h"
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT5 + BIT6 + BIT7;
  PSB_SET;        //液晶并口方式
  RST_SET;        //复位脚RST置高
}

//***********************************************************************
//  显示屏命令写入函数
//***********************************************************************
void LCD_write_com(unsigned char com)
{
  RS_CLR;
  RW_CLR;
  EN_SET;
  DataPort = com;
  delay_ms(5);
  EN_CLR;
}

//***********************************************************************
//  显示屏数据写入函数
//***********************************************************************
void LCD_write_data(unsigned char data)
{
  RS_SET;
  RW_CLR;
  EN_SET;
  DataPort = data;
  delay_ms(5);
  EN_CLR;
}

//***********************************************************************
//  显示屏清空显示
//***********************************************************************

void LCD_clear(void)
{
  LCD_write_com(0x01);
  delay_ms(5);
}

//***********************************************************************
//函数名称：DisplayCgrom(uchar hz)显示CGROM里的汉字
//***********************************************************************
void DisplayCgrom(uchar addr,uchar *hz)
{
  LCD_write_com(addr);
  delay_ms(5);
  while(*hz != '\0')
  {
    LCD_write_data(*hz);
    hz++;
    delay_ms(5);
  }

}
void Write_num(uchar addr,uchar num)
{
    LCD_write_com(addr);
    delay_ms(5);
    LCD_write_data(num+'0');
    delay_ms(5);

}
//****************************************************************
//函数名称：Display()显示测试结果
//****************************************************************
void Display(void)
{
  DisplayCgrom(0x80,"第一行1");
  DisplayCgrom(0x88,"第二行");
  DisplayCgrom(0x90,"第三行");
  DisplayCgrom(0x98,"第四行");
}

//***********************************************************************
//  显示屏初始化函数
//***********************************************************************
void LCD_init(void)
{
  LCD_write_com(FUN_MODE);          //显示模式设置
  delay_ms(5);
  LCD_write_com(FUN_MODE);          //显示模式设置
  delay_ms(5);
  LCD_write_com(DISPLAY_ON);            //显示开
  delay_ms(5);
  LCD_write_com(CLEAR_SCREEN);          //清屏
  delay_ms(5);
}








