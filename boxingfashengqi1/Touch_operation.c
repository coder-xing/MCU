/*
 * Touch_operation.c
 *
 *  Created on: 2018年11月30日
 *      Author: liu
 */

/*
 * Touch_operation.c
 *
 *  Created on: 2018年11月28日
 *      Author: liu
 */
#include "Touch_operation.h"
#include <stdio.h>
#include <stdlib.h>
extern unsigned int LCD_X,LCD_Y;
extern unsigned char button;
extern unsigned char Display_flag;
static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void touch_screen()
{
    if(Getpix()==1)
    {
        if((LCD_X>=15)&&(LCD_X<15+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                GUIsquare2pix(15,200,75,260,Red);
                delay_ms(1);
                delay_ms(1);
                delay_ms(1);
                while(PEN==0);
                button=1;
                GUIsquare2pix(15,200,75,260,White);
            }
        }
        else if((LCD_X>=90)&&(LCD_X<90+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                GUIsquare2pix(90,200,150,260,Red);
                delay_ms(3);
                while(PEN==0);
                button=2;
                GUIsquare2pix(90,200,150,260,White);
            }
        }
        else if((LCD_X>=165)&&(LCD_X<165+60))
        {   if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                GUIsquare2pix(165,200,225,260,Red);
                delay_ms(3);
                while(PEN==0);
                button=3;
                GUIsquare2pix(165,200,225,260,White);
            }
        }
    }
}

void touch_screen1()
{
    if(Getpix()==1)
    {
        if((LCD_X>=76)&&(LCD_X<200))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                GUIsquare2pix(15,200,75,260,Red);
                delay_ms(1);
                delay_ms(1);
                delay_ms(1);
                while(PEN==0);
                button=1;
                GUIsquare2pix(15,200,75,260,White);
            }
        }
        else if((LCD_X>=90)&&(LCD_X<90+60))
        {
            if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                GUIsquare2pix(90,200,150,260,Red);
                delay_ms(3);
                while(PEN==0);
                button=2;
                GUIsquare2pix(90,200,150,260,White);
            }
        }
        else if((LCD_X>=165)&&(LCD_X<165+60))
        {   if((LCD_Y>=200)&&(LCD_Y<200+60))
            {
                GUIsquare2pix(165,200,225,260,Red);
                delay_ms(3);
                while(PEN==0);
                button=3;
                GUIsquare2pix(165,200,225,260,White);
            }
        }
    }
}

void num_char(unsigned char x,unsigned char y,double num)
{
    unsigned char i,j;
    unsigned int temp;
    temp=(unsigned int)num;
    for(i=0;i<=4;i++)
    {
        if(temp/(unsigned int)pow(10,4-i)!=0)
        {

            j=5-i;
            break;
        }
    }
    for(i=j;i>0;i--)//将整数部分转换成字符串型
        {
            LCD_PutChar(x,y,table[temp/(unsigned int)pow(10,i-1)],Red,Yellow);
            x+=8;
            temp %=(unsigned int)pow(10,i-1);
        }
            LCD_PutChar(x,y,'.',Red,Yellow);
            x+=8;
            num -=(int)num;
       for(i=i+2;i<5-1;i++)//将小数部分转换成字符串型
       {
           num*=10;
           LCD_PutChar(x,y,table[(unsigned int)num],Red,Yellow);
           x+=8;
           num-=(int)num;
       }
}
void func_choose()
{   if(Display_flag==0)
    {
        touch_screen();
        if(button==1)
        {
    //    button=0;
            CLR_Screen(White);               //用背景色清屏

            Display_flag=1;
        }
        if(button==2)
        {
            button=0;
            CLR_Screen(Black);               //用背景色清屏
            Display_flag=1;
        }
        if(button==3)
        {
            button=0;
            CLR_Screen(Blue);               //用背景色清屏
            Display_flag=1;
        }
    }
    if( Display_flag==1)
    {
        LCD_PutString24(20,100,"频率:",Red,White);
        GUIfull(76,100,200,124,Yellow);
        num_char(76,104,6.2);
        LCD_PutString24(20,150,"幅度:",Red,White);
        GUIfull(76,150,200,174,Yellow);
        Display_flag=2;
    }
}





