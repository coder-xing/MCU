/*
 * TAPWM.c
 *
 *  Created on: 2018年11月14日
 *      Author: Jack
 */
#include"TBPWM.h"
extern unsigned long fre_const;
extern unsigned long tmp;
extern unsigned char boxing_index;

/*
 * 名称：TBPWM_init()
 * 功能：初始化定时器A pwm
 * 传参： 1、Clk： 1-->ACLK, 2-->SMCLK, 3-->TACLK, 4-->TACLK取反
 *       2、Div: 1/2/4/8分频
 *       3、Mode1: P4.2PWM输出模式选择， 1-->OUTMOD_7(高电平输出), 2-->OUTMOD_3(低电平输出)。
 *       4、Mode2: P4.3PWM输出模式选择， 1-->OUTMOD_7(高电平输出), 2-->OUTMOD_3(低电平输出)
 */
char TBPWM_init(char Clk, char Div, char Mode1, char Mode2)
{
    TBCTL = 0;            //复位
    TBCTL |= MC_1;        //TIMERA增计数模式
    switch(Clk)
    {
    case 1:  TBCTL |= TBSSEL_1; break;     //ACLK
    case 2:  TBCTL |= TBSSEL_2; break;     //SMCLK
    case 3:  TBCTL |= TBSSEL_0; break;     //外部输入（TACLK）
    case 4:  TBCTL |= TBSSEL_0; break;     //外部输入（TACLK取反）
    default:
        return 0;   //参数有误
    }
    switch(Div)                            //选择分频系数
    {
        case 1:   TBCTL|=ID_0; break;      //1
        case 2:   TBCTL|=ID_1; break;      //2
        case 4:   TBCTL|=ID_2; break;      //4
        case 8:   TBCTL|=ID_3; break;      //8
        default :  return(0);              //参数有误
    }
    switch(Mode1)                          //设置PWM通道1的输出模式。
    {
        case 1:
       {    //如果设置为高电平模式
            TBCCTL2 = OUTMOD_7;     //高电平PWM输出
            P4SEL |= BIT2;          //从P4.2输出 (不同型号单片机可能不一样)
            P4DIR |= BIT2;          //从P4.2输出 (不同型号单片机可能不一样)
       }
       break;
        case 2:
       {    //如果设置为低电平模式
            TBCCTL2 = OUTMOD_3;     //低电平PWM输出
            P4SEL |= BIT2;          //从P4.2输出 (不同型号单片机可能不一样)
            P4DIR |= BIT2;          //从P4.2输出 (不同型号单片机可能不一样)
       }
       break;
        case 0:            //如果设置为禁用
            P4SEL &= ~BIT2;         //P4.2恢复为普通IO口
            break;
        default :  return(0);       //参数有误
    }
    switch(Mode2)                   //设置PWM通道2的输出模式。
    {
        case 1:
        {//如果设置为高电平模式
            TBCCTL2 =OUTMOD_7;      //高电平PWM输出
            P4SEL |= BIT3;          //从P4.3输出 (不同型号单片机可能不一样)
            P4DIR |= BIT3;          //从P4.3输出 (不同型号单片机可能不一样)
        }
        break;
        case 2:
        {   //如果设置为低电平模式
            TBCCTL3 =OUTMOD_3;      //低电平PWM输出
            P4SEL |= BIT3;          //从P4.3输出 (不同型号单片机可能不一样)
            P4DIR |= BIT3;          //从P4.3输出 (不同型号单片机可能不一样)
        }
        break;
        case 0:
        {   //如果设置为禁用
            P4SEL &= ~BIT3;         //P4.3恢复为普通IO口
        }
        break;
        default :  return(0);       //参数有误
    }

    return 1;
}

//设置周期函数：设置PWM波形的周期，单位是多少个TACLK周期
void TBPWM_set_period(unsigned int Period)
{
    TBCCR0 = Period;
}

//设置各通道的占空比：设置TimerB的PWM输出的有效电平的持续时间
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
 * 设置占空比，用千分比设置：
 * 入口参数：Channel: 当前设置的通道号  1或2
 *          Percent: PWM有效时间的千分比 (0~1000)
 * 出口参数：无
 *   说   明: 1000=100.0%  500=50.0% ，依次类推
 *   范   例: TBPWM_set_permill(1,300)设置PWM通道1方波的占空比为30.0%
 *          TBPWM_set_permill(2,825)设置PWM通道2方波的占空比为82.5%
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
    TBPWM_set_permill(1, dianya/5*1000);  //若电压为2V，则PWM调节为2/5*1000=400-->40%的占空比
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





