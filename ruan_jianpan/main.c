#include <msp430f169.h>
#include "LCD1602.h"
#include "keyget.h"
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
uchar result;
uchar value=0x30;//获取键值
uchar front=0;
uchar back;
uchar fd=0;
uchar symbol;
uchar flag=0;//标志位
uchar coord=0;//LCD1602显示位置
uchar num[]={0x00,0x31,0x32,0x33,0x2b,0x34,0x35,0x36,0x2d,0x37,0x38,0x39,0x2a,0x40,0x30,0x3d,0x2f};
void calcute(){
    if(symbol==4){
        result=front+back;}
    if(symbol==8){
        result=front-back;
    }
    if(symbol==12){
        result=front*back;
    }
    if(symbol==16){
        result=front/back;
    }
}
void key_value1(){
    unsigned char a=1,s,count=1;
    unsigned char compare,wei;
    compare=value;
       for(a=1;a<0x10;a<<=1){
           P2DIR=0xff;
           P2OUT=0x00;
           P2DIR=0x0f;
           P2OUT=a;
           if(P2IN!=a){
            if(0x10&P2IN)
                s=0;
            if(0x20&P2IN)
                s=1;
            if(0x40&P2IN)
                s=2;
            if(0x80&P2IN)
                s=3;
            value=s*4+count;
           }
           count++;
       }
       if(compare==value){
           fd=1;
           return;
       }
       else if(fd==1)
       {   LCD_write_single_char(coord++,1,num[value]);
           if(value!=4&&value!=8&&value!=12&&value!=13&&value!=15&&value!=16){
           value=compare*10+value;}
           else if(value!=15){symbol=value;
               front =compare;
               result= front;
               wei=result/100;
                    LCD_write_single_char(coord++,1,wei+'0');
                    result=result%100;
                    wei=result/10;
                    LCD_write_single_char(coord++,1,wei+'0');
                    result=result%10;
                    LCD_write_single_char(coord++,1,result+'0');
               LCD_write_single_char(10,1,'s');
               value=0;
           }
           else
           { back=compare;
            flag=1;
               }
           fd=0;}
}
void display_answer(){
    uchar wei;
      calcute();
      if(result>100){
      wei=result/100;
      LCD_write_single_char(coord++,1,wei+'0');
      result=result%100;
      wei=result/10;
      LCD_write_single_char(coord++,1,wei+'0');
      result=result%10;
      LCD_write_single_char(coord++,1,result+'0');
}
}
/**
 * main.c
 */
void main(void)
{   P6DIR=0xff;
    P6OUT=0xff;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	clk_init();
	LCD_clear();
	LCD_Port_init();
	LCD_init();
	LCD_write_str(0,0,"result");
	while(1){
	    key_value1();
	    if(flag==1)
	    {  display_answer();
	    flag=0;}
	    P6OUT=~num[value];
	}
}

