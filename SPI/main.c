/*程序功能：实现SPI同步通信 *
 * */
#include <msp430f169.h>
#include "NRF.h"
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
void Clock_Init()
{
  uchar i;
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
/*
void SPI_init(){
    U0CTL|=SWRST;           //软件复位使能
    U0CTL|=SYNC+MM+CHAR;    //选择主机模式+SPI模式 8位数据
    U0TCTL|=SSEL1+STC;//选择了SMCLK作为时钟源，3线SPI模式
    U0BR0=0x02;
    U0BR1=0x00;
    ME1 |= USPIE0;          //USPIE0 USART0 SPI使能
    U0CTL&=~SWRST;          //软件使能
    IE1&=~UTXIE0;           //发送中断使能禁止
    IE1&=~URXIE0;           //接受中断使能禁止
    P3SEL|=0x0e;
}
*/
/**
 * main.c
 */
uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
uchar RX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
uchar sta;
uchar AD_TxBuf[32];
uchar RxBuf[32],temp[6];
//RF24L_01端口设置

void RF24L01_IO_SET()
{
    ME1|=USPIE0;
    UCTL0|=CHAR+SYNC+MM;
    UTCTL0|=CKPH+SSEL1+SSEL0+STC;
    UBR00=0X02;
    UBR10=0X00;
    UMCTL0=0X00;
    UCTL0&=~SWRST;
    P3SEL|=0X0E;
    P3DIR |=(BIT3|BIT1|BIT0);
    P3DIR&=~BIT2;
    P1DIR&=~BIT7;
    P1DIR|=BIT6;
}
//NRF24L21写时序
uchar SPI_RW(uchar data)
{
//    uchar i,temp=0;
    delay_us(3);
    while(!(IFG1 &UTXIFG0));
    delay_us(3);
    TXBUF0=data;
    delay_us(2);
    return(RXBUF0);
}
//NRF24L01的时序
//读SPI寄存器的值
uchar SPI_Read(uchar reg)
{
    uchar reg_val;
    RF24L01_CSN_0;
    SPI_RW(reg);
    reg_val=SPI_RW(0);
    RF24L01_CSN_1;
    return(reg_val);
}
//NRF24L01读写寄存器函数
uchar SPI_RW_Reg(uchar reg,uchar value)
{
    uchar status1;
    RF24L01_CSN_0;
    status1=SPI_RW(reg);
    SPI_RW(value);
    RF24L01_CSN_1;
    return(status1);
}
//NRF24L01用于读数据
//reg:寄存器地址，Pbuf:待读出数据地址，uchars:读出数据个数
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar chars)
{
    uchar status2,uchar_ctr;
    RF24L01_CSN_0;
    status2=SPI_RW(reg);
    for(uchar_ctr=0;uchar_ctr<chars;uchar_ctr++)
    {
        pBuf[uchar_ctr]=SPI_RW(0);
    }
    RF24L01_CSN_1;
    return(status2);
}
//NRF24L01用于写数据
//reg:寄存器地址，pBuf:待写入数据地址，uchars:写入数据个数
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar chars)
{
    uchar status1,uchar_str;
    RF24L01_CSN_0;
    status1 =SPI_RW(reg);
    for(uchar_str=0;uchar_str<chars;uchar_str++)
    {
        SPI_RW(*pBuf++);
    }
    RF24L01_CSN_1;
    return(status1);
}
//数据接收配置
void SetRX_Mode(void)
{
    RF24L01_CE_0;
    SPI_RW_Reg(WRITE_REG+CONFIG,0x0f);
    RF24L01_CE_1;
    delay_us(580);
}
//数据接收后放入rx_buf接收缓冲区中

uchar RF24L01_RxPacket(uchar *rx_buf)
{
    uchar revale=0;
    sta=SPI_Read(STATUS);
    if(sta&0x40)
    {
        RF24L01_CE_0;
        SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
        revale=1;
    }
    SPI_RW_Reg(WRITE_REG+STATUS,sta);
    return revale;
}

void RF24L01_TxPacket(uchar *tx_buf)
{
    RF24L01_CE_0;
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH);
    SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
    RF24L01_CE_1;
    delay_us(10);
}

void init_NRF24L01()
{
    delay_us(10);
    RF24L01_CE_0;
    RF24L01_CSN_1;
    SPI_Write_Buf(WRITE_REG + TX_ADDR,TX_ADDRESS, TX_ADR_WIDTH);
    SPI_Write_Buf(WRITE_REG +RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);
    SPI_RW_Reg(WRITE_REG +EN_AA,0x00);
    SPI_RW_Reg(WRITE_REG +EN_RXADDR,0x01);
    SPI_RW_Reg(WRITE_REG +RF_CH,0);
    SPI_RW_Reg(WRITE_REG +RX_PW_P0,RX_PLOAD_WIDTH);
    SPI_RW_Reg(WRITE_REG+RF_SETUP,0x07);
    SPI_RW_Reg(WRITE_REG+CONFIG,0x0e);
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	//  watchdog timer
	Clock_Init();
	P1DIR =BIT1;
	P5DIR=0xf0;
	RF24L01_IO_SET();
	init_NRF24L01();
	while(1){
	RF24L01_TxPacket("44");
	//TX_BUF[0]="3"
	}
}
