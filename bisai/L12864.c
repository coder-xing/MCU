/*
 * L12864.c
 *
 *  Created on: 2018��11��16��
 *      Author: liu
 */
#include "L12864.h"
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT5 + BIT6 + BIT7;
  PSB_SET;        //Һ�����ڷ�ʽ
  RST_SET;        //��λ��RST�ø�
}

//***********************************************************************
//  ��ʾ������д�뺯��
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
//  ��ʾ������д�뺯��
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
//  ��ʾ�������ʾ
//***********************************************************************

void LCD_clear(void)
{
  LCD_write_com(0x01);
  delay_ms(5);
}

//***********************************************************************
//�������ƣ�DisplayCgrom(uchar hz)��ʾCGROM��ĺ���
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
//�������ƣ�Display()��ʾ���Խ��
//****************************************************************
void Display(void)
{
  DisplayCgrom(0x80,"��һ��1");
  DisplayCgrom(0x88,"�ڶ���");
  DisplayCgrom(0x90,"������");
  DisplayCgrom(0x98,"������");
}

//***********************************************************************
//  ��ʾ����ʼ������
//***********************************************************************
void LCD_init(void)
{
  LCD_write_com(FUN_MODE);          //��ʾģʽ����
  delay_ms(5);
  LCD_write_com(FUN_MODE);          //��ʾģʽ����
  delay_ms(5);
  LCD_write_com(DISPLAY_ON);            //��ʾ��
  delay_ms(5);
  LCD_write_com(CLEAR_SCREEN);          //����
  delay_ms(5);
}








