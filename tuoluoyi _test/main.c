#include  <msp430x16x.h>
#include<stdlib.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define SMPLRT_DIV  0x19    //�����ʷ�Ƶ������ֵ��0x07(125Hz) */
#define CONFIG   0x1A       // ��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz) */
#define GYRO_CONFIG  0x1B   // �������Լ켰������Χ������ֵ��0x18(���Լ죬 2000deg/s) */
#define ACCEL_CONFIG 0x1C  // ���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(�� �Լ죬2G��5Hz) */
#define ACCEL_XOUT_H 0x3B  // �洢����� X �ᡢY �ᡢZ ����ٶȸ�Ӧ���Ĳ���ֵ */
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H  0x41   // �洢������¶ȴ������Ĳ���ֵ */
#define TEMP_OUT_L  0x42
#define GYRO_XOUT_H  0x43 // �洢����� X �ᡢY �ᡢZ �������Ǹ�Ӧ���Ĳ���ֵ */
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define PWR_MGMT_1  0x6B // ��Դ��������ֵ��0x00(��������) */
#define WHO_AM_I  0x75 //IIC ��ַ�Ĵ���(Ĭ����ֵ 0x68��ֻ��) */

#define XGH  0x43 // �洢����� X �ᡢY �ᡢZ �������Ǹ�Ӧ���Ĳ���ֵ */
#define XGL  0x44
#define YGH  0x45
#define YGL  0x46
#define ZGH  0x47
#define ZGL  0x48

#define XAH 0x3B  // �洢����� X �ᡢY �ᡢZ ����ٶȸ�Ӧ���Ĳ���ֵ */
#define XAL 0x3C
#define YAH 0x3D
#define YAL 0x3E
#define ZAH 0x3F
#define ZAL 0x40

unsigned char i2cRead(unsigned char addr){
    I2CNDAT=0x01;
    unsigned char ctlbyte;
    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(2);
    U0CTL |= MST;
    I2CIFG &= ~ARDYIFG;
    I2CTCTL &= ~I2CTRX;
    I2CTCTL = I2CSTT+I2CSTP;
    while((I2CIFG&RXRDYIFG)==0);
    ctlbyte=I2CDRB;
    delay_ms(2);
    while((I2CTCTL&I2CSTP)==0x02);
    return ctlbyte;
}

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
unsigned char i2cWrite(unsigned char addr,unsigned char dat){
    //I2CNDAT=2;
    I2CNDAT=0x02;
    //I2CSA=(0xd0>>1);

    U0CTL |= MST;
    I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;
    //I2CNDAT=0x02;
    //P6OUT=~1;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=addr;
    delay_ms(2);
    //I2CNDAT=0x01;
    //P6OUT=~2;
    while((I2CIFG&TXRDYIFG)==0);
    I2CDRB=dat;
    delay_ms(2);
    //P6OUT=~3;
    while((I2CTCTL&I2CSTP)==0x02);
}

void i2c_init(){

    P3SEL|=0x0a;//P3.1   P3.3
    P3DIR&=~0x0a;
    U0CTL|=I2C+SYNC;
    U0CTL&=~I2CEN;
    I2CTCTL=I2CSSEL_2;

    //I2CNDAT=0x01;
    I2CSA=(0xd0>>1);
    U0CTL|=I2CEN;

}

void mpuWrite(unsigned char addr,unsigned char dat){

}

void MPU6050_Init()             //��ʼ������ ����ʵ����д 5���Ĵ���
         {       /*
                 MPU6050_I2C_ByteWrite(0xd0,0x00,MPU6050_RA_PWR_MGMT_1);      // reg107, ���ѣ�8M�ڲ�ʱ��Դ
                 MPU6050_I2C_ByteWrite(0xd0,0x07,MPU6050_RA_SMPLRT_DIV);         //����Ƶ�� 1000
                 MPU6050_I2C_ByteWrite(0xd0,0x06,MPU6050_RA_CONFIG);
                 MPU6050_I2C_ByteWrite(0xd0,0x01,MPU6050_RA_ACCEL_CONFIG);     //���ٶ����� 2g
                 MPU6050_I2C_ByteWrite(0xd0,0x18,MPU6050_RA_GYRO_CONFIG);          //���ٶ����� 2000��/s
                 */
                i2cWrite(PWR_MGMT_1,0x00);
                i2cWrite(SMPLRT_DIV,0x07);
                i2cWrite(CONFIG,0x06);
                i2cWrite(ACCEL_CONFIG,0x01);
                i2cWrite(GYRO_CONFIG,0x18);
         }

unsigned int readMpu16(unsigned char addr){
    unsigned char i,j;
    i=i2cRead(addr);
    j=i2cRead(addr+1);
    return (i<<8)+j;
}

unsigned char *recvBuf;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //recvBuf=(unsigned char *)malloc(sizeof(unsigned char)*2);
    Clock_Init();
    i2c_init();
    MPU6050_Init();
    P6DIR=0xff;
    P6OUT=0xff;
    while(1){
        P6OUT=~(readMpu16(ZAH)>>8);
        //delay_ms(500);
    }

}
