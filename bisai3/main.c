#include <msp430f169.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "UART.h"
#include "normal.h"
#include "FLASH.h"
/*���󰴼�*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
unsigned char biaozhunbo[11];
unsigned char suijibo[128];
unsigned char recover=0;
unsigned char panduan;
unsigned char itest = 0;
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
    P3DIR |=BIT0;//���ڲ���һ������
}

void pulse()
{
    P3OUT |=BIT0;
    delay_ms(1);
    P3OUT &=~BIT0;
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

{   uchar fre,rec,fasong=2;
P6DIR=0xff;
P6OUT=0xff;
WDTCTL = WDTPW + WDTHOLD;                     //�رտ��Ź�
Clock_Init();                                 //ϵͳʱ������
keyboard_init();                              //���󰴼���ʼ��
UART_Init();                                  //���ڳ�ʼ��
_EINT();                                     //�����ж�
while(1){
//    P6OUT=FlashReadChar(InfoA+5);
    if(recover==1)                      //�����������5�� ���ϴε����ݶ�ȡ����������
    {   pulse();
        rec=FlashReadChar(InfoA);
        if(rec==0x11)                   //�ж��Ƿ���9���ֽڻ���32���ֽ�
        {  // P6OUT=0x00;
            for(fre=0;fre<10;fre++)
                biaozhunbo[fre]=FlashReadChar(InfoA+fre);   //��ȡ�ֽڲ������鱣��
            for(fre=0;fre<10;fre++)
            {Send_Byte(biaozhunbo[fre]);}       //���ڷ�������
        }
        else if(rec==0x12){
            for(fre=0;fre<128;fre++)
                suijibo[fre]=FlashReadChar(InfoB+fre);
            Send_Byte(0x12);
            for(fre=0;fre<128;fre++)
            {Send_Byte(suijibo[fre]);
            }}
        recover++;
    }
    if(keyval==15){             //ȷ�ϼ�
        FlashErase(InfoA);      //������д��ȥ
        FlashErase(InfoB);
        if(reyi_flag!=1){       //��������Ҳ� ���� ���ǲ���һ�֣�����������㷨
            FlashWriteChar(InfoA,0x11); //�Ƚ����ͱ�־λ�洢
            switch(panduan)
            {
            case 1:FlashWriteChar(InfoA+1,0x01);break;  //�洢��������
            case 2:FlashWriteChar(InfoA+1,0x02);break;
            case 3:FlashWriteChar(InfoA+1,0x03);break;
            }
            for(fre=4;fre>0;fre--){ //���������ݴ洢
                FlashWriteChar(InfoA+fasong,Cal>>(fre-1)*8);
                fasong++;}
            memcpy(byte,&dianya,sizeof(float));//����������ת��Ϊ4���ֽ�����
            for(fre=4;fre>0;fre--){            //�洢����������
                FlashWriteChar(InfoA+fasong,byte[fre-1]);
                fasong++;
            }
            fasong=0;
            switch(panduan)
            {
            case 1: Send_Byte(0x11);Send_Byte(0x01);break;  //���ڷ��Ͳ�������
            case 2: Send_Byte(0x11);Send_Byte(0x02);break;
            case 3: Send_Byte(0x11);Send_Byte(0x03);break;
            }
            for(fre=4;fre>=1;fre--){                        //����Ƶ������ 4���ֽ�
                Send_Byte(Cal>>(fre-1)*8);}
            //            Send_Byte(0x12);
            memcpy(byte,&dianya,sizeof(float));
            for(fre=4;fre>0;fre--)
                Send_Byte(byte[fre-1]);                     //���͸���������
            flag=0;
            panduan=0;}
        //���Ⲩ��
        if(reyi_flag==1)
        { //  Send_Byte(0x12);
        //unsigned char itest = 0;
        //    pulse();
        xiaoshu+=(float)f_num;                             //���㸡��������
        memcpy(byte,&xiaoshu,sizeof(float));                //������������ת��Ϊ4���ֽ�����
        if(cishu<32){
            for(fre=4;fre>0;fre--){
                suijibo[itest]=byte[fre-1];                 //���͸���������
                itest++;}
        }
        //            for(fre=4;fre>0;fre--)
        //                Send_Byte(byte[fre-1]);
        cishu++;
        if(cishu==32)                                       //������ͳ���32�����ݣ��˳��������Ⲩģʽ
        {
            //�洢����
            FlashWriteChar(InfoA,0x12);
            for(itest=0;itest<128;itest++)
                FlashWriteChar(InfoB+itest,suijibo[itest]);
            Send_Byte(0x12);            //��־λ
            for(itest=0;itest<128;itest++)
                Send_Byte(suijibo[itest]);
            flag=0;reyi_flag=0;cishu=0;itest=0;
            //һ���Է���
        }
        }
        f_num=0;
        Cal=0;              //���ڼ����ֵ
        keyval=0;           //��ֵ����
        xiaoshu=0;          //������
        n=1;                //С��λ��
        fudian_flag=0;      //�������жϱ�־λ*/
    }
}
}
#pragma vector=USART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{
  uchar data=0;
  data=U0RXBUF;
  P6OUT=~data;//���յ������ݴ�����
