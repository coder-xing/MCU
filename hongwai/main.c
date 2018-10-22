#include <msp430f169.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define IRIN (P1IN&BIT0)
uchar IRvalue[6];
uchar Time;
void port1_init(){
    P1IFG &=~BIT0;
    P1IE |=BIT0;
    P1IES |=BIT0;
    P1DIR &=~BIT0;
    P6DIR =0xff;
    _EINT();
}
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
/**
 * main.c
 */
void main(void)
{   port1_init();
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Clock_Init();
	while(1){
//	    P6OUT=~IRvalue[2];
//	    delay_ms(1000);
	    P6OUT=IRvalue[2];
//	    delay_ms(1000);
	}
}

#pragma vector =PORT1_VECTOR
__interrupt void port1_hongwai(void){

    uint err;
    uchar k,j;
    Time=0;
    delay_ms(7);
    if(IRIN==0x00){
        err=1000;
        while((IRIN==0)&&(err>0)){
            delay_us(10);
            err--;
        }
        if(IRIN!=0){
            err=500;
            while((IRIN!=0)&&(err>0))
            {
                delay_us(10);
                err--;

            }
            for(k=0;k<4;k++)
            {
                for(j=0;j<8;j++){
                    err=60;
                    while((IRIN==0)&&err>0)
                    {
                        delay_us(10);
                        err--;
                    }
                    err=500;
                    while((IRIN!=0)&&(err>0))
                    {
                        delay_us(100);
                        Time++;
                        err--;
                        if(Time>30){
                            return;

                        }
                    }
                    IRvalue[k]>>=1;
                    if(Time>=8)
                    {
                        IRvalue[k]|=0x80;
//                        P6OUT=0x00;
                    }
                    Time=0;
                }
            }
        }
        if(IRvalue[2]!=~IRvalue[3])
        {
            return;
        }

    }
    P1IFG=0x00;
}
