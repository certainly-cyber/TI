/*
 * mpu6050.h
 *
 *  Created on: 2021��5��4��
 *      Author: certainly
 */

#ifndef MPU6050_MPU6050_H_
#define MPU6050_MPU6050_H_
#include "driverlib.h"
#include "stdio.h"
#include "delay.h"

#define PORT_mpu6050   P2OUT   //�ܶ˿ں�
#define DIR_mpu6050     P2DIR   //�˿ڷ���

#define SDA_Value_mpu6050   (P2IN & BIT6)    //��ȡSDA���ŵĵ�ƽֵ����֮ǰ�˿ڷ�����Ϊ����!!!!!!

#define SCK_I2C_OUT_mpu6050  DIR_mpu6050   |=BIT5
#define SCK_I2C_H_mpu6050    PORT_mpu6050   |= BIT5  // SCK
#define SCK_I2C_L_mpu6050    PORT_mpu6050   &= ~BIT5

#define SDA_DIR_OUT_mpu6050     DIR_mpu6050   |= BIT6     //���ݶ˿ڷ�����Ϊ���   SDA
#define SDA_DIR_IN_mpu6050     DIR_mpu6050   &= ~BIT6    //���ݶ˿ڷ�����Ϊ����
#define SDA_I2C_H_mpu6050      PORT_mpu6050   |= BIT6
#define SDA_I2C_L_mpu6050     PORT_mpu6050   &= ~BIT6

void IIC_Start_mpu6050 ();
void IIC_Stop_mpu6050 ();
u8 IIC_Wait_Ack_mpu6050 ();
void IIC_Ack_mpu6050 (void);
void IIC_NAck_mpu6050 (void);
void IIC_Send_Byte_mpu6050 (u8 dat);
u8 IIC_Read_Byte_mpu6050 (unsigned char ack);



#endif /* MPU6050_MPU6050_H_ */
