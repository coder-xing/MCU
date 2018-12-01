#include <msp430f169.h>
unsigned char a[4]={0x01,0x02,0x03,0x04};
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
void Clock_Init()
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
//д����
void write_SegA (unsigned char value[])
{   unsigned char b;
  char *Flash_ptr;                          // Flash pointer
//  unsigned int i;
  Flash_ptr = (char *) 0x1080;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  for(b=0;b<4;b++){
  *Flash_ptr++ = value[b];
  }                                         // Write value to flash
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}
unsigned char read_SegA(unsigned int address)
{   unsigned char data;
    char *Flash_ptrA;
    Flash_ptrA = (char *) 0x1080;
    Flash_ptrA+=address;
    data=* Flash_ptrA;
    return data;
}
/**
 * main.c
 */
void main(void)
{   unsigned char dat;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	FCTL2 = FWKEY + FSSEL0 + FN0;             // MCLK/2 for Flash Timing Generator
	P6DIR=0xff;
	write_SegA(a);
	Clock_Init();
	delay_ms(1000);
	dat=read_SegA(1);
	while(1)
	    P6OUT=~dat;

}
