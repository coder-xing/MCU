#include <msp430f169.h>
#include "config.h"
#include "chinese.h"
#include "tftset.h"
/*
 * main.c
 */
/*
端口初始化*/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;

  P3SEL=0x00;                    //设置IO口为普通I/O模式
  P3DIR = 0xff;                  //设置IO口方向为输出
  P3OUT = 0x00;                  //初始设置为00,LED灯所在IO口
}
/*LED闪烁一次，可在需要查看程序执行位置时灵活设置，类似断点*/
void LED_Light()
{
    LED8=0x00;                    //点亮LED
    delay_ms(500);
    LED8=0xff;                    //熄灭LED
    delay_ms(500);
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Port_Init();                       //系统初始化，设置IO口属性
	LED_Light();                       //LED点亮一次，如有现象，说明单片机初始化成功
	Clock_Init();
	

}
