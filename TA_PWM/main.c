#include <msp430.h> 

char TAPwmInit(char Clk,char Div,char Mode1,char Mode2)
{
    TACTL = 0;                  //清除以前设置
    TACTL |= MC_1;              //定时器TA设为增计数模式
    switch(Clk)                 //选择时钟源
    {
        case 'A': case 'a':  TACTL|=TASSEL_1; break;    //ACLK
        case 'S': case 's':  TACTL|=TASSEL_2; break;    //SMCLK
        case 'E':            TACTL|=TASSEL_0; break;    //外部输入(TACLK)
        case 'e':            TACTL|=TASSEL_3; break;    //外部输入(TACLK取反)
        default :  return(0);                           //参数有误
    }
    switch(Div)                 //选择分频系数
    {
        case 1:   TACTL|=ID_0; break;   //1
        case 2:   TACTL|=ID_1; break;   //2
        case 4:   TACTL|=ID_2; break;   //4
        case 8:   TACTL|=ID_3; break;   //8
        default :  return(0);           //参数有误
    }
    switch(Mode1)               //设置PWM通道1的输出模式。
    {
        case 'P':case 'p':          //如果设置为高电平模式
            TACCTL1 = OUTMOD_7;     //高电平PWM输出
            P1SEL |= BIT6;          //从P1.2输出 (不同型号单片机可能不一样)
            P1DIR |= BIT6;          //从P1.2输出 (不同型号单片机可能不一样)
            break;
        case 'N':case 'n':          //如果设置为低电平模式
            TACCTL1 = OUTMOD_3;     //低电平PWM输出
            P1SEL |= BIT6;          //从P1.2输出 (不同型号单片机可能不一样)
            P1DIR |= BIT6;          //从P1.2输出 (不同型号单片机可能不一样)
            break;
        case '0':case 0:            //如果设置为禁用
            P1SEL &= ~BIT6;         //P1.2恢复为普通IO口
            break;
        default :  return(0);       //参数有误
    }
    switch(Mode2)                   //设置PWM通道1的输出模式。
    {
        case 'P':case 'p':          //如果设置为高电平模式
            TACCTL2 =OUTMOD_7;      //高电平PWM输出
            P1SEL |= BIT7;          //从P1.3输出 (不同型号单片机可能不一样)
            P1DIR |= BIT7;          //从P1.3输出 (不同型号单片机可能不一样)
            break;
        case 'N':case 'n':          //如果设置为低电平模式
            TACCTL2 =OUTMOD_3;      //低电平PWM输出
            P1SEL |= BIT7;          //从P1.3输出 (不同型号单片机可能不一样)
            P1DIR |= BIT7;          //从P1.3输出 (不同型号单片机可能不一样)
            break;
        case '0':case 0:            //如果设置为禁用
            P1SEL &= ~BIT7;         //P1.3恢复为普通IO口
            break;
        default :  return(0);       //参数有误
    }
    return(1);
}

void TAPwmSetPeriod(unsigned int Period)
{
    TACCR0 = Period;
}
void TAPwmSetDuty(char Channel,unsigned int Duty)
{
    switch(Channel)
    {
        case 1: TACCR1=Duty; break;
        case 2: TACCR2=Duty; break;
    }
}

void TAPwmSetPermill(char Channel,unsigned int Percent)
{
    unsigned long int Period;
    unsigned int Duty;
    Period = TACCR0;
    Duty = Period * Percent / 1000;
    TAPwmSetDuty(Channel,Duty);
}
void ClkInit(){
    unsigned char i;
    BCSCTL1&=~XT2OFF;//打开XT2振荡器；
    BCSCTL2 |=SELS+SELM1 ;//SMCLK8分频
    do
    {
        IFG1 &= ~OFIFG;//清除振荡错误标志
        for(i=0;i<0xff;i++) _NOP();

    }
    while((IFG1 & OFIFG)!=0);
    IFG1&=~OFIFG;//清除中断标志
}

void Clk_Init()
{
  unsigned char i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}
void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    ClkInit();

    TAPwmInit('A',1,'P','P');   //将定时器TA初始化成为PWM发生器
                  //时钟源=ACLK ; 无分频;  通道1和通道2均设为高电平模式。
    TAPwmSetPeriod(500);        //通道1/2的PWM方波周期均设为500个时钟周期
    TAPwmSetDuty(1,200);        //1通道 有效200个时钟周期
    TAPwmSetPermill(2,200);     //2通道 20.0%
    LPM0;
}

