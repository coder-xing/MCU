
#include "i2c.h"
#include "LCD1602.h"
/**
 * main.c
 */
void write_time(uchar time,uchar data){
    i2c_start();
    Send_data(0xd0);
    i2c_ans();
    Send_data(time);
    i2c_ans();
    Send_data(data);
    i2c_ans();
    i2c_stop();
}

uchar read_time(uchar adress){
    uchar data;
    i2c_start();
    Send_data(0xd0);
    i2c_ans();
    Send_data(adress);
    i2c_ans();
    i2c_start();
    Send_data(0xd1);
    i2c_ans();
    data=read_data();
    i2c_stop();
    return data;
}
void port_init(){
    P6DIR |=0xff;
    P6OUT =0x00;
    P5DIR |=BIT0+BIT1;
}

void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}
uchar bcd_hex(uchar bcd){
  uchar hex;
  hex=(bcd>>4)*10;
  hex+=(bcd%16);
  return hex;
}
/*

uchar hex_bcd(uchar val)
{
   uchar i,j,k;
   i=val/10;
   j=val;
   k=j+(i<<4);
   return k;

}*/
void main(void)
{   uchar s;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	port_init();
	Port_init();
//	write_time(0x02,0x04);
	LCD_clear();
	LCD_init();
	while(1){
//	     write_time(0x02,0x05);
         s=read_time(0x00);
         LCD_write_single_char(0,0,(bcd_hex(s)%10)+'0');}
}
