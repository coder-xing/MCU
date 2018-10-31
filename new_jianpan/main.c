/****************************************************************************
                          keyboard for MSP430
                          Designed by CUGer����Mr.Wei
                ������ã���ϵͳʱ�Ӹı�ʱ��ע���޸İ���������ʱ
****************************************************************************/
#include<msp430x16x.h>

#define KEY_DIR P1DIR
#define KEY_OUT P1OUT
#define KEY_IN  P1IN
#define KEY_IE  P1IE
#define KEY_IES P1IES
#define KEY_IFG P1IFG

/***************ȫ�ֱ���***************/
unsigned char Key_Val;                              //��ż�ֵ

void CtrlKey(unsigned char sw);                     //���Ƽ��̿���//sw=0�� sw=1��

/*******************************************
�������ƣ�Init_Keypad
��    �ܣ���ʼ��ɨ����̵�IO�˿�
��    ������
����ֵ  ����
********************************************/
void Init_Keypad(void)
{
  KEY_DIR = 0x0f;                                     //P1.0~P1.3����Ϊ���״̬,P1.4~P1.7���� ״̬(����H)
  KEY_OUT  &=0xf0;                                    //P1.0~P1.3���Ϊ�͵�ƽ�����ࣩ

  KEY_IES =0xf0;                                      //P1.4~P1.7�½��ش����ж�
  KEY_IE  =0xf0;                                      //P1.4~P1.7�����ж�
  KEY_IFG= 0;                                         //�жϱ�־��0

  Key_Val = 16;                                        //����ֵ��ʼ��Ϊ��0~15��ֵ����
}

/*******************************************
�������ƣ�Check_Key
��    �ܣ�ɨ����̵�IO�˿ڣ���ü�ֵ
��    ������
����ֵ  ����
********************************************/
//p14\5\6\7 ����������
/***************************************
            key_Val ��Ӧ��ֵ
        �У�[p14]  [p15]  [p16]   [p17]
             ��     ��     ��     ��
 �У�
[p13]��      0     1      2      3
[p12]��      4     5      6      7
[p11]��      8     9      10     11
[p10]��     12     13     14     15
***************************************/
void Check_Key(void)
{
  unsigned char hang ,lie,tmp1,tmP6;
  unsigned char keymap[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//���ü����߼���ֵ���������ֵ��ӳ��

  tmp1 = 0x08;
  for(hang = 0;hang < 4;hang++)              //��ɨ��
  {
    KEY_OUT = 0x0f;                       //P1.0~P1.3���ȫ1
    KEY_OUT -= tmp1;                      //P1.0~p1.3�����λ����һ��Ϊ0
    tmp1 >>=1;
    if((KEY_IN & 0xf0)<0xf0)              //�Ƿ�P1IN��P1.4~P1.7����һλΪ0
    {
      tmP6 = 0x10;                        // tmP6���ڼ�����һλΪ0
      for(lie = 0;lie < 4;lie++)          // �м��
      {
        if((KEY_IN & tmP6) == 0x00)       // �Ƿ��Ǹ���,����0Ϊ��
        {
          Key_Val = keymap[hang*4 + lie];  // ��ȡ��ֵ
          return;                         // �˳�ѭ��
        }
        tmP6 <<= 1;                       // tmP6����1λ
      }
    }
  }
}

/*******************************************
�������ƣ�delay
��    �ܣ���ʱԼ15ms�������������
��    ������
����ֵ  ��t= tmp*5*clk ����ʹ��ʱ�ӵ���tmpֵ
********************************************/
void delay(void)
{
    unsigned int tmp;

    for(tmp = 12000;tmp > 0;tmp--);
}
/*******************************************
�������ƣ�Key_Event
��    �ܣ���ⰴ��������ȡ��ֵ
��    ������
����ֵ  ����
********************************************/
void Key_Event(void)
{
    unsigned char tmp;

    KEY_OUT =0;                                                             // ����P1OUTȫΪ0���ȴ���������
    tmp = KEY_IN;                                                          // ��ȡ p1IN
    if((tmp & 0xf0) < 0xf0)                                                //����м�����
    {
        delay();                                                            //��������
        Check_Key();                                                        // ����check_Key(),��ȡ��ֵ
        switch(Key_Val)                                                     //P6.0~P6.3�ӷ�������ܣ����Գ�����
        {
        case 0:  P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 1:  P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 2:  P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 3:  P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        case 4:  P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 5:  P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 6:  P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 7:  P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        case 8:  P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 9:  P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 10: P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 11: P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        case 12: P6DIR  |=BIT0;P6OUT |=BIT0; delay();P6OUT ^=BIT0;break;
        case 13: P6DIR  |=BIT1;P6OUT |=BIT1; delay();P6OUT ^=BIT1;break;
        case 14: P6DIR  |=BIT2;P6OUT |=BIT2; delay();P6OUT ^=BIT2;break;
        case 15: P6DIR  |=BIT3;P6OUT |=BIT3; delay();P6OUT ^=BIT3;break;
        default :break;
        }
    }
}


/*********************************************************************
���ƴ򿪻��߹رռ����ж�
SW= 0���رգ�   ELSE����
*********************************************************************/
void CtrlKey(unsigned char sw)
{
 if(sw==0)
   KEY_IE =0;     //�رն˿��ж�
 else
           KEY_IE =0xf0; //�򿪶˿��ж�
}

/*********************************************************************
                      ������
*********************************************************************/

void main()
{
  WDTCTL=WDTPW+WDTHOLD;                    //�رտ��Ź�
  Init_Keypad();                           //���̳�ʼ��
  delay();
  CtrlKey(1);                              //���̴�
  while(1)
    Key_Event();                           //����ɨ�裬��ȡ��ֵ

}

/*�˿�1�����ж�*/
#pragma vector=PORT1_VECTOR
__interrupt void Port(void)
{
  if((KEY_IFG&0xf0)!=0)
  {
    Key_Event();
    if(Key_Val!=16)         //��ֵ��=16�м�����
    {
      CtrlKey(0);         //�ؼ����ж�
    }
  }

  KEY_IFG=0;KEY_OUT=0;   //���жϱ�־
}
