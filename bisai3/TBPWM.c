/*
 * TAPWM.c
 *
 *  Created on: 2018��11��14��
 *      Author: Jack
 */
#include"TBPWM.h"
extern unsigned long fre_const;
extern unsigned long tmp;
extern unsigned char boxing_index;

/*
 * ���ƣ�TBPWM_init()
 * ���ܣ���ʼ����ʱ��A pwm
 * ���Σ� 1��Clk�� 1-->ACLK, 2-->SMCLK, 3-->TACLK, 4-->TACLKȡ��
 *       2��Div: 1/2/4/8��Ƶ
 *       3��Mode1: P4.2PWM���ģʽѡ�� 1-->OUTMOD_7(�ߵ�ƽ���), 2-->OUTMOD_3(�͵�ƽ���)��
 *       4��Mode2: P4.3PWM���ģʽѡ�� 1-->OUTMOD_7(�ߵ�ƽ���), 2-->OUTMOD_3(�͵�ƽ���)
 */
char TBPWM_init(char Clk, char Div, char Mode1, char Mode2)
{
    TBCTL = 0;            //��λ
    TBCTL |= MC_1;        //TIMERA������ģʽ
    switch(Clk)
    {
    case 1:  TBCTL |= TBSSEL_1; break;     //ACLK
    case 2:  TBCTL |= TBSSEL_2; break;     //SMCLK
    case 3:  TBCTL |= TBSSEL_0; break;     //�ⲿ���루TACLK��
    case 4:  TBCTL |= TBSSEL_0; break;     //�ⲿ���루TACLKȡ����
    default:
        return 0;   //��������
    }
    switch(Div)                            //ѡ���Ƶϵ��
    {
        case 1:   TBCTL|=ID_0; break;      //1
        case 2:   TBCTL|=ID_1; break;      //2
        case 4:   TBCTL|=ID_2; break;      //4
        case 8:   TBCTL|=ID_3; break;      //8
        default :  return(0);              //��������
    }
    switch(Mode1)                          //����PWMͨ��1�����ģʽ��
    {
        case 1:
       {    //�������Ϊ�ߵ�ƽģʽ
            TBCCTL2 = OUTMOD_7;     //�ߵ�ƽPWM���
            P4SEL |= BIT2;          //��P4.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P4DIR |= BIT2;          //��P4.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
       }
       break;
        case 2:
       {    //�������Ϊ�͵�ƽģʽ
            TBCCTL2 = OUTMOD_3;     //�͵�ƽPWM���
            P4SEL |= BIT2;          //��P4.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P4DIR |= BIT2;          //��P4.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
       }
       break;
        case 0:            //�������Ϊ����
            P4SEL &= ~BIT2;         //P4.2�ָ�Ϊ��ͨIO��
            break;
        default :  return(0);       //��������
    }
    switch(Mode2)                   //����PWMͨ��2�����ģʽ��
    {
        case 1:
        {//�������Ϊ�ߵ�ƽģʽ
            TBCCTL2 =OUTMOD_7;      //�ߵ�ƽPWM���
            P4SEL |= BIT3;          //��P4.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P4DIR |= BIT3;          //��P4.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
        }
        break;
        case 2:
        {   //�������Ϊ�͵�ƽģʽ
            TBCCTL3 =OUTMOD_3;      //�͵�ƽPWM���
            P4SEL |= BIT3;          //��P4.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P4DIR |= BIT3;          //��P4.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
        }
        break;
        case 0:
        {   //�������Ϊ����
            P4SEL &= ~BIT3;         //P4.3�ָ�Ϊ��ͨIO��
        }
        break;
        default :  return(0);       //��������
    }

    return 1;
}

//�������ں���������PWM���ε����ڣ���λ�Ƕ��ٸ�TACLK����
void TBPWM_set_period(unsigned int Period)
{
    TBCCR0 = Period;
}

//���ø�ͨ����ռ�ձȣ�����TimerB��PWM�������Ч��ƽ�ĳ���ʱ��
void TBPWM_set_duty(char Channel, unsigned int Duty)
{
    switch(Channel)
    {
        case 1: TBCCR2=Duty; break;
        case 2: TBCCR3=Duty; break;
        default: break;
    }

    return;
}
/*
 * ����ռ�ձȣ���ǧ�ֱ����ã�
 * ��ڲ�����Channel: ��ǰ���õ�ͨ����  1��2
 *          Percent: PWM��Чʱ���ǧ�ֱ� (0~1000)
 * ���ڲ�������
 *   ˵   ��: 1000=100.0%  500=50.0% ����������
 *   ��   ��: TBPWM_set_permill(1,300)����PWMͨ��1������ռ�ձ�Ϊ30.0%
 *          TBPWM_set_permill(2,825)����PWMͨ��2������ռ�ձ�Ϊ82.5%
 */

void TBPWM_set_permill(char Channel, unsigned int Percent)
{
    unsigned long int Period;
    unsigned int Duty;
    Period = TBCCR0;
    Duty = Period * Percent / 1000;
    TBPWM_set_duty(Channel,Duty);

    return;
}

void TBPWM_set_period_duty(unsigned int Period, unsigned long dianya)
{
    TBPWM_set_period(Period);
    TBPWM_set_permill(1, dianya/5*1000);  //����ѹΪ2V����PWM����Ϊ2/5*1000=400-->40%��ռ�ձ�
}

//#pragma vector=TIMERB0_VECTOR
//__interrupt void Timer_B(void)
//{
//    tmp++;
//    if(tmp = fre_const)
//    {
//        boxing_index++;
//        tmp++;
//    }
//}





