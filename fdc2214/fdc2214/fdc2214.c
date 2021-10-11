/*
 * fdc2214.c
 *
 *  Created on: 2021��5��4��
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
    delay_us(4);                //��ʱ
    SDA_I2C_L_fdc2214;
    delay_us(4);                //��ʱ
    SCK_I2C_L_fdc2214;
}

/**************************************
ֹͣ�ź�
**************************************/
void IIC_Stop_fdc2214()
{
    SCK_I2C_OUT_fdc2214;
    SDA_DIR_OUT_fdc2214;
    SDA_I2C_L_fdc2214;
    SCK_I2C_L_fdc2214;
    delay_us(4);                //��ʱ
    SDA_I2C_H_fdc2214;
    SCK_I2C_H_fdc2214;
    delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack_fdc2214()
{
    u8 temp;
    SDA_DIR_IN_fdc2214;
    SCK_I2C_OUT_fdc2214;
    SCK_I2C_H_fdc2214;             //����ʱ����
    delay_us(1);                //��ʱ
    if(SDA_Value_fdc2214) {temp=1; IIC_Stop_fdc2214();     SDA_DIR_OUT_fdc2214;   return temp;}
    else
    {temp=0;    }
    SCK_I2C_L_fdc2214;          //����ʱ����
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
//������ACKӦ��
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
    SCK_I2C_L_fdc2214; //����ʱ�ӿ�ʼ���ݴ���
    for (i=0; i<8; i++)         //8λ������
    {
        if(dat&0x80) SDA_I2C_H_fdc2214;
        else SDA_I2C_L_fdc2214;
        dat <<= 1;              //�Ƴ����ݵ����λ
        delay_us(2);
        SCK_I2C_H_fdc2214;
        delay_us(2);            //��ʱ
        SCK_I2C_L_fdc2214;
        delay_us(2);            //��ʱ
    }
}


//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte_fdc2214(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_DIR_IN_fdc2214;
    SCK_I2C_OUT_fdc2214;
    for (i=0; i<8; i++)         //8λ������
    {
        SCK_I2C_L_fdc2214;
        delay_us(2);            //��ʱ
        SCK_I2C_H_fdc2214;
        receive <<= 1;
        if(SDA_Value_fdc2214) receive |= 1;    //������
        delay_us(2);

    }
    if (!ack)
        IIC_NAck_fdc2214();//����nACK
    else
        IIC_Ack_fdc2214(); //����ACK

    return receive;
}


