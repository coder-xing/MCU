/*
 * OLED.h
 *
 *  Created on: 2018Äê11ÔÂ26ÈÕ
 *      Author: liu
 */

#ifndef OLED_H_
#define OLED_H_
#include "I2C.h"
#define Max_Column 128
#define SIZE    16

void OLED_Clear(void);
void oled_init();
void OLED_Set_Pos(unsigned char, unsigned char);
void fonts_display(unsigned char,unsigned,unsigned char);
void OLED_ShowChar(unsigned char,unsigned char,unsigned char);
void OLED_ShowNum(unsigned char,unsigned char,unsigned long,unsigned char,unsigned char);
unsigned long oled_pow(unsigned char,unsigned char);
#endif /* OLED_H_ */
