/*
 * normal.h
 *
 *  Created on: 2018年11月18日
 *      Author: liu
 */

#ifndef NORMAL_H_
#define NORMAL_H_
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
uchar keyval;                       //获取的键盘的值0~15;
ulong Cal=0;                //用于获取输入的值
uchar position=0;                   //用于测试数据输入
uchar flag=0;                       //主菜单标志判断
ulong f_num=0;                      //浮点数整数部分
uchar fudian_flag=0;                //浮点数标志判断
uchar n=1;                            //浮点数位数
float xiaoshu=0;
unsigned char byte[4];                //浮点数数组
uchar reyi_flag=0;
uchar cishu=0;                       //任意波发送次数
float dianya=2;                        //ctl电压




#endif /* NORMAL_H_ */
