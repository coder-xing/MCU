#include <msp430f169.h>
#include "config.h"
#include "chinese.h"
#include "tftset.h"
/*
 * main.c
 */
/*
�˿ڳ�ʼ��*/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;

  P3SEL=0x00;                    //����IO��Ϊ��ͨI/Oģʽ
  P3DIR = 0xff;                  //����IO�ڷ���Ϊ���
  P3OUT = 0x00;                  //��ʼ����Ϊ00,LED������IO��
}
/*LED��˸һ�Σ�������Ҫ�鿴����ִ��λ��ʱ������ã����ƶϵ�*/
void LED_Light()
{
    LED8=0x00;                    //����LED
    delay_ms(500);
    LED8=0xff;                    //Ϩ��LED
    delay_ms(500);
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Port_Init();                       //ϵͳ��ʼ��������IO������
	LED_Light();                       //LED����һ�Σ���������˵����Ƭ����ʼ���ɹ�
	Clock_Init();
	

}
