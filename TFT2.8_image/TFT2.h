/*
 * TFT2.h
 *
 *  Created on: 2018年11月10日
 *      Author: liu
 */

/*
 * TFT2.8.h
 *
 *  Created on: 2018年11月10日
 *      Author: liu
 */
#ifndef TFT2_8_H_
#define TFT2_8_H_
#include "Config.h"
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif
unsigned char pic[];
void TFT_Initial(void);
void show_colour_bar (void);
void Write_Cmd_Data(unsigned char x, unsigned int y);
void Write_Cmd(unsigned char DH,unsigned char DL);
void Write_Data(unsigned char DH,unsigned char DL);
void delayms(unsigned int tt);
//void show_photo(void);
void Show_RGB (unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1,unsigned int Color);
void  Write_Data_U16(unsigned int y);
//Read_RegData(unsigned char Regaddr);
void CLR_Screen(unsigned int bColor);
//void PutGB3232(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor);
//void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);
//void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor);
//void LCD_PutChar8x8(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);
//void Put16x16(unsigned short x, unsigned short  y, unsigned char g[2], unsigned int fColor,unsigned int bColor);
void address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
void line(int x1,int y1,int x2,int y2,int color);
void draw_circle(int x, int y, int r, int color);
void Put_pixel(unsigned int x,unsigned int y,unsigned int color);
void Line(  uchar X0,
            uchar Y0,
            uchar X1,
            uchar Y1,
            unsigned int color);
void Rectangle( uchar left,
                uchar top,
                uchar right,
                uchar bottom,
                unsigned int color);
void Bar(   uchar left,
            uchar top,
            uchar right,
            uchar bottom,
            unsigned int color);
#endif /* TFT2_8_H_ */
