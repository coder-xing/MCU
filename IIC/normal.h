/*
 * normal.h
 *
 *  Created on: 2018��10��22��
 *      Author: liu
 */
/*
 * normal.h
 *
 *  Created on: 2018��10��14��
 *      Author: liu
 */

#ifndef NORMAL_H_
#define NORMAL_H_
#include <msp430f169.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int


#endif /* NORMAL_H_ */
