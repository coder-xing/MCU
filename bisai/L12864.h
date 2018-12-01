/*
 * L12864.h
 *
 *  Created on: 2018��11��16��
 *      Author: liu
 */
/*
 * L12864.h
 *
 *  Created on: 2018��11��16��
 *      Author: liu
 */

#ifndef L12864_H_
#define L12864_H_
#include<msp430f169.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define DataDIR         P4DIR                     //���ݿڷ���
#define DataPort        P4OUT                     //P4��Ϊ���ݿ�

//12864/1602Һ�����ƹܽ�
#define RS_CLR          P5OUT &= ~BIT5           //RS�õ�
#define RS_SET          P5OUT |=  BIT5           //RS�ø�

#define RW_CLR          P5OUT &= ~BIT6           //RW�õ�
#define RW_SET          P5OUT |=  BIT6           //RW�ø�

#define EN_CLR          P5OUT &= ~BIT7           //E�õ�
#define EN_SET          P5OUT |=  BIT7           //E�ø�

#define PSB_CLR         P5OUT &= ~BIT0            //PSB�õͣ����ڷ�ʽ
#define PSB_SET         P5OUT |=  BIT0            //PSB�øߣ����ڷ�ʽ

#define RST_CLR         P5OUT &= ~BIT1            //RST�õ�
#define RST_SET         P5OUT |= BIT1             //RST�ø�

//12864Ӧ��ָ�
#define CLEAR_SCREEN    0x01                  //����ָ�������ACֵΪ00H
#define AC_INIT     0x02                  //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD   0x06                  //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE    0x30                  //����ģʽ��8λ����ָ�
#define DISPLAY_ON  0x0c                  //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF 0x08                  //��ʾ��
#define CURSE_DIR   0x14                  //�α������ƶ�:AC=AC+1
#define SET_CG_AC   0x40                  //����AC����ΧΪ��00H~3FH
#define SET_DD_AC   0x80                      //����DDRAM AC
#define FUN_MODEK   0x36                  //����ģʽ��8λ��չָ�
void Port_Init();
void Write_num(unsigned char,unsigned char);
void LCD_write_com(unsigned char);
void LCD_write_data(unsigned char) ;
void LCD_clear(void);
void DisplayCgrom(unsigned char,unsigned char *);
void Display(void);
void LCD_init(void);
#endif /* L12864_H_ */
