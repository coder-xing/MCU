/*
 * i2c.c
 *
 *  Created on: 2018年10月14日
 *      Author: liu
 */
#include "i2c.h"
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

void i2c_stop(){
    SDA_OUT;
    SCL_OUT;
    SDA_L;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SDA_H;
    delay_us(10);
}

void i2c_ans(){
    uchar i=0;
    SDA_OUT;
    SCL_OUT;
    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
  //  while((SDADATA)&&(i<200))i++;
    SCL_L;
    delay_us(10);

}

void i2c_unans(){
    SDA_OUT;
    SCL_OUT;
    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SCL_L;
    delay_us(10);
}

 void Send_data(uchar data){
       SDA_OUT;
       SCL_OUT;
       uchar count;
       SCL_L;
       for(count=0;count<8;count++){
           if(data&0x80)
               SDA_H;//最高位为1
           else
               SDA_L;//最高位位0
           data=data<<1;
           delay_us(10);
           SCL_H;
           delay_us(10);
           SCL_L;
           delay_us(10);
       }
       SDA_H;
       delay_us(10);
       SDA_H;
       delay_us(10);
}

uchar read_data(){
       uchar i,dat=0;
       SDA_OUT;
       SCL_OUT;
//       SCL_L;
 //      delay_us(10);
       SDA_H;
       SDA_IN;
       delay_us(10);
       for(i=0;i<8;i++){
           SCL_H;
           delay_us(10);
           dat<<=1;
           if(SDADATA){
               dat++;
           }
           SCL_L;
           delay_us(10);
       }
       return dat;
}

/*uchar read_data(){
    SDA_OUT;
    SCL_OUT;
    unsigned char a=0,dat=0;
        SDA_H;
        delay_us(10);
        for(a=0;a<8;a++)
        {
            SCL_H;
            delay_us(10);
            dat<<=1;
            dat|=SDA;
            delay_us(10);
            SCL=0;
            delay_us(10);
        }
        return dat;
}
*/

