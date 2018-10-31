/*
 * i2c.c
 *
 *  Created on: 2018年10月13日
 *      Author: liu
 */
#include <msp430f169.h>
#include "i2c.h"

void i2c_write(unsigned char naddr,unsigned char data)
{
    I2CNDAT =0x02;
    U0CTL |=MST;
    I2CTCTL |=I2CSTT +I2CSTP + I2CTRX;
    while((I2CIFG &TXRDYIFG)==0);//等待发送准备好
    I2CDRB =naddr;   //发送地址

    while((I2CIFG &TXRDYIFG)==0);
    delay_ms(9);
    I2CDRB =data;   // 发送命令

    while((I2CTCTL &I2CSTP)==0x02);//等待停止信号
    delay_ms(9);
}
unsigned char  i2c_read(unsigned char naddr){
    I2CNDAT =0x01;
    unsigned char return_data;
    U0CTL |=MST;//主机模式
    I2CTCTL |= I2CSTT +I2CSTP +I2CTRX;//发送初始化，包括起始信号，停止信号，终止信号
    while((I2CIFG&TXRDYIFG)==0);//等待发送准备好
    I2CDRB =naddr;
    delay_ms(9);
    U0CTL |=MST;     //主机模式
    I2CIFG &=~ARDYIFG ;
    I2CTCTL &=~I2CTRX;        //接收模式
    I2CTCTL =I2CSTT +I2CSTP;  //起始信号和停止信号
    while((I2CIFG&RXRDYIFG)==0);//等待发送准备好
    return_data =I2CDRB;
    delay_ms(9);
    while((I2CTCTL &I2CSTP)==0x02);//等待停止信号
    return return_data;
}

uchar bcd_hex(uchar bcd)         /*bcd码转换为十六进制*/
{
    uchar temp = bcd & 0x0f;
    bcd = bcd >> 4;
    bcd %= 0xf;

    return bcd *10 + temp;
}

uchar hex_bcd(uchar val)        /*十六进制转化为BCD码*/
{
   uchar i,j,k;
   i=val/10;
   j=val;
   k=j+(i<<4);
   return k;

}

