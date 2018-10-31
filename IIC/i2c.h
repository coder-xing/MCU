/*
 * i2c.h
 *
 *  Created on: 2018Äê10ÔÂ22ÈÕ
 *      Author: liu
 */
#ifndef I2C_H_
#define I2C_H_
#include "normal.h"
#define SDA_OUT P3DIR|=BIT0
#define SDA_IN P3DIR&=~BIT0
#define SCL_OUT P3DIR|=BIT1
#define SDA_H  P3OUT|=BIT0
#define SDA_L  P3OUT&=~BIT0
#define SCL_H  P3OUT|=BIT1
#define SCL_L  P3OUT&=~BIT1
#define SDADATA (P3IN &BIT0)
void i2c_start();
void i2c_stop();
//void i2c_ans();
//void i2c_unans();
void i2c_send_data(uchar Data);
uchar i2c_read_data();
void i2c_write(uchar,uchar,uchar);
uchar i2c_read(uchar,uchar);
#endif /* I2C_H_ */

