#include <msp430f169.h>
#include "LCD1602.h"
#include<stdio.h>
void TimeA_init();
uint second=0,minute=0,hour=0,count=0,cp=0;
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	clk_init();
	Port_init();
	LCD_init();
	LCD_clear();
	keys_init();
	TACCR0=10000;
	TACTL |= TASSEL1 + TACLR + ID0 + ID1 + MC0 + TAIE;
	uchar *liu="now time is:";
	uchar *start="00:00:00";
	LCD_write_str(0,0,liu);
	LCD_write_str(8,1,start);
	_EINT();
	while(1){
	    if(second>59){
	        minute++;
	        second=0;
	    }
	    if(minute>59){
	        hour++;
	        minute=0;
	    }
	    if(hour>24){
	        second=0;
	        hour=0;
	        minute=0;
	    }
        LCD_write_num(15,1,second%10);
        LCD_write_num(14,1,second/10);
        LCD_write_single_char(13,1,':');
        LCD_write_num(12,1,minute%10);
        LCD_write_num(11,1,minute/10);
        LCD_write_single_char(10,1,':');
        LCD_write_num(9,1,hour%10);
        LCD_write_num(8,1,hour/10);
	}
}
#pragma vector =PORT1_VECTOR
__interrupt void keys_operate(void){
    if((P1IN&BIT0)==0){
        cp++;
        if(cp%2==0){
        TACTL &= 0xfffd;}
        else
            TACTL |= TASSEL1 + TACLR + ID0 + ID1 + MC0 + TAIE;}
    if((P1IN&BIT1)==0){
           second++;
           if(second>=60){
               second=0;
               minute++;
           }
           }
    if((P1IN&BIT2)==0){
           second--;
           if(second<0)
               if(minute>0){
                   minute--;
                   second=0;}
                   else
                       second=0;
           }

    if((P1IN&BIT3)==0){
           second=0;
           hour=0;
           minute=0;}
    P1IFG=0;
}
#pragma vector =TIMERA1_VECTOR
__interrupt void TIme_A(void){

    switch(TAIV)                                  //需要判断中断的类型
    {
    case 2:break;
    case 4:break;
    case 10:count++;break;                         //设置标志位Flag
    }
    if(count==100){
    second++;
    count=0;}

}



