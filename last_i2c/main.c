#include <msp430x16x.h>

#define SDA_IN            P5DIR &=~BIT0    // P5.0 IN
#define SDA_OUT           P5DIR |=BIT0     // P5.0 OUT

#define SDA_LOW           P5OUT &=~BIT0   // sda=0
#define SDA_HIGH          P5OUT |=BIT0   // sda=1

#define SCL_IN            P5DIR &=~BIT1    // P5.1 IN
#define SCL_OUT           P5DIR |=BIT1    // P5.1 OUT

#define SCL_LOW           P5OUT &=~BIT1
#define SCL_HIGH          P5OUT |=BIT1
#define W_EEPROM_LENGH    14


#define   TURE                1
#define   FALSE               0
#define   AckError            0x55
#define   OutOfRang           0xaa
#define   OutOfAddr           0xbb

unsigned char  dat[W_EEPROM_LENGH]={1,2,3,4,5,6,7,8,9,10,11,12,13,14};
unsigned char    x[W_EEPROM_LENGH];

void i2c_delay(unsigned char us);
void i2c_delay_ms(unsigned char ms);
void i2c_start();
void i2c_stop(void);
void i2c_SendAck(void);
void i2c_SendNoAck(void);
unsigned char i2c_check_ACK(void);
void i2c_SendByte(unsigned char data);
unsigned char i2c_RevByte(void);
unsigned char EEPROM_ByteWrite(unsigned int addr,unsigned char data);
unsigned char EEPROM_RandomRead(unsigned int addr);
unsigned char EEPROM_SequentialRead(unsigned int addr,unsigned int n,unsigned char* p);
unsigned int EEPROM_PageWrite(unsigned int page,unsigned char* p,unsigned char n);

void i2c_delay(unsigned char us)
{
unsigned char tmp;
while(us--)
{
for(tmp=0;tmp<4;tmp++)
{
_NOP();
}
}
}

void i2c_delay_ms(unsigned char ms)
{
 unsigned int tmp;
 while(ms--)
 {
 for(tmp=0;tmp<5000;tmp++)
 {
  _NOP();
  }
 }

}

void i2c_start(void)
{
SDA_OUT;
i2c_delay(20);
SDA_HIGH;
i2c_delay(3);
SCL_HIGH;
i2c_delay(2);
SDA_LOW;
i2c_delay(5);
SCL_LOW;
i2c_delay(10);
}

void i2c_stop(void)
{
SDA_OUT;
SDA_LOW;
i2c_delay(2);
SCL_HIGH;
i2c_delay(2);
SDA_LOW;
i2c_delay(2);
SDA_HIGH;
}

void i2c_SendAck(void)
{
SDA_OUT;
SDA_LOW;
i2c_delay(2);
SCL_LOW;
i2c_delay(2);
SCL_HIGH;
i2c_delay(2);
SCL_LOW;
SDA_HIGH;
}

void i2c_SendNoAck(void)
{
SDA_OUT;
SDA_HIGH;
i2c_delay(2);
SCL_LOW;
i2c_delay(2);
SCL_HIGH;
i2c_delay(2);
SCL_LOW;
}

unsigned char i2c_check_ACK(void)
{
unsigned char AckStatus;
SDA_IN;
SCL_HIGH;
i2c_delay(2);
if(P5IN & 0x01)
{
AckStatus = FALSE;
}
else
{
AckStatus = TURE;
}
SCL_LOW;
i2c_delay(2);
SDA_OUT;
return AckStatus;
}


void i2c_SendByte(unsigned char data)
{
unsigned char tmp;
SDA_OUT;
for(tmp=0;tmp<8;tmp++)
{
if(data & 0x80)
{
SDA_HIGH;
}
else
{
SDA_LOW;
}
i2c_delay(2);
SCL_HIGH;
i2c_delay(2);
SCL_LOW;
i2c_delay(2);
data <<= 1;
}
i2c_delay(15);
}

unsigned char i2c_RevByte(void)
{
 unsigned char tmp;
 unsigned char DATA=0;
 SDA_IN;
 SCL_LOW;
 i2c_delay(2);
 for(tmp=0;tmp<8;tmp++)
 {
  SCL_HIGH;
  i2c_delay(2);
  DATA <<= 1;
  if(P5IN & 0x01)
  {
   DATA |= 0x01;
  }
  else
  {
   DATA &= 0xfe;
  }
  SCL_LOW;
 }
SDA_OUT;
return DATA;
}

