/*
 * keyget.c
 *
 *  Created on: 2018Äê10ÔÂ18ÈÕ
 *      Author: liu
 */
#include "keyget.h"
#include<msp430f169.h>
extern unsigned char value;
void key_value(){
    unsigned char a=1,s,count=1;
       unsigned char b=0;
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
               b=1;
               return;
           }
           count++;
       }
       if(b==0){
           P6OUT=0xff;
       }
       P2DIR|=BIT4+BIT5+BIT6+BIT7;
       P2OUT=0xf0;
}





