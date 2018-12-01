/*
 * L12864.h
 *
 *  Created on: 2018年11月16日
 *      Author: liu
 */
/*
 * L12864.h
 *
 *  Created on: 2018年11月16日
 *      Author: liu
 */

#ifndef L12864_H_
#define L12864_H_
#include<msp430f169.h>
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define DataDIR         P4DIR                     //数据口方向
#define DataPort        P4OUT                     //P4口为数据口

//12864/1602液晶控制管脚
#define RS_CLR          P5OUT &= ~BIT5           //RS置低
#define RS_SET          P5OUT |=  BIT5           //RS置高

#define RW_CLR          P5OUT &= ~BIT6           //RW置低
#define RW_SET          P5OUT |=  BIT6           //RW置高

#define EN_CLR          P5OUT &= ~BIT7           //E置低
#define EN_SET          P5OUT |=  BIT7           //E置高

#define PSB_CLR         P5OUT &= ~BIT0            //PSB置低，串口方式
#define PSB_SET         P5OUT |=  BIT0            //PSB置高，并口方式

#define RST_CLR         P5OUT &= ~BIT1            //RST置低
#define RST_SET         P5OUT |= BIT1             //RST置高

//12864应用指令集
#define CLEAR_SCREEN    0x01                  //清屏指令：清屏且AC值为00H
#define AC_INIT     0x02                  //将AC设置为00H。且游标移到原点位置
#define CURSE_ADD   0x06                  //设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE    0x30                  //工作模式：8位基本指令集
#define DISPLAY_ON  0x0c                  //显示开,显示游标，且游标位置反白
#define DISPLAY_OFF 0x08                  //显示关
#define CURSE_DIR   0x14                  //游标向右移动:AC=AC+1
#define SET_CG_AC   0x40                  //设置AC，范围为：00H~3FH
#define SET_DD_AC   0x80                      //设置DDRAM AC
#define FUN_MODEK   0x36                  //工作模式：8位扩展指令集
void Port_Init();
void Write_num(unsigned char,unsigned char);
void LCD_write_com(unsigned char);
void LCD_write_data(unsigned char) ;
void LCD_clear(void);
void DisplayCgrom(unsigned char,unsigned char *);
void Display(void);
void LCD_init(void);
#endif /* L12864_H_ */
