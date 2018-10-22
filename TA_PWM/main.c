#include <msp430.h> 

char TAPwmInit(char Clk,char Div,char Mode1,char Mode2)
{
    TACTL = 0;                  //�����ǰ����
    TACTL |= MC_1;              //��ʱ��TA��Ϊ������ģʽ
    switch(Clk)                 //ѡ��ʱ��Դ
    {
        case 'A': case 'a':  TACTL|=TASSEL_1; break;    //ACLK
        case 'S': case 's':  TACTL|=TASSEL_2; break;    //SMCLK
        case 'E':            TACTL|=TASSEL_0; break;    //�ⲿ����(TACLK)
        case 'e':            TACTL|=TASSEL_3; break;    //�ⲿ����(TACLKȡ��)
        default :  return(0);                           //��������
    }
    switch(Div)                 //ѡ���Ƶϵ��
    {
        case 1:   TACTL|=ID_0; break;   //1
        case 2:   TACTL|=ID_1; break;   //2
        case 4:   TACTL|=ID_2; break;   //4
        case 8:   TACTL|=ID_3; break;   //8
        default :  return(0);           //��������
    }
    switch(Mode1)               //����PWMͨ��1�����ģʽ��
    {
        case 'P':case 'p':          //�������Ϊ�ߵ�ƽģʽ
            TACCTL1 = OUTMOD_7;     //�ߵ�ƽPWM���
            P1SEL |= BIT6;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P1DIR |= BIT6;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            break;
        case 'N':case 'n':          //�������Ϊ�͵�ƽģʽ
            TACCTL1 = OUTMOD_3;     //�͵�ƽPWM���
            P1SEL |= BIT6;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P1DIR |= BIT6;          //��P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            break;
        case '0':case 0:            //�������Ϊ����
            P1SEL &= ~BIT6;         //P1.2�ָ�Ϊ��ͨIO��
            break;
        default :  return(0);       //��������
    }
    switch(Mode2)                   //����PWMͨ��1�����ģʽ��
    {
        case 'P':case 'p':          //�������Ϊ�ߵ�ƽģʽ
            TACCTL2 =OUTMOD_7;      //�ߵ�ƽPWM���
            P1SEL |= BIT7;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P1DIR |= BIT7;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            break;
        case 'N':case 'n':          //�������Ϊ�͵�ƽģʽ
            TACCTL2 =OUTMOD_3;      //�͵�ƽPWM���
            P1SEL |= BIT7;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            P1DIR |= BIT7;          //��P1.3��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
            break;
        case '0':case 0:            //�������Ϊ����
            P1SEL &= ~BIT7;         //P1.3�ָ�Ϊ��ͨIO��
            break;
        default :  return(0);       //��������
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
    BCSCTL1&=~XT2OFF;//��XT2������
    BCSCTL2 |=SELS+SELM1 ;//SMCLK8��Ƶ
    do
    {
        IFG1 &= ~OFIFG;//����񵴴����־
        for(i=0;i<0xff;i++) _NOP();

    }
    while((IFG1 & OFIFG)!=0);
    IFG1&=~OFIFG;//����жϱ�־
}

void Clk_Init()
{
  unsigned char i;
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
void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    ClkInit();

    TAPwmInit('A',1,'P','P');   //����ʱ��TA��ʼ����ΪPWM������
                  //ʱ��Դ=ACLK ; �޷�Ƶ;  ͨ��1��ͨ��2����Ϊ�ߵ�ƽģʽ��
    TAPwmSetPeriod(500);        //ͨ��1/2��PWM�������ھ���Ϊ500��ʱ������
    TAPwmSetDuty(1,200);        //1ͨ�� ��Ч200��ʱ������
    TAPwmSetPermill(2,200);     //2ͨ�� 20.0%
    LPM0;
}

