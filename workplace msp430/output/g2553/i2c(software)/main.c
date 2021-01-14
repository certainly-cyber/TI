#include "msp430g2553.h"
#define uchar unsigned char
#define uint unsigned int
//=====================================延时函数=================================
#define  CPU_F ((double)8000000)
#define  delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define  delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
//=====================================IIC地址设置=================================
#define  WHO_AM_I         0x75           //IIC地址寄存器(默认数值0x68，只读)--（6050）
#define  SlaveAddress     0x80           //IIC写入时的地址字节数据，+1为读取--（PCA9685）
//=====================================IIC端口定义=================================
#define  SCL1  P1OUT|=BIT4
#define  SCL0  P1OUT&= ~BIT4

#define  SDA1  P1OUT|=BIT5               //IIC数据引脚
#define  SDA0  P1OUT&= ~BIT5
#define  SDAIN  P1DIR&= ~BIT5
#define  SDAOUT  P1DIR|=BIT5
#define  SDADATA  (P1IN&BIT5)
//================================I2C起始信号================================
void I2C_Start()
{
    SDA1;               //拉高数据线
    SCL1;               //拉高时钟线
    delay_us(5);         //延时
    SDA0;               //产生下降沿
    delay_us(5);
    SCL0;               //拉低时钟线
}
//================================I2C停止信号================================
void I2C_Stop()
{
    SDA0;               //拉高数据线
    SCL1;
    delay_us(5);
    SDA1;               //产生上升沿
    delay_us(5);
}
//================================I2C发送应答信号 入口参数:ack (0:ACK 1:NAK)================================
void I2C_SendACK(uchar ack)
{
    SDAOUT;
    if(ack)
        SDA1;
    else
        SDA0;
    //SDA=ack;          //读应答信号
    SCL1;
    delay_us(5);
    SCL0;               //拉低时钟线
    delay_us(5);
}
//================================I2C接收应答信号================================
uchar I2C_RecvACK()
{
    uchar cy;
    SCL1;
    SDAIN;
    delay_us(5);
    if(SDADATA)
    {
        cy=1;
    }
    else
    {
        cy=0;
    }
    //cy=SDA;           //读应答信号
    SCL0;
    delay_us(5);
    SDAOUT;
    return cy;
}
//================================向I2C总线发送一个字节数据================================
void I2C_SendByte(uchar dat)
{
    uchar i;
    for(i=0;i<8;i++)    //8位计数器
    {
        if((dat<<i)&0x80)
        {
            SDA1;
        }
        else
        {
            SDA0;
        }
        //SDA=cy;       //送数据口
        SCL1;
        delay_us(5);
        SCL0;
        delay_us(5);
    }
    I2C_RecvACK();
}
//================================从I2C总线接收一个字节数据================================
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat=0,cy;
    SDA1;               //使能内部上拉,准备读取数据,
    SDAIN;              //8位计数器
    for(i=0;i<8;i++)
    {
        dat<<=1;
        SCL1;
        delay_us(5);
        if(SDADATA)
        {
            cy=1;
        }
        else
        {
            cy=0;
        }
        dat|=cy;        //读数据
        SCL0;
        delay_us(5);
    }
    SDAOUT;
    return dat;
}
//=======================向I2C设备写入一个字节数据======================================
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                        //起始信号
    I2C_SendByte(SlaveAddress);         //发送设备地址+写信号
    I2C_SendByte(REG_Address);          //内部寄存器地址
    I2C_SendByte(REG_data);             //内部寄存器数据
    I2C_Stop();                         //发送停止信号
}
//======================从I2C设备读取一个字节数据=====================================
uchar Single_ReadI2C(uchar REG_Address)
{
    uchar REG_data;

    I2C_Start();                        //起始信号
    I2C_SendByte(SlaveAddress);         //发送设备地址+写信号
    I2C_SendByte(REG_Address);          //发送存储单元地址，从0开始
    I2C_Start();                        //起始信号
    I2C_SendByte(SlaveAddress+1);       //发送设备地址+读信号
    REG_data=I2C_RecvByte();            //读出寄存器数据
    I2C_SendACK(1);                     //接收应答信号
    I2C_Stop();                         //停止信号

    return REG_data;
}
//======================================合成数据======================================
int GetData(uchar REG_Address)
{
    char H,L;
    H=Single_ReadI2C(REG_Address);
    L=Single_ReadI2C(REG_Address+1);
    return (H<<8)+L;                    //合成数据
}
//======================================接收多个数据（未经过验证）======================================
unsigned char  I2C_ReadNData(unsigned char address, unsigned char *pRead, unsigned char len )
{
    unsigned char i;
    I2C_Start();                        //起始信号
    I2C_SendByte(SlaveAddress);         //发送设备地址+写信号
    I2C_SendByte(address);          //发送存储单元地址，从0开始
    I2C_Start();                        //起始信号
    I2C_SendByte(SlaveAddress+1);       //发送设备地址+读信号
    for( i= 0; i< len -1 ; i++)

    {

        *pRead++= I2C_RecvByte();

     }


    I2C_SendACK(1);                     //接收应答信号
    I2C_Stop();                         //发送停止信号

    *pRead = UCB0RXBUF;

    return 0;


}
//======================================发送多个数据（未经过验证）======================================

unsigned char I2C_WriteNData( unsigned char address, unsigned char *pWbuf, unsigned char len)
{
       unsigned char i;
       unsigned char REG_data;
       I2C_Start();                        //起始信号
       I2C_SendByte(SlaveAddress);         //发送设备地址+写信号
       I2C_SendByte(address);          //内部寄存器地址
       for( i= 0; i < len; i++)

       {

           REG_data = *pWbuf++;      // 发送寄存器内容
           I2C_SendByte(REG_data);             //内部寄存器数据


        }
       I2C_Stop();                         //发送停止信号
       return 0;

}
