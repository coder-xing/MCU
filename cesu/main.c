/*��Ŀ��飺��*/
#include <msp430f169.h>
#include <stdio.h>
#include <string.h>
#include "LCD1602.h"
//#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
unsigned int time1=0,time2=0;
unsigned int time;
unsigned char byte[4];
unsigned char cishu=0;
unsigned long speed;
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
/***********************************
 * ��������:ʵ�ֶ�ʱ��A��PWM��ʼ��
 * �Ĵ���ѡ��:TACCTL1
 ***********************************/
void TimeA_init(){
    TACTL |=TASSEL_1+MC_2;        //ѡ��SMCLK��Ϊ��ʱ��A��ʱ�ӣ�8��Ƶ����������ģʽ;
    TACCTL1=CM_1+CCIS_0+CAP+SCS+CCIE;   //ѡ�������½��ض�����(CM_1:�����ز���CM_2:�½��ز���)
                                        //����Դ:CCI1A,����ģʽ,ͬ������,�����ж�����
}

void port_init(){
     P1SEL |=BIT2;                      //ѡ��P1.2�ڶ����ܽ�:��ʱ��A����CCI1A����;
     P3DIR |=BIT1;                      //P1.1������Ϊ���ģʽ
     P6DIR=0xff;
     P6OUT=0xff;
     P3OUT =BIT1;
}
/*
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TimeA_init();
    Clock_Init();
    port_init();                        //�˿ڳ�ʼ��
    _EINT();                            //�����ж�
    Port_init();
    LCD_clear();
    LCD_init();
    while(1)
    {

    }

}

#pragma vector =TIMERA1_VECTOR
__interrupt void ceju()
{
    switch(TAIV)
    {
    case 0x02: cishu++;
    P6OUT=~cishu;
        if(time1==0){
            time1=TACCR1;
           }
        else
        {
            time2=TACCR1;
            if(time2>time1){
                time=time2-time1;
            }
            else{
                time=time2+(65535-time1);
                }
            speed=(long)(0.7*3600)/(30*time/1000000.0+'0');
            LCD_write_single_char(1,1,speed/10000+'0');
            LCD_write_single_char(2,1,speed%10000/1000+'0');
            LCD_write_single_char(3,1,speed%10000%1000/100+'0');
            LCD_write_single_char(4,1,speed%10000%1000%100/10+'0');
            LCD_write_si ngle_char(5,1,speed%10000%1000%100%10+'0');
            LCD_write_single_char(1,0,time/10000+'0');
            LCD_write_single_char(2,0,time%10000/1000+'0');
            LCD_write_single_char(3,0,time%1000%1000/100+'0');
            LCD_write_single_char(4,0,time%1000%1000%100/10+'0');
            LCD_write_single_char(5,0,time%1000%1000%100%10+'0');
            delay_ms(600);
            time1=0;
            time2=0;
        }
    break;
    case 0x04:break;
    case 0x10:break;
    }
}


