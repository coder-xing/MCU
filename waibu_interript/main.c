#include<msp430f169.h>


#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void){
    P6OUT=~P6OUT;
    P1IFG &= 0;
}


void main(){
    WDTCTL = WDTPW + WDTHOLD;
    P6DIR=0xff;
    //P1 init
    P1DIR &= 0;
    P1IES |= BIT0+BIT1; //0:�����أ�    1���½���
    P1IE |= BIT0+BIT2;  //�ж�����1Ϊ����
    P1IFG &= 0;  //�жϱ�־��0Ϊ�ɽ����ж�
    _EINT();    //�������ж�
}
