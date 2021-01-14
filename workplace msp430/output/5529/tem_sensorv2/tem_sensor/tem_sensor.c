/*
 * tem_sensor.c
 *
 *  Created on: 2020年9月5日
 *      Author: certainly
 */
#include "tem_sensor.h"
#include "delay.h"
#include "driverlib.h"
#include "stdio.h"
//************************************
#define  uint  unsigned int
#define  uchar unsigned char
#define  Nack_counter  10
int bit_out=0;
int bit_in=0;
unsigned char DataL,DataH,pecreg;
//************ 函数声明*****************************************
float transmit(unsigned int a);
void I2C_start_TEM();
void I2C_Stop_TEM();
void I2C_SendACK_TEM(u8 ack);
u8 I2C_RecvACK_TEM();
void I2C_SendByte_TEM(u8 dat);
u8 I2C_RecvByte_TEM();
void Single_Write_TEM(u8 commend,u8 HighBIT,u8 LowBIT);
unsigned int memread(void);
//------------------------------
float transmit(unsigned int a)
{   float b;
   b=0.02*a-273.15;
    return b;
}
void I2C_start_TEM()
{
    SDA_DIR_OUT_TEM;
    SCK_I2C_OUT_TEM;
    SDA_I2C_H_TEM;
    SCK_I2C_H_TEM;
    delay_us(1);
    SDA_I2C_L_TEM;
    delay_us(1);
    SCK_I2C_L_TEM;

}

/**************************************
停止信号
**************************************/
void I2C_Stop_TEM()
{
    SCK_I2C_OUT_TEM;
    SDA_DIR_OUT_TEM;
    SDA_I2C_L_TEM;
    SCK_I2C_L_TEM;
    delay_us(1);                //延时
    SDA_I2C_H_TEM;
    delay_us(1);                //延时
    SCK_I2C_H_TEM;
    delay_us(5);
}
/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void I2C_SendACK_TEM(u8 ack)
{
    SDA_DIR_OUT_TEM;
    SCK_I2C_OUT_TEM;
    if(ack==1) SDA_I2C_H_TEM;
    else       SDA_I2C_L_TEM;
    SCK_I2C_H_TEM;
    delay_us(1);                //延时
    SCK_I2C_L_TEM;
    delay_us(1);                //延时
}

/**************************************
接收应答信号
**************************************/
u8 I2C_RecvACK_TEM()
{
    u8 temp;
    SDA_DIR_IN_TEM;
    SCK_I2C_H_TEM;             //拉高时钟线
    delay_us(1);                //延时
    _NOP();
    if(SDA_Value_TEM) temp=1;
    else temp=0;
    SCK_I2C_L_TEM;          //拉低时钟线
    delay_us(1);                //延时
    SDA_DIR_OUT_TEM;
    return temp;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void I2C_SendByte_TEM(u8 dat)
{
    u8 i;
    SDA_DIR_OUT_TEM;
    SCK_I2C_OUT_TEM;
    for (i=0; i<8; i++)         //8位计数器
    {
        if(dat&0x80) SDA_I2C_H_TEM;
        else SDA_I2C_L_TEM;
        dat <<= 1;              //移出数据的最高位
        SCK_I2C_H_TEM;
        delay_us(1);            //延时
        SCK_I2C_L_TEM;
        delay_us(1);            //延时
    }
//    SDA_I2C_L_TEM;
    I2C_RecvACK_TEM();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
u8 I2C_RecvByte_TEM()
{
    u8 i;
    SDA_DIR_IN_TEM;
    u8 dat = 0;
    SDA_I2C_H_TEM;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;             //拉高时钟线
        SCK_I2C_H_TEM;
        delay_us(1);            //延时
        if(SDA_Value_TEM) dat |= 1;    //读数据
        SCK_I2C_L_TEM; //拉低时钟线
        delay_us(1);            //延时
    }
    SDA_DIR_OUT_TEM;
    return dat;
}
//******单字节写入*******************************************
void Single_Write_TEM(u8 commend,u8 HighBIT,u8 LowBIT)
{
    I2C_start_TEM();                  //起始信号
    I2C_SendByte_TEM(TEM_SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte_TEM(commend);    //内部寄存器地址
    I2C_SendByte_TEM(LowBIT);
    I2C_SendByte_TEM(HighBIT);
    I2C_Stop_TEM();                   //发送停止信号
}
//*****读信息*******************************************
unsigned int memread(void)
{
    I2C_start_TEM();                  //起始信号
    I2C_SendByte_TEM(0xB4);   //发送设备地址+写信号
    I2C_SendByte_TEM(0x07);   //commend
    I2C_start_TEM();                  //重新发送起始信号
    I2C_SendByte_TEM(0xB5);   //发送设备地址+读信号
    DataL=I2C_RecvByte_TEM();
    I2C_SendACK_TEM(0);
    DataH=I2C_RecvByte_TEM();
    I2C_SendACK_TEM(0);
    pecreg=I2C_RecvByte_TEM();
    I2C_SendACK_TEM(1);
    I2C_Stop_TEM();
    return(DataH*256+DataL);
}



