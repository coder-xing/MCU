/*
 * UART.h
 *
 *  Created on: 2018��11��17��
 *      Author: liu
 */

#ifndef UART_H_
#define UART_H_
#include<msp430f169.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
void UART_Init();
void Send_Byte(unsigned char data);
#endif /* UART_H_ */
