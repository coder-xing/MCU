/*
 * i2c.c
 *
 *  Created on: 2018年10月22日
 *      Author: liu
 */
#include "i2c.h"
#include "normal.h"
void i2c_start(){
    SDA_OUT;
    SCL_OUT;
    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SDA_L;
    delay_us(10);
    SCL_L;
    delay_us(10);
}
void i2c_stop()
{
    SDA_OUT;
    SCL_OUT;
    SDA_L;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SDA_H;
    delay_us(10);
}
void i2c_send_data(uchar data){
    SDA_OUT;
    SCL_OUT;
    uchar a=0,b=0,cp=0;
    for(a=0;a<8;a++){
        cp=data>>7;
        if(cp&BIT0){
            SDA_H;
        }
        else{
            SDA_L;
        }
        data=data<<1;
        delay_us(10);
        SCL_H;
        delay_us(10);
        SCL_L;
        delay_us(10);
    }
    SDA_H;
    delay_us(10);
    SCL_H;
    SDA_IN;
    while(SDADATA)
    {
        b++;
        if(b>200)
        {
            SCL_L;
            delay_us(10);
            return;
        }
    }
    SCL_L;
    delay_us(10);
}
uchar i2c_read_data()
{   SDA_OUT;
    SCL_OUT;
    uchar dat;
    uchar a=0;
    SDA_H;
    delay_us(10);
    SDA_IN;
    for(a=0;a<8;a++)
    {
        SCL_H;
        delay_us(10);
        dat<<=1;
        if(SDADATA)
        {   dat++;}
        delay_us(10);
        SCL_L;
        delay_us(10);
    }
    return dat;
}
//向器件写入数据
void i2c_write(uchar addr,uchar device,uchar dat){
    i2c_start();
    i2c_send_data(addr);
    i2c_send_data(device);
    i2c_send_data(dat);
    i2c_stop();
}
//读取器件地址以及数据
uchar i2c_read(uchar addr,uchar device){
    uchar num;
    i2c_start();
    i2c_send_data(addr);
    i2c_send_data(device);
    i2c_start();
    i2c_send_data(addr+1);
    num=i2c_read_data();
    i2c_stop();
    return num;
}