//  Send_Byte(data);                    //�����յ��������ٷ��ͳ�ȥ
}

//������̻�ȡ��ֵ
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{ //  uchar fre;
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
            //    P6OUT=~keyval;
                return;
            }
            cmp2<<=1;
        }
        }
    }
    if(flag==0)                               //����ѡ��;
    {
        switch(keyval)
        {
        case 0:break;
        case 1:panduan=1;flag=1;break;          //�����ж�Ϊ���ж�ѡ���������ʲô����
        case 2:panduan=2;flag=1;break;
        case 3:panduan=3;flag=1;break;
        case 4:panduan=4;flag=1;reyi_flag=1;break;
        /*  case 1:Send_Byte(0x11);Send_Byte(0x01); flag=1;break;
       case 2:Send_Byte(0x11);Send_Byte(0x02); flag=1;break;
       case 3:Send_Byte(0x11);Send_Byte(0x03); flag=1;break;
       case 4:Send_Byte(0x11);Send_Byte(0x04);flag=1;reyi_flag=1;break;//������Ⲩ*/
        case 5:recover++;break;
        default:break;
        }
    }
    else
        switch( keyval)             //���ݼ�ֵ���в���
        {
        case 0:
            if(fudian_flag==0)
                Cal=Cal*10+0;       //�������β���
            else
            {
                xiaoshu+=(0.0)/pow(10,n);//���㸡�㲿��
                n++;
            }break;
        case 1:if(fudian_flag==0)
            Cal=Cal*10+1;
        else
        {
            xiaoshu+=(1.0)/pow(10,n);
            n++;
        }break;
        case 2:if(fudian_flag==0)
            Cal=Cal*10+2;
        else
        {
            xiaoshu+=(2.0)/pow(10,n);
            n++;
        }break;
        case 3:if(fudian_flag==0)
            Cal=Cal*10+3;
        else
        {
            xiaoshu+=(3.0)/pow(10,n);
            n++;
        }break;
        case 4:if(fudian_flag==0)
            Cal=Cal*10+4;
        else
        {
            xiaoshu+=(4.0)/pow(10,n);
            n++;
        }break;
        case 5:if(fudian_flag==0)
            Cal=Cal*10+5;
        else
        {
            xiaoshu+=(5.0)/pow(10,n);
            n++;
        }break;
        case 6:if(fudian_flag==0)
            Cal=Cal*10+6;
        else
        {
            xiaoshu+=(6.0)/pow(10,n);
            n++;
        }break;
        case 7:if(fudian_flag==0)
            Cal=Cal*10+7;
        else
        {
            xiaoshu+=(7.0)/pow(10,n);
            n++;
        }break;
        case 8:if(fudian_flag==0)
            Cal=Cal*10+8;
        else
        {
            xiaoshu+=(8.0)/pow(10,n);
            n++;
        }break;
        case 9:if(fudian_flag==0)
            Cal=Cal*10+9;
        else
        {
            xiaoshu+=(9.0)/pow(10,n);
            n++;
        }break;
        case 10:dianya+=0.1;                //��ѹ��ֵ����
        break;
        case 11:Cal=0;break;                //���
        case 12:                            //��λ
            flag=0;
            Cal=0;
            break;
        case 13:                            //С���㣬�������ж�λ
            f_num=Cal;
            Cal=0;
            fudian_flag=1;

            break;
        case 14:dianya-=0.1;               //��ѹ��ֵ������С
        break;
        case 15:                    //������������
            break;
        }
    }
    KEYIFG=0;                               //�ж�����
}