unsigned char EEPROM_ByteWrite(unsigned int addr,unsigned char data)
{
unsigned char Dev_addr; //设备地址
unsigned char AddrLow;
unsigned char AddrHigh;
AddrLow = (unsigned char)addr;
AddrHigh = (unsigned char)(addr>>8);
Dev_addr = 0xa0|(AddrHigh<<1);
i2c_start();
i2c_SendByte(Dev_addr);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_SendByte(AddrHigh);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_SendByte(AddrLow);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_SendByte(data);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_stop();
i2c_delay_ms(10);
return 0;
}

unsigned char EEPROM_RandomRead(unsigned int addr)
{
unsigned char Dev_addr; //设备地址
unsigned char AddrLow;
unsigned char AddrHigh;
unsigned char tmp;
AddrLow = (unsigned char)addr;
AddrHigh = (unsigned char)(addr>>8);
Dev_addr = 0xa0|(AddrHigh<<1);
i2c_start();
i2c_SendByte(Dev_addr);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_delay_ms(5);
i2c_SendByte(AddrHigh);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_SendByte(AddrLow);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_delay_ms(5);
i2c_start();
Dev_addr = 0xa1|(AddrHigh<<1);
i2c_SendByte(Dev_addr);
if(i2c_check_ACK() == FALSE)
{
 return AckError;
}
i2c_delay_ms(5);
tmp = i2c_RevByte();
i2c_SendNoAck();
i2c_stop();
i2c_delay(100);
return tmp;
}


unsigned int EEPROM_PageWrite(unsigned int page,unsigned char* p,unsigned char n)
{
unsigned char Dev_addr;
unsigned char AddrLow;
unsigned char AddrHigh;
unsigned int tmp;
if((n > 64)|(page > 512)) //根据读写的设备而变更为适合的页数和每页字节数
{
return OutOfRang;
}
tmp = ((unsigned int)page) << 6; //得出页首地址
AddrLow = (unsigned char)tmp;
AddrHigh = (unsigned char)(tmp>>8);
Dev_addr = 0xa0 | (AddrHigh << 1);
i2c_start();
i2c_SendByte(Dev_addr);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_delay_ms(5);
i2c_SendByte(AddrHigh);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_SendByte(AddrLow);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
while(n--)
{
i2c_SendByte(*p++);
if(i2c_check_ACK() == FALSE)
{
 return AckError;
}
}
i2c_stop();
i2c_delay_ms(10);
return 0;
}

unsigned char EEPROM_SequentialRead(unsigned int addr,unsigned int n,unsigned char* p)
{
unsigned char Dev_addr; //设备地址
unsigned char AddrLow;
unsigned char AddrHigh;
if(n > (32768 - addr)) //检查预写入地址是否有效
{
return OutOfAddr;
}
AddrLow = (unsigned char)addr;
AddrHigh = (unsigned char)(addr>>8);
Dev_addr = 0xa0|(AddrHigh<<1);
i2c_start();
i2c_SendByte(Dev_addr);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_delay_ms(5);
i2c_SendByte(AddrHigh);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_SendByte(AddrLow);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
i2c_delay_ms(5);
i2c_start();
Dev_addr = 0xa1|(AddrHigh<<1);
i2c_SendByte(Dev_addr);
if(i2c_check_ACK() == FALSE)
{
return AckError;
}
while(n--)
{
*p = i2c_RevByte();
p++;
if(n)
i2c_SendAck();
else
i2c_SendNoAck();
}
i2c_stop();
return 0;
}



main()
{
//unsigned char tt,tt1;
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
DCOCTL = 0X73;
BCSCTL1= 0X87; // 设置时钟频率4.00MHz
P5DIR |=0X03;
P5OUT |=0X03;
//EEPROM_ByteWrite(0x0,12);
//tt=EEPROM_RandomRead(0x0);
//tt1=tt;
EEPROM_PageWrite(2,dat,14);
EEPROM_SequentialRead(128,14,x);
}
