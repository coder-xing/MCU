#include <msp430f169.h>
#include "L12864.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>
/*���󰴼�*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
uchar keyval;                       //��ȡ�ļ��̵�ֵ0~15;
uchar func=0;                //����ȷ������13ѡ��Ĺ���
ulong Cal=0,confirm=0;                //���ڻ�ȡ�����ֵ
uchar position=0;                   //���ڲ�����������
uchar addr=0x80;                    //12864��ʾ��ַ
uchar Lcd_H=0;
uchar flag=0;                       //���˵���־�ж�
ulong confirm;
uchar f_num=0;
uchar fudian=0;
uchar n;//��������־�ж�
ulong fudianshu=0;
float f_cal=0;         //��ȡ������С������
unsigned char byte[4];//���������ݴ�������
/*���󰴼���ʼ��(P1.0~P1.3��������)
 *       P1.7 P1.6 P1.5 P1.4
 * P1.0   0    1    2    3
 * P1.1   4    5    6    7
 * P1.2   8    9    up   clc
 * P1.3   func .    down ok
 */

void keyboard_init(){
   KEYDIR =0xf0;//����λ���������λ����
   KEYOUT =0x00;
   KEYIE=0x0f;//��4λ�ж�����λ
   KEYIES=0x0f;//�½��ش���
   KEYIFG =0x00;//����жϱ�־λ
}

//ʱ�ӳ�ʼ��
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG;
}
void main(void)
{
    P6DIR=0xff;
    P6OUT=0xff;
    WDTCTL = WDTPW + WDTHOLD;                     //�رտ��Ź�
    Clock_Init();                                 //ϵͳʱ������
    keyboard_init();                              //���󰴼���ʼ��
    UART_Init();                                  //���ڳ�ʼ��
    Port_Init();                                  //ϵͳ��ʼ��������IO������
    LCD_init();                                   //Һ��������ʼ������
    LCD_clear();                                  //����
    _EINT();                            //�����ж�
//    LCD_write_str(0,0,"hello");
    while(1){}
}
//���ڽ��������ж�
//������̻�ȡ��ֵ
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{   uchar a;
    unsigned char fre=0;//���������������

 //   unsigned char fudu=0;  �����ѹ����
    if((KEYIN & 0x0f)<0x0f)
    {   delay_ms(15);//����
        uchar line,row,cmp1,cmp2;
        cmp1=0x70;
        for(line=0;line<4;line++)
        {
            KEYOUT=cmp1;
            cmp1>>=1;
            if((KEYIN & 0xf0)<0xf0)
            {   cmp2=0x01;
                for(row=0;row<4;row++)
            {
               if((KEYIN &cmp2)==0)
               {
                   keyval= line+row*4;
                   P6OUT=~keyval;
                   return;
               }
               cmp2<<=1;
           }
       }
   }    if(flag==0)                               //����ѡ��;
        {
       switch(keyval)
       {
       case 0:break;
       case 1:Send_Byte(0x01); flag=1;break;
       case 2:Send_Byte(0x02); flag=1;break;
       case 3:Send_Byte(0x03); flag=1;break;
       default:break;
       }
        }
       else
        switch(keyval)
            {
            case 0:
                if(fudian==0)
                    Cal=Cal*10+0;
                else
                {f_cal+=((float)keyval)/(10^n) ;
                    n++;}
                    break;
            case 1:
                if(fudian==0)
                 Cal=Cal*10+1;
                 else
                 {f_cal+=((float)keyval)/(10^n);
                     n++;}
                  break;
            case 2:Cal=Cal*10+2;break;
            case 3:Cal=Cal*10+3;break;
            case 4:Cal=Cal*10+4;break;
            case 5:if(fudian==0)
                Cal=Cal*10+5;
            else
            {f_cal+=(float)keyval/(10^n) ;
                n++;}
            break;
            case 6:Cal=Cal*10+6;break;
            case 7:Cal=Cal*10+7;break;
            case 8:Cal=Cal*10+8;break;
            case 9:Cal=Cal*10+9;break;
            case 10:break;
            case 11:Cal=0;fudian=0;break;
            case 12:
                flag=0;
                Cal=0;
                fudian=0;
                break;
            case 13:
               fudian=1;
//               P6OUT=0x00;
               fudianshu=Cal;
               Cal=0;
               break;
            case 14:break;
            case 15://���֮ǰ�������ֵ
                   // Cal=0;     //����������ֵ
                    /*    for(fre=4;fre>=1;fre--){
                        Send_Byte(Cal>>(fre-1)*8);}
                        Send_Byte(fudu);
                    if(func==0x04);*/
                    if(fudian==1)
                    {
                        confirm=confirm+f_cal;
                        memcpy( byte,&confirm,sizeof(float));
                        for(a=4;a>0;a--)
                        Send_Byte(byte[a-1]);
                    }
                    for(fre=4;fre>=1;fre--){
                        Send_Byte(fudianshu>>(fre-1)*8);}
                    Cal=0;
                    keyval=0;
                    fudian=0;
                    break;
            }
    }
    KEYIFG=0;
}

