/*
 * mpu6050main.c
 *
 *  Created on: 2021��5��4��
 *      Author: certainly
 */

#include "driverlib.h"
#include "mpu6050.h"
#include "stdio.h"
#include "delay.h"
#include "mpu6050.h"
#include "mpu6050main.h"
#include "mpu6050.h"
int time=0;
int res=0;
//��ʼ��MPU6050
//����ֵ: 0,�ɹ�
//        ����,�������
u8 MPU_Init(void)
{
        u8 res;
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
        for(time=0;time<100;time++)
        {delay_ms(1);}
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);//����MPU6050
        MPU_Set_Gyro_Fsr(3); //�����Ǵ�����,��2000dps
        MPU_Set_Accel_Fsr(0); //���ٶȴ����� ��2g
        MPU_Set_Rate(200); //���ò�����50HZ
        MPU_Write_Byte(MPU_INT_EN_REG,0X00); //�ر������ж�
        MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
        MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);//�ر�FIFO
        MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);//INT���ŵ͵�ƽ��Ч
//        res=MPU_Read_Byte(MPU_INTBP_CFG_REG);
//        printf("%d\r\n",res);
//        delay_s(1);
        res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
//        printf("%d\r\n",res);

        if(res==MPU_ADDR)//����ID��ȷ
        {
            printf("ok\r\n");
            MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X02);//����CLKSEL,PLL X ��Ϊ�ο�
//            res=MPU_Read_Byte(MPU_PWR_MGMT1_REG);
//            printf("%d\r\n",res);
            MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);//���ٶ������Ƕ�����
            MPU_Set_Rate(200); //���ò�����Ϊ50HZ
        }else return 1;
        return 0;
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{


    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ
}

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Accel_Fsr(u8 fsr)
{

    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_LPF(u16 lpf)
{

    u8 data=0;
    if(lpf>=188) data=1;
    else if(lpf>=98) data=2;
    else if(lpf>=42) data=2;
    else if(lpf>=42) data=3;
    else if(lpf>=20) data=4;
    else if(lpf>=10) data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Rate(u16 rate)
{

    u8 data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);  //�������ֵ�ͨ�˲���
    return MPU_Set_LPF(rate/2); //�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{

    u8 buf[2];
    float temp;
    short raw;
        MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((u16)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{

    u8 buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((u16)buf[0]<<8)|buf[1];
        *gy=((u16)buf[2]<<8)|buf[3];
        *gz=((u16)buf[4]<<8)|buf[5];
    }
    return res;
}

//�õ����ٶ�ֵ(ԭʼֵ)
//ax,ay,az:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{

    u8 buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((u16)buf[0]<<8)|buf[1];
        *ay=((u16)buf[2]<<8)|buf[3];
        *az=((u16)buf[4]<<8)|buf[5];
    }
    return res;;
}

//IICдһ���ֽ�
//reg:      �Ĵ�����ַ
//data:     ����
//����ֵ:  0,����
//          ����,�������
u8 MPU_Write_Byte(u8 reg,u8 data)
{
    IIC_Start_mpu6050();
    IIC_Send_Byte_mpu6050((MPU_ADDR<<1)|0);//����������ַ+д����
    if(IIC_Wait_Ack_mpu6050 ()) //�ȴ�Ӧ��
    {
        IIC_Stop_mpu6050 ();
        return 1;
    }
    IIC_Send_Byte_mpu6050(reg); //д�Ĵ�����ַ
    IIC_Wait_Ack_mpu6050 ();     //�ȴ�Ӧ��
    IIC_Send_Byte_mpu6050(data);//��������
    if(IIC_Wait_Ack_mpu6050 ())  //�ȴ�ACK
    {
        IIC_Stop_mpu6050 ();
        return 1;
    }
    IIC_Stop_mpu6050 ();
    return 0;
}

//IIC��һ���ֽ�
//reg:�Ĵ�����ַ
//����ֵ:����������

u8 MPU_Read_Byte(u8 reg)
{
//    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);
    u8 res;
    IIC_Start_mpu6050();
    IIC_Send_Byte_mpu6050((MPU_ADDR<<1)|0);//����������ַ+д����
    IIC_Wait_Ack_mpu6050();//�ȴ�Ӧ��
    IIC_Send_Byte_mpu6050(reg);//д�Ĵ�����ַ
    IIC_Wait_Ack_mpu6050();//�ȴ�Ӧ��
    IIC_Start_mpu6050();
    IIC_Send_Byte_mpu6050((MPU_ADDR<<1)|1);//�����ڼ��ַ+������
    IIC_Wait_Ack_mpu6050();//�ȴ�Ӧ��
    res=IIC_Read_Byte_mpu6050(0);//��ȡ���ݣ�����nACK
    IIC_Stop_mpu6050();//����һ��ֹͣ����
    return res;
}

//IIC����д
//addr:������ַ
//reg: �Ĵ�����ַ
//len: д�볤��
//buf: ������
//����ֵ: 0,����
//              �������������
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
    u8 i;
    IIC_Start_mpu6050();
    IIC_Send_Byte_mpu6050((addr<<1)|0);//����������ַ+д����
    if(IIC_Wait_Ack_mpu6050())//�ȴ�Ӧ��
    {
        IIC_Stop_mpu6050();
        return 1;
    }
    IIC_Send_Byte_mpu6050(reg);//д�Ĵ�����ַ
    IIC_Wait_Ack_mpu6050();//�ȴ�Ӧ��
    for(i=0;i<len;i++)
    {
        IIC_Send_Byte_mpu6050(buf[i]);//��������
        if(IIC_Wait_Ack_mpu6050())//�ȴ�ACK
        {
            IIC_Stop_mpu6050();
            return 1;
        }
    }
    IIC_Stop_mpu6050();
    return 0;
}
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ó���
//buf:��ȡ�������ݴ洢��
//����ֵ: 0,����
//              �������������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{


    IIC_Start_mpu6050();

    IIC_Send_Byte_mpu6050((addr<<1)|0);//����������ַ+д����
    if(IIC_Wait_Ack_mpu6050())//�ȴ�Ӧ��
    {
        IIC_Stop_mpu6050();
        return 1;
    }
    IIC_Send_Byte_mpu6050(reg);//д�Ĵ�����ַ
    IIC_Wait_Ack_mpu6050();//�ȴ�Ӧ��
    IIC_Start_mpu6050();
    IIC_Send_Byte_mpu6050((addr<<1)|1);//����������ַ+������
    IIC_Wait_Ack_mpu6050();//�ȴ�Ӧ��
    while(len)
    {
        if(len==1) *buf=IIC_Read_Byte_mpu6050(0);//�����ݣ�����nACK
        else *buf=IIC_Read_Byte_mpu6050(1);//�����ݣ�����ACK
        len--;
        buf++;
    }
    IIC_Stop_mpu6050();//����һ��ֹͣ����
    return 0;
}



