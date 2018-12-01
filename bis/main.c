#include <msp430f169.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <TBPWM.h>
#include "UART.h"

/*���󰴼�*/
#define KEYDIR P1DIR
#define KEYOUT P1OUT
#define KEYIN P1IN
#define KEYIE P1IE
#define KEYIES P1IES
#define KEYIFG P1IFG
uchar keyval;                       //��ȡ�ļ��̵�ֵ0~15;
ulong Cal=0,confirm;                //���ڻ�ȡ�����ֵ
uchar position=0;                   //���ڲ�����������
uchar flag=0;                       //���˵���־�ж�
ulong f_num=0;                      //��������������
uchar fudian_flag=0;                //��������־�ж�
uchar n=1;                            //������λ��
float xiaoshu=0;
unsigned char byte[4];                //����������
uchar reyi_flag=0;
uchar cishu=0;                       //���Ⲩ���ʹ���

typedef struct _data{
    unsigned char wavetype;
    unsigned long freq;
    float dianya;
}DATA;

DATA data;
float input_array[32]={0};

void data_init(DATA data);


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
//    TBPWM_init(2, 8, 1, 2);
//    TBPWM_set_period_duty(2000,PWM_ctl);//ͨ��1(P4.2)����Ϊ2000us���ڣ�50%ռ�ձ�
    _EINT();                                     //�����ж�

    data_init(data);   //��ʼ�����ݰ�
    while(1){}
}
//���ڽ��������ж�
//������̻�ȡ��ֵ
#pragma vector=PORT1_VECTOR
__interrupt void key_cal(void)
{   unsigned char fre=0;
    if((KEYIN & 0x0f)<0x0f)
    {
        delay_ms(15);//����
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
        }
        if(flag==0)                               //����ѡ��;
        {
            switch(keyval)
            {
                case 0:break;
                case 1:Send_Byte(0x01); data.wavetype=1; flag=1;break;
                case 2:Send_Byte(0x02); data.wavetype=2; flag=1;break;
                case 3:Send_Byte(0x03); data.wavetype=3; flag=1;break;
                case 4:Send_Byte(0x04); data.wavetype=4; flag=1;reyi_flag=1;break;//������Ⲩ
                default:break;
            }
        }
        else
            switch(keyval)
            {
                case 0:
                    if(fudian_flag==0)
                        Cal=Cal*10+0;
                    else
                    {
                        xiaoshu+=(0.0)/pow(10,n);
                        n++;
                    }break;
                case 1:
                    if(fudian_flag==0)
                        Cal=Cal*10+1;
                    else
                    {
                        xiaoshu+=(1.0)/pow(10,n);
                        n++;
                    }break;
                case 2:
                    if(fudian_flag==0)
                        Cal=Cal*10+2;
                    else
                    {
                        xiaoshu+=(2.0)/pow(10,n);
                        n++;
                    }break;
                case 3:
                    if(fudian_flag==0)
                        Cal=Cal*10+3;
                    else
                    {
                        xiaoshu+=(3.0)/pow(10,n);
                        n++;
                    }break;
                case 4:
                    if(fudian_flag==0)
                        Cal=Cal*10+4;
                    else
                    {
                        xiaoshu+=(4.0)/pow(10,n);
                        n++;
                    }break;
                case 5:
                    if(fudian_flag==0)
                        Cal=Cal*10+5;
                    else
                    {
                        xiaoshu+=(5.0)/pow(10,n);
                        n++;
                    }break;
                case 6:
                    if(fudian_flag==0)
                        Cal=Cal*10+6;
                    else
                    {
                        xiaoshu+=(6.0)/pow(10,n);
                        n++;
                    }break;
                case 7:
                    if(fudian_flag==0)
                        Cal=Cal*10+7;
                    else
                    {
                        xiaoshu+=(7.0)/pow(10,n);
                        n++;
                    }break;
                case 8:
                    if(fudian_flag==0)
                        Cal=Cal*10+8;
                    else
                    {
                        xiaoshu+=(8.0)/pow(10,n);
                        n++;
                    }break;
                case 9:
                    if(fudian_flag==0)
                        Cal=Cal*10+9;
                        else
                        {
                            xiaoshu+=(9.0)/pow(10,n);
                            n++;
                        }break;
                case 10:
                    data.dianya+=0.1;
                    if(data.dianya > 4.9)
                        data.dianya = 5.0;
                    break;
                case 11:Cal=0;break;
                case 12:
                    flag=0;
                    Cal=0;
                    break;
                case 13:
                    f_num=Cal;
                    Cal=0;
                    fudian_flag=1;

                    break;
                case 14:
                    data.dianya-=0.1;
                    if(data.dianya < 0.2)
                        data.dianya = 0.2;
                    break;
                case 15:
                    //������������
                    if(reyi_flag!=1)
                    {
                        for(fre=4;fre>=1;fre--)
                        {
                            Send_Byte(Cal>>(fre-1)*8);
                        }
                        memcpy(byte,&data.dianya,sizeof(float));
                        for(fre=4;fre>0;fre--)
                            Send_Byte(byte[fre-1]);
                        flag=0;
                    }
                    //���Ⲩ��
                    if(reyi_flag==1)
                    {
                        xiaoshu+=(float)f_num;
                        memcpy(byte,&xiaoshu,sizeof(float));
                        for(fre=4;fre>0;fre--)
                            Send_Byte(byte[fre-1]);
                        cishu++;
                        if(cishu==31)
                        {
                            flag=0;reyi_flag=0;cishu=0;
                        }
                    }
                    //����������
                    /*if( fudian_flag==1){
                          Send_Byte(Cal>>(fre-1)*8);
                          xiaoshu+=(float)f_num;
                          memcpy( byte,&xiaoshu,sizeof(float));
                          for(fre=4;fre>0;fre--)
                              Send_Byte(byte[fre-1]);
                     }*/
                    f_num=0;
                    Cal=0;              //���ڼ����ֵ
                    keyval=0;           //��ֵ����
                    xiaoshu=0;          //������
                    n=1;                //С��λ��
                    fudian_flag=0;      //�������жϱ�־λ
                break;
            }
    }
    KEYIFG=0;
}

void data_init(DATA data)
{
    data.wavetype = 0;
    data.freq = 1000;
    data.dianya = 2;
}
