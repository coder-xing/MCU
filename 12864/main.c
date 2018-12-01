#include <msp430f169.h>
#include "L12864.h"

void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
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
//      ������
//***********************************************************************
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  Port_Init();                                  //ϵͳ��ʼ��������IO������
  delay_ms(100);                                //��ʱ100ms
  LCD_init();                                   //Һ��������ʼ������
  LCD_clear();                                  //����
  delay_ms(5);
  draw_picture();
  Write_num(0x88,5);
  while(1)
  {
 //     Write_num(0x88,5);
 //     Write_num(0x81,6);
  }
}
