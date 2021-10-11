/*
 * fdc2214.c
 *
 *  Created on: 2021年5月4日
 *      Author: certainly
 */
#include "driverlib.h"
#include "fdc2214.h"
#include "stdio.h"
#include "delay.h"

void IIC_Start_fdc2214()
{
    SDA_DIR_OUT_fdc2214;
    SCK_I2C_OUT_fdc2214;
    SDA_I2C_H_fdc2214;
    SCK_I2C_H_fdc2214;
    delay_us(4);                //延时
    SDA_I2C_L_fdc2214;
    delay_us(4);                //延时
    SCK_I2C_L_fdc2214;
}

/**************************************
停止信号
**************************************/
void IIC_Stop_fdc2214()
{
    SCK_I2C_OUT_fdc2214;
    SDA_DIR_OUT_fdc2214;
    SDA_I2C_L_fdc2214;
    SCK_I2C_L_fdc2214;
    delay_us(4);                //延时
    SDA_I2C_H_fdc2214;
    SCK_I2C_H_fdc2214;
    delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack_fdc2214()
{
    u8 temp;
    SDA_DIR_IN_fdc2214;
    SCK_I2C_OUT_fdc2214;
    SCK_I2C_H_fdc2214;             //拉高时钟线
    delay_us(1);                //延时
    if(SDA_Value_fdc2214) {temp=1; IIC_Stop_fdc2214();     SDA_DIR_OUT_fdc2214;   return temp;}
    else
    {temp=0;    }
    SCK_I2C_L_fdc2214;          //拉低时钟线
    SDA_DIR_OUT_fdc2214;
    return temp;
}
void IIC_Ack_fdc2214(void)
{
    SCK_I2C_OUT_fdc2214;
    SDA_DIR_OUT_fdc2214;
    SCK_I2C_L_fdc2214;
    SDA_I2C_L_fdc2214;
    delay_us(2);
    SCK_I2C_H_fdc2214;
    delay_us(2);
    SCK_I2C_L_fdc2214;
}
//不产生ACK应答
void IIC_NAck_fdc2214(void)
{
    SCK_I2C_OUT_fdc2214;
    SDA_DIR_OUT_fdc2214;
    SCK_I2C_L_fdc2214;
    SDA_I2C_H_fdc2214;
    delay_us(2);
    SCK_I2C_H_fdc2214;
    delay_us(2);
    SCK_I2C_L_fdc2214;
}

void IIC_Send_Byte_fdc2214(u8 dat)
{
    u8 i;
    SDA_DIR_OUT_fdc2214;
    SCK_I2C_OUT_fdc2214;
    SCK_I2C_L_fdc2214; //拉低时钟开始数据传输
    for (i=0; i<8; i++)         //8位计数器
    {
        if(dat&0x80) SDA_I2C_H_fdc2214;
        else SDA_I2C_L_fdc2214;
        dat <<= 1;              //移出数据的最高位
        delay_us(2);
        SCK_I2C_H_fdc2214;
        delay_us(2);            //延时
        SCK_I2C_L_fdc2214;
        delay_us(2);            //延时
    }
}


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte_fdc2214(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_DIR_IN_fdc2214;
    SCK_I2C_OUT_fdc2214;
    for (i=0; i<8; i++)         //8位计数器
    {
        SCK_I2C_L_fdc2214;
        delay_us(2);            //延时
        SCK_I2C_H_fdc2214;
        receive <<= 1;
        if(SDA_Value_fdc2214) receive |= 1;    //读数据
        delay_us(2);

    }
    if (!ack)
        IIC_NAck_fdc2214();//发送nACK
    else
        IIC_Ack_fdc2214(); //发送ACK

    return receive;
}


