/*
 * mpu6050.h
 *
 *  Created on: 2021年5月4日
 *      Author: certainly
 */

#ifndef MPU6050_MPU6050_H_
#define MPU6050_MPU6050_H_
#include "driverlib.h"
#include "stdio.h"
#include "delay.h"

#define PORT_mpu6050   P2OUT   //总端口号
#define DIR_mpu6050     P2DIR   //端口方向

#define SDA_Value_mpu6050   (P2IN & BIT6)    //读取SDA引脚的电平值，读之前端口方向设为输入!!!!!!

#define SCK_I2C_OUT_mpu6050  DIR_mpu6050   |=BIT5
#define SCK_I2C_H_mpu6050    PORT_mpu6050   |= BIT5  // SCK
#define SCK_I2C_L_mpu6050    PORT_mpu6050   &= ~BIT5

#define SDA_DIR_OUT_mpu6050     DIR_mpu6050   |= BIT6     //数据端口方向设为输出   SDA
#define SDA_DIR_IN_mpu6050     DIR_mpu6050   &= ~BIT6    //数据端口方向设为输入
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
