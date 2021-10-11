/*
 * mpu6050.c
 *
 *  Created on: 2021��5��4��
 *      Author: certainly
 */
#include "driverlib.h"
#include "mpu6050.h"
#include "stdio.h"
#include "delay.h"


void IIC_Start_mpu6050()
{
    SDA_DIR_OUT_mpu6050;
    SCK_I2C_OUT_mpu6050;
    SDA_I2C_H_mpu6050;
    SCK_I2C_H_mpu6050;
    delay_us(4);                //��ʱ
    SDA_I2C_L_mpu6050;
    delay_us(4);                //��ʱ
    SCK_I2C_L_mpu6050;
}

/**************************************
ֹͣ�ź�
**************************************/
void IIC_Stop_mpu6050()
{
    SCK_I2C_OUT_mpu6050;
    SDA_DIR_OUT_mpu6050;
    SCK_I2C_L_mpu6050;
    SDA_I2C_L_mpu6050;
    delay_us(4);                //��ʱ
    SCK_I2C_H_mpu6050;
    SDA_I2C_H_mpu6050;
    delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack_mpu6050()
{
    u8 ucErrTime=0;
    SDA_DIR_IN_mpu6050;
    SCK_I2C_OUT_mpu6050;
    SDA_I2C_H_mpu6050;
    delay_us(1);
    SCK_I2C_H_mpu6050;             //����ʱ����
    delay_us(1);                //��ʱ
    while(SDA_Value_mpu6050)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop_mpu6050();
            return 1;
        }
    }
    SCK_I2C_L_mpu6050;
    return 0;
}
void IIC_Ack_mpu6050(void)
{
    SCK_I2C_OUT_mpu6050;
    SDA_DIR_OUT_mpu6050;
    SCK_I2C_L_mpu6050;
    SDA_I2C_L_mpu6050;
    delay_us(2);
    SCK_I2C_H_mpu6050;
    delay_us(2);
    SCK_I2C_L_mpu6050;
}
//������ACKӦ��
void IIC_NAck_mpu6050(void)
{
    SCK_I2C_OUT_mpu6050;
    SDA_DIR_OUT_mpu6050;
    SCK_I2C_L_mpu6050;
    SDA_I2C_H_mpu6050;
    delay_us(2);
    SCK_I2C_H_mpu6050;
    delay_us(2);
    SCK_I2C_L_mpu6050;
}

void IIC_Send_Byte_mpu6050(u8 dat)
{
    u8 i;
    SDA_DIR_OUT_mpu6050;
    SCK_I2C_OUT_mpu6050;
    SCK_I2C_L_mpu6050; //����ʱ�ӿ�ʼ���ݴ���
    for (i=0; i<8; i++)         //8λ������
    {
        if(dat&0x80) SDA_I2C_H_mpu6050;
        else SDA_I2C_L_mpu6050;
        dat <<= 1;              //�Ƴ����ݵ����λ
        delay_us(2);
        SCK_I2C_H_mpu6050;
        delay_us(2);            //��ʱ
        SCK_I2C_L_mpu6050;
        delay_us(2);            //��ʱ
    }
}


//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte_mpu6050(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_DIR_IN_mpu6050;
    SCK_I2C_OUT_mpu6050;
    for (i=0; i<8; i++)         //8λ������
    {
        SCK_I2C_L_mpu6050;
        delay_us(2);            //��ʱ
        SCK_I2C_H_mpu6050;
        receive <<= 1;
        if(SDA_Value_mpu6050) receive |= 1;    //������
        delay_us(1);

    }
    if (!ack)
        IIC_NAck_mpu6050();//����nACK
    else
        IIC_Ack_mpu6050(); //����ACK

    return receive;
}




